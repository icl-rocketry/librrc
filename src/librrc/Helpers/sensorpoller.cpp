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
        _networksensor -> updateState();
        sensorState = static_cast<const NetworkSensorState *>(_networksensor->getState());

        if (sensorState->lastNewStateUpdateTime > prevNewUpdateTime){
            newdata = true;
            sensorvalue = sensorState->sensorValue;
            prevNewUpdateTime = sensorState->lastNewStateUpdateTime;
        }

        if (sensorState->lastNewStateRequestTime >= sensorState->lastNewStateUpdateTime){
            if (sensorState->lastNewStateRequestTime >= networkTimeout){
                //throw some sort of error (Kiran)
            }
        }
        
        last_time = millis();
    }
}

float SensorPoller::getVal()
{
    return sensorvalue;
}