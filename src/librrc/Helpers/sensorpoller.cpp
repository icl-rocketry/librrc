#include "sensorpoller.h"
#include <Arduino.h>

void SensorPoller::setup()
{
    last_time = millis();
}

void SensorPoller::update()
{
    if (millis() - last_time > _poll_interval)
    {
        _networksensor->updateState();
        // sensorState = static_cast<const NetworkSensorState *>(_networksensor->getState());

        // if (sensorState->lastNewStateUpdateTime > prevNewUpdateTime){
        if (_networksensor->getLastStateUpdateTime() > prevNewUpdateTime)
        {
            newdata = true;
            sensorvalue = _networksensor->getValue();
            prevNewUpdateTime = _networksensor->getLastStateUpdateTime();
        }

        if (_networksensor->getLastStateRequestTime() >= _networksensor->getLastStateUpdateTime())
        {
            if (_networksensor->getLastStateRequestTime() >= networkTimeout)
            {
                // throw some sort of error (Kiran)
            }
        }

        last_time = millis();
    }
}

float SensorPoller::getVal()
{
    return sensorvalue;
}