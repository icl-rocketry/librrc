#include "rocketcomponent.h"
#include <librrc/componentstatusflags.h>

#include <string>
#include <Arduino.h>

RocketComponent::~RocketComponent(){};

//Returns true if there is an error !!
bool RocketComponent::flightCheck(uint32_t timeout,uint32_t stateExpiry,std::string handler){
    const RocketComponentState& currentState = this->getState();
    const uint8_t cid = this->getID();
    
    if (lastStateRequestTime > lastStateUpdateTime) //component hasnt sent new update
    {
        if (millis() - lastStateRequestTime > timeout) //component timeout
        { //maybe packet got lost on the network? might indicate something more serious however
            this->_logcb(handler + " Component: " + std::to_string(cid) + " timeout!");
            //update state of component to no response error
            // try requesting an update again
            this->updateState(); 
            return 1;
        }
    } 
    else if (millis()-lastStateRequestTime > stateExpiry)
    {
        //current state has expired, request new state update
        this->updateState();
        // this->_logcb(handler + " Component: " + std::to_string(cid) + "Update");
    }


    //check if the component state has changed or initialzied
    if (currentState.getStatus() != previousStatus || !previousStatus)
    {
        //update previous state
        previousStatus = currentState.getStatus();

        if (!currentState.flagSet(LIBRRC::COMPONENT_STATUS_FLAGS::NOMINAL))
        {
            //log the changes
            this->_logcb(handler + " Component: " + std::to_string(cid) + " error: " + std::to_string(currentState.getStatus()));
            
        }else{
            this->_logcb(handler + " Component: " + std::to_string(cid) + "Nominal");
        }
        
    }

    if (!currentState.flagSet(LIBRRC::COMPONENT_STATUS_FLAGS::NOMINAL))
    {
        return 1;
    }

    return 0;
}

