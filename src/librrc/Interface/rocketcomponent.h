#pragma once

#include <iostream>
#include <unistd.h>
#include <string>

#include <librrc/Helpers/bitwiseflagmanager.h>
#include <librrc/Helpers/rrclog.h>
#include <librrc/componentstatusflags.h>
#include <librrc/componentstatusflags.h>

#include "rocketcomponenttype.h"


struct RocketComponentState : public LIBRRC::BitwiseFlagManager<LIBRRC::COMPONENT_STATUS_FLAGS>
{

    uint32_t lastNewStateUpdateTime;
    uint32_t lastNewStateRequestTime;
    LIBRRC::component_status_flags_t previousStatus; // used to track changes in state

    /**
     * @brief Allows tracking of the state of remote components such as
     * networked components
     * 
     * @param remoteState 
     */
    void trackRemoteStatus(uint16_t remoteState)
    {
        _statusString = remoteState;
    }
};

class RocketComponent{
    public:
        RocketComponent(uint8_t id,RocketComponentTypes::TYPES componentType,LIBRRC::RRCLog::LogCallback_t logcb):
        _id(id),
        _componentType(componentType),
        _logcb(logcb)
        {};
        
        /**
         * @brief Returns a const pointer to the component state for external objects to 
         * get the last received state of the component.
         * 
         * @return const RocketComponentState* 
         */
        const RocketComponentState* getState(){return p_getState();};
        virtual void updateState() = 0;
        //TODO: change netRetryInterval to pollInterval so its more agnostic between networked vs non-networked
        virtual bool flightCheck(uint32_t netRetryInterval,uint32_t stateExpiry,std::string handler);
        virtual ~RocketComponent() = 0;

        uint8_t getID(){return _id;};
        RocketComponentTypes::TYPES getComponentType(){return _componentType;};

    protected:
        const uint8_t _id;
        const RocketComponentTypes::TYPES _componentType;

        /**
         * @brief Returns a non const pointer to the component state allowing parent class
         * to update state of component
         * 
         * @return RocketComponentState* 
         */
        virtual RocketComponentState* p_getState() = 0;


        LIBRRC::RRCLog::LogCallback_t _logcb;
            

};

