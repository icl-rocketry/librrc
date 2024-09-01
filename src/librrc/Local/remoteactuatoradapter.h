#pragma once
/**
 * @file remoteactuatoradapter.h
 * @author Kiran de Silva (kd619@ic.ac.uk)
 * @brief Provides an adapter to allow NRCRemoteActuators to be used locally without the overhead
 * of serializing and de-serializing through the loopback interface.
 * @version 0.1
 * @date 2024-01-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <Arduino.h>

#include <librrc/Interface/rocketcomponent.h>
#include <librrc/Interface/rocketactuator.h>
#include <librrc/Helpers/rrclog.h>

template<typename REMOTE_T>
class RemoteActuatorAdapter : public RocketActuator
{
    public:
        RemoteActuatorAdapter(uint8_t id,REMOTE_T& remoteActuator,LIBRRC::RRCLog::LogCallback_t logcb):
        RocketActuator(id,logcb),
        m_remoteActuator(remoteActuator)
        {};

        void arm(int32_t arg) override { m_remoteActuator.localArm_impl(arg); };

        void disarm() override { m_remoteActuator.localDisarm_impl(); };

        void execute(int32_t param) override
        {
            if (getState().flagSet(LIBRRC::COMPONENT_STATUS_FLAGS::NOMINAL))
            {
                m_remoteActuator.localExecute_impl(param);
            }
        };

        void updateState() override 
        {
            lastStateRequestTime = millis();
            m_remoteActuator.localUpdateState_impl();
            lastStateUpdateTime = millis();
        };

        int32_t getValue() override 
        {
            return m_remoteActuator._value;
        };

        ~RemoteActuatorAdapter(){};

    protected:
        REMOTE_T& m_remoteActuator;

        RocketComponentState& getStateMutable() override {return m_remoteActuator._state;};
    
    private:
        /**
         * @brief current Value is moved to private to prevent unintentional access as this value is now unused.
         * 
         */
        using RocketActuator::currentValue;


    
};