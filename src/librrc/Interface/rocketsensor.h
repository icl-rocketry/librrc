#pragma once

#include "rocketcomponent.h"

#include <librrc/Helpers/rrclog.h>

#include <memory>


struct RocketSensorState:public RocketComponentState{
    float sensorValue; // in si units
};
 //stoopid empty class ...
class RocketSensor:public RocketComponent{
    public:
        RocketSensor(uint8_t id,LIBRRC::RRCLog::LogCallback_t logcb):
        RocketComponent(id,logcb)
        {};

        virtual ~RocketSensor() = 0;

};

