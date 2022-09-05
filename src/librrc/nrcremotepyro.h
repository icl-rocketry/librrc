#pragma once

#include "nrcremoteactuatorbase.h"

#include <rnp_networkmanager.h>
#include <rnp_packet.h>

class NRCRemotePyro : public NRCRemoteActuatorBase<NRCRemotePyro>
{

public:
    NRCRemotePyro(uint8_t gpio,
                    uint8_t channel,
                    RnpNetworkManager &networkmanager,
                    uint16_t default_angle = 0,
                    uint16_t min_angle = 0,
                    uint16_t max_angle = 180,
                    uint16_t min_counts = counts(500),
                    uint16_t max_counts = counts(2500)
                    ): 
    NRCRemoteActuatorBase(networkmanager),
        _gpio(gpio),
        _channel(channel),
        _default_angle(default_angle),
        _min_angle(min_angle),
        _max_angle(max_angle),
        _min_counts(min_counts),
        _max_counts(max_counts)
        {};

    void setup();
   
protected:
    friend class NRCRemoteActuatorBase;

    const uint8_t _gpio;



    void execute_impl(packetptr_t packetptr);


};