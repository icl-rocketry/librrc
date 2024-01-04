#pragma once

#include "rocketcomponent.h"

#include <memory>

// 
#include <librrc/Helpers/rrclog.h>



class RocketActuator: public RocketComponent{
    public:
        RocketActuator(uint8_t id,LIBRRC::RRCLog::LogCallback_t logcb):
        RocketComponent(id,logcb)
        {};

        virtual void arm() = 0;

        virtual void execute(int32_t param) = 0;

        virtual int32_t getValue() { return currentValue;};

        virtual ~RocketActuator() = 0;
    
    protected:
        int32_t currentValue;

};