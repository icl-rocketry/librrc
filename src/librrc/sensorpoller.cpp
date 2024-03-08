#include "sensorpoller.h"
#include <Arduino.h>

void SensorPoller::setup()
{
    m_lastTime = millis();
}

void SensorPoller::update()
{
    if (millis() - m_lastTime > m_pollInterval)
    {
        m_networkSensor -> updateState();
        m_sensorState = static_cast<const NetworkSensorState*>(m_networkSensor->getState());

        if (m_sensorState->lastNewStateUpdateTime > m_prevNewUpdateTime){
            newData = true;
            m_sensorValue = m_sensorState->sensorValue;
            m_prevNewUpdateTime = m_sensorState->lastNewStateUpdateTime;
        }

        if (m_sensorState->lastNewStateRequestTime >= m_sensorState->lastNewStateUpdateTime){
            if (m_sensorState->lastNewStateRequestTime >= m_networkTimeout){
                throw std::runtime_error("Sensor with ID: " + std::to_string(m_networkSensor->getID()) + " not responding to poll request");
            }
        }

        m_lastTime = millis();
    }
}

float SensorPoller::getVal()
{
    return m_sensorValue;
}