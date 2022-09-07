#pragma once

#include "nrcremoteactuatorbase.h"

#include <rnp_networkmanager.h>
#include <rnp_packet.h>

class NRCRemoteTempSSR : public NRCRemoteActuatorBase<NRCRemoteTempSSR>
{

public:
    NRCRemoteSSR(RnpNetworkManager &networkmanager,
                    uint8_t gpio
                    ): 
        NRCRemoteActuatorBase(networkmanager),
        _gpio(gpio)
        {};

    void setup();

    void update(float sensor_temperature);
   
protected:
    friend class NRCRemoteActuatorBase;

    const uint8_t _gpio;
    float _sensor_temperature;

    void execute_impl(packetptr_t packetptr);
};