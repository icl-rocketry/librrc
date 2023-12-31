#include "nrcremotepyro.h"
#include <esp_task.h>
#include <functional>
#include <Arduino.h>
#include <atomic>

NRCRemotePyro::NRCRemotePyro(uint8_t firePin,uint8_t contPin,RnpNetworkManager &networkmanager) : 
    NRCRemoteActuatorBase(networkmanager),
    _firePin(firePin),
    _contPin(contPin),
    offTimeUpdated(false)
    {};


NRCRemotePyro::~NRCRemotePyro()
{
    if ((offTaskHandle != nullptr) && (eTaskGetState(offTaskHandle) != eTaskState::eDeleted))
    {
        vTaskDelete(offTaskHandle); //I beg this doesnt crash
        offTaskHandle = nullptr;
    }
}

void NRCRemotePyro::setup()
{
    pinMode(_firePin,OUTPUT);
    digitalWrite(_firePin,LOW);
    pinMode(_contPin,INPUT);
    _state.newFlag(COMPONENT_STATUS_FLAGS::DISARMED);
    updateContinuity();

    if (!spawnOffTask())
    {
        _state.flagSet(COMPONENT_STATUS_FLAGS::ERROR);
    }
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
    //update task data
    offTime = execute_command.arg;
    offTimeUpdated = true;
    eTaskState offTaskState = eTaskGetState(offTaskHandle);

    if (offTaskState == eTaskState::eBlocked)
    {
        //abort any delay if the task is blocked
        xTaskAbortDelay(offTaskHandle);
    }
    else if (offTaskState == eTaskState::eSuspended)
    {
        //unsuspend off task
        vTaskResume(offTaskHandle);
    }
    else if ((offTaskState != eTaskState::eReady) || (offTaskState != eTaskState::eRunning))
    {
        _state.newFlag(COMPONENT_STATUS_FLAGS::ERROR);
    }
    


}

void NRCRemotePyro::getstate_impl(packetptr_t packetptr)
{
    updateContinuity();
    NRCRemoteBase::getstate_impl(std::move(packetptr));
}

bool NRCRemotePyro::spawnOffTask()
{
    // spawn off Task
    struct TaskData_t
    {
        uint8_t firePin;
        std::atomic<uint32_t> &offTime;
        std::atomic<bool> &offTimeUpdated;
    };

    TaskData_t taskdata{_firePin, offTime, offTimeUpdated};

    // spawn task to switch off pin given timeout param
    // spawn with higher priority than calling thread so that the scoped taskdata is copied before it goes out of scope. (this feels fragile???)
    offTaskHandle = xTaskCreateStaticPinnedToCore([](void *pvParameters)
                                                  {
                                                      // create local copy of data as task data is not static
                                                      TaskData_t taskdata = *reinterpret_cast<TaskData_t *>(pvParameters);
                                                      vTaskSuspend(NULL); // suspend task waiting for first
                                                      for (;;)
                                                      {
                                                          digitalWrite(taskdata.firePin, HIGH);
                                                          taskdata.offTimeUpdated = false;
                                                          vTaskDelay(taskdata.offTime / portTICK_PERIOD_MS); // sleep for required amount
                                                          // check if offTime has been updated while we were asleep
                                                          if (taskdata.offTimeUpdated)
                                                          {
                                                              continue; // skip switching off the pyro and continue sleeping with new offTime
                                                          }

                                                          digitalWrite(taskdata.firePin, LOW);
                                                          // suspend off task when timeout is finished
                                                          vTaskSuspend(NULL);
                                                      }
                                                      // vTaskDelete(NULL); // delete task
                                                  },
                                                  "nukeofftask", 
                                                  offTaskStackSize, 
                                                  (void *)&taskdata, 
                                                  2, 
                                                  taskStack.data(), 
                                                  &offTaskTCB, 
                                                  1);
    
    vTaskDelay(1); // make sure the offTask is spawned -> i dont think this is necessary but i had it here before so oh well

    if (offTaskHandle == nullptr)
    {
        return false;
        Serial.println("Kiran messed up task stack allocation, get stack highwater mark and make fun of him");
    }
    return true;
}