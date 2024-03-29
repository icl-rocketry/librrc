#include "nrcremotepyro.h"
#include <esp_task.h>
#include <functional>
#include <Arduino.h>
#include <atomic>



void NRCRemotePyro::setup()
{
    pinMode(_firePin,OUTPUT);
    digitalWrite(_firePin,LOW);
    pinMode(_contPin,INPUT);
    _state.newFlag(COMPONENT_STATUS_FLAGS::DISARMED);
    updateContinuity();
}


void NRCRemotePyro::arm_impl(packetptr_t packetptr)
{
    SimpleCommandPacket armingpacket(*packetptr);

    if(armingpacket.arg != 1){
        _contCheckOverride = false;
    }
    else{
        _contCheckOverride = true;  
    }

    updateContinuity();
    // NRCRemoteActuatorBase::arm_impl(std::move(packetptr));

    if (_state.getStatus() == static_cast<uint16_t>(COMPONENT_STATUS_FLAGS::DISARMED) || 
        (_state.flagSetAnd(COMPONENT_STATUS_FLAGS::DISARMED,COMPONENT_STATUS_FLAGS::ERROR_CONTINUITY) && _contCheckOverride))
        {
            _state.deleteFlag(COMPONENT_STATUS_FLAGS::DISARMED);
            _state.newFlag(COMPONENT_STATUS_FLAGS::NOMINAL);
        }


}

void NRCRemotePyro::updateContinuity()
{
    if (digitalRead(_contPin))
    {
        if (_state.flagSet(COMPONENT_STATUS_FLAGS::ERROR_CONTINUITY))
        {
            _state.deleteFlag(COMPONENT_STATUS_FLAGS::ERROR_CONTINUITY);
        }
    }
    else
    {
        
    if (!_state.flagSet(COMPONENT_STATUS_FLAGS::ERROR_CONTINUITY))
        {
            _state.newFlag(COMPONENT_STATUS_FLAGS::ERROR_CONTINUITY);
        }
    }
}

void NRCRemotePyro::execute_impl(packetptr_t packetptr)
{
    SimpleCommandPacket execute_command(*packetptr);

    digitalWrite(_firePin,HIGH);

    struct TaskData_t{
        uint8_t firePin;
        uint32_t param;
        std::atomic<bool> &taskDeleted;
    };

    TaskData_t taskdata{_firePin,execute_command.arg,_taskDeleted};

    if ((async_off_task_handle != nullptr) && (eTaskGetState(async_off_task_handle) != eTaskState::eDeleted))
    {
        if (!_taskDeleted){
            vTaskDelete(async_off_task_handle); // remove previous running task and replace with new task
        }
        async_off_task_handle = nullptr;
    }
    

    // spawn task to switch off pin given timeout param wiht higher prioirty so sleeping starts immediatley and non static task data is copied immediatley
    xTaskCreatePinnedToCore([](void *pvParameters)
                            {
                                // create local copy of data as task data is not static
                                TaskData_t taskdata = *reinterpret_cast<TaskData_t *>(pvParameters);

                                TickType_t xLastWakeTime = xTaskGetTickCount();
                                vTaskDelayUntil(&xLastWakeTime, taskdata.param / portTICK_PERIOD_MS); // sleep for required amount
                                digitalWrite(taskdata.firePin,LOW);
                                taskdata.taskDeleted = true;
                                vTaskDelete(NULL); // delete task 
                            },
                            "nukeofftask",
                            1000,
                            (void *)&taskdata,
                            2,
                            &async_off_task_handle,
                            1);
    _taskDeleted = false;
    vTaskDelay(1);

}

void NRCRemotePyro::getstate_impl(packetptr_t packetptr)
{
    updateContinuity();
    NRCRemoteBase::getstate_impl(std::move(packetptr));
}