#pragma once

#include "nrcremoteactuatorbase.h"

#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <functional>
#include <atomic>
#include <array>

#include <librrc/HAL/arduinogpio.h>
#include <librrc/componentstatusflags.h>
#include <librrc/Local/remoteactuatoradapter.h>

template<typename GPIOHAL = ArduinoGpio>
class NRCRemotePyro : public NRCRemoteActuatorBase<NRCRemotePyro<GPIOHAL>>
{
    //type alias for ease of use
    using PINMODE = typename GPIOHAL::PINMODE;

    //type alias for explict NRCRemoteBase type
    using NRCRemoteActuatorBase_T = NRCRemoteActuatorBase<NRCRemotePyro>;
    using NRCRemoteBase_T = NRCRemoteBase<NRCRemotePyro>;
    

    public:

       
        // template<typename T> NRCRemotePyro(uint8_t firePin, uint8_t contPin, RnpNetworkManager &networkmanager) -> NRCRemotePyro<ArduinoGpio>;
        // /**
        //  * @brief Construct a new NRCRemotePyro object with default arduino gpio for backwards compatibility
        //  * 
        //  * @param firePin 
        //  * @param contPin 
        //  * @param networkmanager 
        //  */
        // NRCRemotePyro(uint8_t firePin, uint8_t contPin, RnpNetworkManager &networkmanager) : 
        // NRCRemotePyro(ArduinoGpio(firePin),ArduinoGpio(contPin), networkmanager)
        // {};

        /**
         * @brief Construct a new NRCRemotePyro object with generic GPIOHAL 
         * 
         * @param firePin 
         * @param contPin 
         * @param networkmanager 
         */
        NRCRemotePyro(GPIOHAL firePin, GPIOHAL contPin, RnpNetworkManager &networkmanager) : 
        NRCRemoteActuatorBase_T(networkmanager),
        m_firePin(firePin),
        m_contPin(contPin)
        {};

        /**
         * @brief Setup pyro and spawn background task
         * 
         */
        void setup()
        {
            m_firePin.pinMode(PINMODE::GPIO_OUTPUT);
            m_firePin.digitalWrite(0);
            m_contPin.pinMode(PINMODE::GPIO_INPUT);

            this->_state.newFlag(LIBRRC::COMPONENT_STATUS_FLAGS::DISARMED);
            updateContinuity();

            if (!spawnOffTask())
            {
                this->_state.newFlag(LIBRRC::COMPONENT_STATUS_FLAGS::ERROR);
            }
        }

        /**
         * @brief Destroy the NRCRemotePyro object, ensuring background task is deleted.
         * 
         */
        ~NRCRemotePyro()
        {
            if ((offTaskHandle != nullptr) && (eTaskGetState(offTaskHandle) != eTaskState::eDeleted))
            {
                vTaskDelete(offTaskHandle); // I beg this doesnt crash
                offTaskHandle = nullptr;
            }
        };

    protected:  
    
        //NRCRemote Interface

        void arm_impl(packetptr_t packetptr)
        {
            SimpleCommandPacket armingpacket(*packetptr);
            arm(armingpacket.arg);
        }

        void execute_impl(packetptr_t packetptr)
        {
            SimpleCommandPacket execute_command(*packetptr);
            execute(execute_command.arg);
        }

        void getstate_impl(packetptr_t packetptr)
        {
            updateState();
            this->NRCRemoteBase<NRCRemotePyro>::getstate_impl(std::move(packetptr));
        }

        
        //Adapter Interface

        friend class RemoteActuatorAdapter<NRCRemotePyro<GPIOHAL>>;
    public:
        /**
         * @brief Arm pyro, if arg == 1, continuity is ignored
         * 
         * @param arg 
         */
        void arm(int32_t arg)
        {
            if (arg != 1)
            {
                m_contCheckOverride = false;
            }
            else
            {
                m_contCheckOverride = true;
            }

            updateContinuity();

            if (this->_state.getStatus() == static_cast<uint16_t>(LIBRRC::COMPONENT_STATUS_FLAGS::DISARMED) ||
                (this->_state.flagSetAnd(LIBRRC::COMPONENT_STATUS_FLAGS::DISARMED, LIBRRC::COMPONENT_STATUS_FLAGS::ERROR_CONTINUITY) && m_contCheckOverride))
            {
                this->_state.deleteFlag(LIBRRC::COMPONENT_STATUS_FLAGS::DISARMED);
                this->_state.newFlag(LIBRRC::COMPONENT_STATUS_FLAGS::NOMINAL);
            }
        };

        /**
         * @brief Disarm Method for Adapter Interface
         * 
         */
        void disarm()
        {
            this->_state.deleteFlag(LIBRRC::COMPONENT_STATUS_FLAGS::NOMINAL);
            this->_state.newFlag(LIBRRC::COMPONENT_STATUS_FLAGS::DISARMED);
        };
    public:
        /**
         * @brief Pyro exectutor
         * 
         * @param arg time the pyro is switched on for
         */
        void execute(int32_t arg)
        {
            if (arg < 0)
            {
                return;
            }
            
            //check if offTaskHandle is nullptr

            if (offTaskHandle == nullptr)
            {
                this->_state.newFlag(LIBRRC::COMPONENT_STATUS_FLAGS::ERROR);
                //maybe log this somewehere too
                offTimeDeadline = 0;
                return;
            }

            // update task data
            offTimeDeadline = millis() + arg;
            uint32_t notificationValue = static_cast<uint32_t>(arg);
            xTaskNotify(offTaskHandle,notificationValue,eSetValueWithOverwrite);

        };

        /**
         * @brief Update the state and current value
         * 
         */
        void updateState()
        {
            updateContinuity();
            if (offTimeDeadline <= millis())
            {
                this->_value = 0;
            }
            else
            {
                this->_value = static_cast<int32_t>(offTimeDeadline-millis());
            }
        };

        /**
         * @brief Check continuity of pyro
         * 
         */
        void updateContinuity()
        {
            if (m_contPin.digitalRead())
            {
                if (this->_state.flagSet(LIBRRC::COMPONENT_STATUS_FLAGS::ERROR_CONTINUITY))
                {
                    this->_state.deleteFlag(LIBRRC::COMPONENT_STATUS_FLAGS::ERROR_CONTINUITY);
                }
            }
            else
            {

                if (!this->_state.flagSet(LIBRRC::COMPONENT_STATUS_FLAGS::ERROR_CONTINUITY))
                {
                    this->_state.newFlag(LIBRRC::COMPONENT_STATUS_FLAGS::ERROR_CONTINUITY);
                }
            }
        }

        /**
         * @brief Spawn task to handle switching off the pyro channel
         * 
         * @return true task started succsefully
         * @return false task failed to start
         */
        bool spawnOffTask()
        {
            // spawn off Task
            struct TaskData_t
            {
                GPIOHAL firePin;
            }; 

            TaskData_t taskdata{m_firePin};

            // spawn task to switch off pin given timeout param
            // spawn with higher priority than calling thread so that the scoped taskdata is copied before it goes out of scope. (this feels fragile???)
            offTaskHandle = xTaskCreateStaticPinnedToCore([](void *pvParameters)
                                                        {
                                                            // create local copy of data as task data is not static
                                                            TaskData_t taskdata = *reinterpret_cast<TaskData_t *>(pvParameters);
                                                            uint32_t notificationData = 0;
                                                            BaseType_t notifyReturn = pdTRUE;

                                                            for (;;)
                                                            {
                                                                //block on notification here
                                                                xTaskNotifyWait(0,0,&notificationData,portMAX_DELAY);

                                                                taskdata.firePin.digitalWrite(1);
                                                                //block on notification for givien time period
                                                                while (notifyReturn == pdTRUE) // if we receieve a new request to keep the pyro on during the wait
                                                                {
                                                                    TickType_t ticksToWait = notificationData / portTICK_PERIOD_MS;
                                                                    // xTaskNotifyWait returns pdFalse if no notification is recived during timeout, i.e pyro on time has been fully waited
                                                                    notifyReturn = xTaskNotifyWait(0,0,&notificationData, ticksToWait);
                                                                }

                                                                //can only exit if notifyRetunr == pdFalse
                                                                taskdata.firePin.digitalWrite(0);
                                                            }
                                                            
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

    protected: //Variables

        // template<typename T> friend class NRCRemoteActuatorBase<NRCRemotePyro<T>>;
        // template<typename T> friend class NRCRemoteBase<NRCRemoteActuatorBase<NRCRemotePyro<T>>>;


        template<typename T> friend class NRCRemoteActuatorBase;
        template<typename T> friend class NRCRemoteBase;
       
        // friend class NRCRemoteBase;

        // friend class NRCRemoteActuatorBase;
        // friend class NRCRemoteBase;

        /**
         * @brief gpio fire pin to trigger pyro
         * 
         */
        GPIOHAL m_firePin;

        /**
         * @brief gpio continuity pin to check continuity across pyro
         * 
         */
        GPIOHAL m_contPin;

        /**
         * @brief Flag indicating if continuity checking is overriden
         * 
         */
        bool m_contCheckOverride;

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
        static constexpr int offTaskStackSize = 10000;
        /**
         * @brief Task stack
         *
         */
        std::array<StackType_t, offTaskStackSize> taskStack;

        /**
         * @brief Absolute time when pyro will turn off
         * 
         */
        uint32_t offTimeDeadline;

};