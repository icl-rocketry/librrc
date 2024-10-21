#pragma once

#include "rocketcomponent.h"
#include "rocketcomponenttype.h"
#include "rocketsensor.h"
#include "networkcomponentstate.h"

#include <librnp/rnp_networkmanager.h>


#include <librrc/Helpers/rrclog.h>


class NetworkSensor:public RocketSensor{

    public:
        NetworkSensor(uint8_t id,uint8_t address,uint8_t sourceService,uint8_t destinationService,RnpNetworkManager& networkmanager,LIBRRC::RRCLog::LogCallback_t logcb):
        RocketSensor(id,logcb),
        _address(address),
        _sourceService(sourceService),
        _destinationService(destinationService),
        _networkmanager(networkmanager)
        {
            // logcb("Sensor:" + std::to_string(id) + " constructed" + std::to_string(address));
        };

        void updateState() override;

        /**
         * @brief Network callback function used to process packets recevied by the network manager. This will usually be managed by
         *        a managing object.
         * 
         * @param packetptr 
         */
        virtual void networkCallback(packetptr_t packetptr);

        ~NetworkSensor();

    protected:
        const uint8_t _address;
        const uint8_t _sourceService;
        const uint8_t _destinationService; 

        RnpNetworkManager& _networkmanager;
        NetworkComponentState _state;

         /**
         * @brief Returns a pointer to the state of the component
         * 
         * @return ComponentState* 
         */
        RocketComponentState& getStateMutable() override {return _state;};
    
};