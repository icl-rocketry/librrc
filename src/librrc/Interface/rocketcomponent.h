#pragma once

#include <iostream>
#include <unistd.h>
#include <string>
#include <optional>

#include <librrc/Helpers/bitwiseflagmanager.h>
#include <librrc/Helpers/rrclog.h>
#include <librrc/componentstatusflags.h>
#include <librrc/componentstatusflags.h>

#include "rocketcomponenttype.h"


class RocketComponent{
    public:
        //type aliases
        using RocketComponentState = LIBRRC::BitwiseFlagManager<LIBRRC::COMPONENT_STATUS_FLAGS>;

    public:
        RocketComponent(uint8_t id,LIBRRC::RRCLog::LogCallback_t logcb):
        _id(id),
        _logcb(logcb)
        {};
        
        /**
         * @brief Returns a const reference to the component state for external objects to 
         * get the last received state of the component.
         * 
         * @return const RocketComponentState& 
         */
        const RocketComponentState& getState() {return this->getStateMutable();};
        uint32_t getLastStateUpdateTime() {return lastStateUpdateTime;};
        uint32_t getLastStateRequestTime() { return lastStateRequestTime;};



        virtual void updateState() = 0;
       
        /**
         * @brief 
         * 
         * @param timeout 
         * @param stateExpiry 
         * @param handler 
         * @return true 
         * @return false 
         */
        virtual bool flightCheck(uint32_t timeout,uint32_t stateExpiry,std::string handler);
        virtual ~RocketComponent() = 0;

        uint8_t getID(){return _id;};

    protected:
        const uint8_t _id;

        /**
         * @brief Returns a non const reference to the component state allowing parent class
         * to update state of component
         * 
         * @return RocketComponentState* 
         */
        // virtual RocketComponentState* p_getState() = 0;

        virtual RocketComponentState& getStateMutable() = 0;
        std::optional<LIBRRC::component_status_flags_t> previousStatus = std::nullopt; //intialized to empty
        
        uint32_t lastStateUpdateTime = 0;
        uint32_t lastStateRequestTime = 0;


        LIBRRC::RRCLog::LogCallback_t _logcb;
            

};

