#pragma once

#include "nrcremoteactuatorbase.h"

#include <rnp_networkmanager.h>
#include <rnp_packet.h>

class NRCRemoteTempSSR : public NRCRemoteActuatorBase<NRCRemoteTempSSR>
{

public:
    NRCRemoteTempSSR(uint8_t gpio,RnpNetworkManager &networkmanager): 
        NRCRemoteActuatorBase(networkmanager),
        _gpio(gpio),
        _setpoint(0)
        {};

    void setup();

    void update(float sensor_temperature);
   
protected:
    friend class NRCRemoteActuatorBase;

    const uint8_t _gpio;
    float _setpoint;


    void execute_impl(packetptr_t packetptr);


    
};