#pragma once
/**
 * @file remoteSensoradapter.h
 * @author Kiran de Silva (kd619@ic.ac.uk)
 * @brief Provides an adapter to allow NRCRemoteSensors to be used locally without the overhead
 * of serializing and de-serializing through the loopback interface.
 * @version 0.1
 * @date 2024-01-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <Arduino.h>

#include <librrc/Interface/rocketcomponent.h>
#include <librrc/Interface/rocketSensor.h>
#include <librrc/Helpers/rrclog.h>

template<typename REMOTE_T>
class RemoteSensorAdapter : public RocketSensor
{
    public:
        RemoteSensorAdapter(uint8_t id,REMOTE_T& remoteSensor,LIBRRC::RRCLog::LogCallback_t logcb):
        RocketSensor(id,logcb),
        m_remoteSensor(remoteSensor)
        {};

        void updateState() override 
        {
            lastStateRequestTime = millis();
            m_remoteSensor.localUpdateState_impl();
            lastStateUpdateTime = millis();
        };

        float getValue() override 
        {
            //cast back to float as remote sensors store sesnor values as int32_t
            return *reinterpret_cast<float*>(&m_remoteSensor._value);
        };

        ~RemoteSensorAdapter(){};

    protected:
        REMOTE_T& m_remoteSensor;

        RocketComponentState& getStateMutable() override {return m_remoteSensor._state;};
    
    private:
        /**
         * @brief sensor Value is moved to private to prevent unintentional access as this value is now unused.
         * 
         */
        using RocketSensor::sensorValue;


    
};