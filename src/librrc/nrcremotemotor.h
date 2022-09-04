#pragma once

#include "nrcremoteactuatorbase.h"

#include <rnp_networkmanager.h>
#include <rnp_packet.h>

class NRCRemoteMotor : public NRCRemoteActuatorBase<NRCRemoteMotor>{

public:

    NRCRemoteMotor(RnpNetworkManager &networkmanager,
                    uint8_t gpio1,
                    uint8_t gpio2,
                    uint8_t channel1,
                    uint8_t channel2
                    ):
    NRCRemoteActuatorBase(networkmanager),
        _gpio1(gpio1),
        _gpio2(gpio2),
        _channel1(channel1),
        _channel2(channel2)
        {};

    void setup();


protected:

    friend class NRCRemoteActuatorBase;

    const uint8_t _gpio1;
    const uint8_t _gpio2;
    const uint8_t _channel1;
    const uint8_t _channel2;

    void execute_impl(packetptr_t packetptr);

    static constexpr int timer_width = 12;
    static constexpr int freq = 10000;
    
    static constexpr int counts(int usec){
       return (int)(float(usec) / (float(1000000/freq)/float(std::pow(2,timer_width))));
    }
};