#pragma once

#include "nrcremoteactuatorbase.h"

#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>
// #include <esp_task.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <functional>
#include <atomic>
#include <array>

#include <librrc/HAL/arduinogpio.h>
#include <librrc/componentstatusflags.h>



template<typename GPIOHAL = ArduinoGpio>
class NRCRemotePyro : public NRCRemoteActuatorBase<NRCRemotePyro>
{
    //type alias for ease of use
    using GPIOHAL::PINMODE;
    using GPIOHAL::LEVEL;

    public:

        template<typename GPIOHAL> NRCRemotePyro(uint8_t firePin, uint8_t contPin, RnpNetworkManager &networkmanager) -> NRCRemotePyro<ArduinoGpio>;
        NRCRemotePyro(uint8_t firePin, uint8_t contPin, RnpNetworkManager &networkmanager) : 
        NRCRemotePyro(ArduinoGpio(firePin),ArduinoGpio(contPin), networkmanager)
        {};

        NRCRemotePyro(GPIOHAL firePin, GPIOHAL contPin, RnpNetworkManager &networkmanager) : 
        NRCRemoteActuatorBase(networkmanager),
        m_firePin(firePin),
        m_contPin(contPin),
        offTimeUpdated(false){};

        void setup()
        {
            m_firePin.pinMode(GPIO_OUTPUT);
            m_firePin.digitalWrite(GPIO_LOW);
            m_contPin.pinMode(GPIO_INPUT);

            _state.newFlag(LIBRRC::COMPONENT_STATUS_FLAGS::DISARMED);
            updateContinuity();

            if (!spawnOffTask())
            {
                _state.newFlag(LIBRRC::COMPONENT_STATUS_FLAGS::ERROR);
            }
        }

        ~NRCRemotePyro()
        {
            if ((offTaskHandle != nullptr) && (eTaskGetState(offTaskHandle) != eTaskState::eDeleted))
            {
                vTaskDelete(offTaskHandle); // I beg this doesnt crash
                offTaskHandle = nullptr;
            }
        };

    protected: //Variables
        friend class NRCRemoteActuatorBase;
        friend class NRCRemoteBase;
        GPIOHAL m_firePin;
        GPIOHAL m_contPin;
        bool _contCheckOverride;

        // Task Control stuff
        /**
         * @brief Task Handle
         *
         */
        TaskHandle_t offTaskHandle = nullptr;

        /**
         * @brief Task control block
         *
         */
        StaticTask_t offTaskTCB;

        /**
         * @brief Task stack size
         *
         */
        static constexpr int offTaskStackSize = 100;
        /**
         * @brief Task stack
         *
         */
        std::array<StackType_t, offTaskStackSize> taskStack;
        /**
         * @brief Atomic off time for task
         *
         */
        std::atomic<int32_t> offTime;
        std::atomic<bool> offTimeUpdated;

    protected: //Methods

        void arm(int32_t arg)
        {

        };
        void execute(int32_t arg)
        {

        };
        void disarm()
        {

        };

        void arm_impl(packetptr_t packetptr)
        {
            SimpleCommandPacket armingpacket(*packetptr);

            if (armingpacket.arg != 1)
            {
                _contCheckOverride = false;
            }
            else
            {
                _contCheckOverride = true;
            }

            updateContinuity();

            if (_state.getStatus() == static_cast<uint16_t>(LIBRRC::COMPONENT_STATUS_FLAGS::DISARMED) ||
                (_state.flagSetAnd(LIBRRC::COMPONENT_STATUS_FLAGS::DISARMED, LIBRRC::COMPONENT_STATUS_FLAGS::ERROR_CONTINUITY) && _contCheckOverride))
            {
                _state.deleteFlag(LIBRRC::COMPONENT_STATUS_FLAGS::DISARMED);
                _state.newFlag(LIBRRC::COMPONENT_STATUS_FLAGS::NOMINAL);
            }
        }

        void updateContinuity()
        {
            if (m_contPin.digitalRead())
            {
                if (_state.flagSet(LIBRRC::COMPONENT_STATUS_FLAGS::ERROR_CONTINUITY))
                {
                    _state.deleteFlag(LIBRRC::COMPONENT_STATUS_FLAGS::ERROR_CONTINUITY);
                }
            }
            else
            {

                if (!_state.flagSet(LIBRRC::COMPONENT_STATUS_FLAGS::ERROR_CONTINUITY))
                {
                    _state.newFlag(LIBRRC::COMPONENT_STATUS_FLAGS::ERROR_CONTINUITY);
                }
            }
        }

        void execute_impl(packetptr_t packetptr)
        {
            SimpleCommandPacket execute_command(*packetptr);

            if (execute_command.arg < 0)
            {
                return;
            }
            // update task data
            offTime = execute_command.arg;
            offTimeUpdated = true;
            eTaskState offTaskState = eTaskGetState(offTaskHandle);

            if (offTaskState == eTaskState::eBlocked)
            {
                // abort any delay if the task is blocked
                xTaskAbortDelay(offTaskHandle);
            }
            else if (offTaskState == eTaskState::eSuspended)
            {
                // unsuspend off task
                vTaskResume(offTaskHandle);
            }
            else if ((offTaskState != eTaskState::eReady) || (offTaskState != eTaskState::eRunning))
            {
                _state.newFlag(LIBRRC::COMPONENT_STATUS_FLAGS::ERROR);
            }
        }

        void getstate_impl(packetptr_t packetptr)
        {
            updateContinuity();
            NRCRemoteBase::getstate_impl(std::move(packetptr));
        }

        bool spawnOffTask()
        {
            // spawn off Task
            struct TaskData_t
            {
                GPIOHAL firePin;
                std::atomic<int32_t> &offTime;
                std::atomic<bool> &offTimeUpdated;
            }; 

            TaskData_t taskdata{m_firePin, offTime, offTimeUpdated};

            // spawn task to switch off pin given timeout param
            // spawn with higher priority than calling thread so that the scoped taskdata is copied before it goes out of scope. (this feels fragile???)
            offTaskHandle = xTaskCreateStaticPinnedToCore([](void *pvParameters)
                                                        {
                                                            // create local copy of data as task data is not static
                                                            TaskData_t taskdata = *reinterpret_cast<TaskData_t *>(pvParameters);
                                                            vTaskSuspend(NULL); // suspend task waiting for first
                                                            for (;;)
                                                            {
                                                                taskdata.firePin.digitalWrite(GPIO_HIGH);
                                                                taskdata.offTimeUpdated = false;
                                                                vTaskDelay(taskdata.offTime / portTICK_PERIOD_MS); // sleep for required amount
                                                                // check if offTime has been updated while we were asleep
                                                                if (taskdata.offTimeUpdated)
                                                                {
                                                                    continue; // skip switching off the pyro and continue sleeping with new offTime
                                                                }

                                                                taskdata.firePin.digitalWrite(GPIO_LOW);
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
};