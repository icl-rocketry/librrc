#pragma once

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

        void arm(int32_t arg) override { m_remoteActuator.arm(arg); };

        void disarm() override { m_remoteActuator.disarm(); };

        void execute(int32_t param) override
        {
            if (getState().flagSet(LIBRRC::COMPONENT_STATUS_FLAGS::NOMINAL))
            {
                m_remoteActuator.execute(param);
            }
        };

        void updateState() override 
        {
            lastStateRequestTime = millis();
            lastStateUpdateTime = millis();
        };

        int32_t getValue() override 
        {
            return m_remoteActuator._value;
        };

        ~RemoteActuatorAdapter(){};

    protected:
        REMOTE_T& m_remoteActuator;

        RocketComponentState& getState() override {return m_remoteActuator._state;};
    
    private:
        /**
         * @brief current Value is moved to private to prevent unintentional access as this value is now unused.
         * 
         */
        using RocketActuator::currentValue;


    
};