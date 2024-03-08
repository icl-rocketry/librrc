#pragma once
#include <Arduino.h>
#include "networksensor.h"



class SensorPoller
{
    public:
    SensorPoller(uint16_t pollInterval, NetworkSensor* networkSensor, const uint64_t networkTimeout):
        m_pollInterval(pollInterval),
        m_networkSensor(networkSensor),
        m_networkTimeout(networkTimeout)
    {};

    void setup();
    void update();
    float getVal();
    bool newData;
    
    private:

    const uint16_t m_pollInterval;
    NetworkSensor* m_networkSensor;

    float m_sensorValue;

    const NetworkSensorState* m_sensorState;

    uint64_t m_lastTime;
    uint64_t m_prevNewUpdateTime = 0;
    const uint64_t m_networkTimeout;
};