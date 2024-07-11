#pragma once

#include "nrcremoteactuatorbase.h"

#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>


#include <math.h>
#include <Preferences.h>

class NRCRemoteSolenoid : public NRCRemoteActuatorBase<NRCRemoteSolenoid>
{

public:
    NRCRemoteSolenoid(uint8_t togglePin, uint8_t contPin, RnpNetworkManager &networkmanager): 
    NRCRemoteActuatorBase(networkmanager),
    _togglePin(togglePin),
    _contPin(contPin)
    {};

    void setup();
    void setHomeState(uint16_t state) {
        if (state == 0) {
            home_state = false;
        } else if (state == 1) {
            home_state = true;
        }
    }

   
protected:

    friend class NRCRemoteActuatorBase;
    friend class NRCRemoteBase;

    const uint8_t _togglePin;
    const uint8_t _contPin;
    bool m_contCheckOverride = false;
    bool home_state; // false = nominally closed, true = nominally open
    void execute(int32_t arg);
    void execute_impl(packetptr_t packetptr);
    void arm(int32_t arg);
    void arm_impl(packetptr_t packetptr);
    void loadCalibration();
    void calibrate_impl(packetptr_t packetptr);
    void updateContinuity();
};