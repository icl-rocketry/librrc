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
 
        last_time = millis();
    }

    if (_networksensor->getLastStateUpdateTime() > prevNewUpdateTime)
    {
        newdata = true;
        sensorvalue = _networksensor->getValue();
        prevNewUpdateTime = _networksensor->getLastStateUpdateTime();
    }

    if (_networksensor->getLastStateRequestTime() >= _networksensor->getLastStateUpdateTime())
    {
        if (millis() - _networksensor->getLastStateUpdateTime() >= networkTimeout)
        {
            throw std::runtime_error("Sensor with ID: " + std::to_string(_networksensor->getID()) + " not responding to poll request");
        }
    }


}

float SensorPoller::getVal()
{
    return sensorvalue;
}