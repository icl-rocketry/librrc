#pragma once

#include "rocketcomponent.h"

#include <memory>

#include <librrc/Helpers/rrclog.h>



class RocketActuator: public RocketComponent{
    public:
        RocketActuator(uint8_t id,LIBRRC::RRCLog::LogCallback_t logcb):
        RocketComponent(id,logcb)
        {};

        /**
         * @brief Default argument arming
         * 
         */
        virtual void arm();

        /**
         * @brief Arm actuator with specified argument, for example in the pyro actuator,
         * arg = 1 disregards continuity checking.
         * 
         * @param arg 
         */
        virtual void arm(int32_t arg) = 0;

        /**
         * @brief Disarm actuator
         * 
         */
        virtual void disarm() = 0;

        /**
         * @brief Call exectue on component with specified parameter
         * 
         * @param param 
         */
        virtual void execute(int32_t param) = 0;

        /**
         * @brief Get the current Value of acutator
         * 
         * @return int32_t 
         */
        virtual int32_t getValue();

        virtual ~RocketActuator() = 0;
    
    protected:
        int32_t currentValue;
        int32_t defaultArmArg = 0;

};