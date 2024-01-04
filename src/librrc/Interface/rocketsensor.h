#pragma once

#include "rocketcomponent.h"

#include <librrc/Helpers/rrclog.h>

#include <memory>


class RocketSensor:public RocketComponent{
    public:
        RocketSensor(uint8_t id,LIBRRC::RRCLog::LogCallback_t logcb):
        RocketComponent(id,logcb)
        {};
        
        /**
         * @brief Get sensor Value
         * 
         * @return float 
         */
        virtual float getValue();

        virtual ~RocketSensor() = 0;

    protected:
        float sensorValue;

};

