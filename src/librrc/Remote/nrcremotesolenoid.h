#pragma once

#include "nrcremoteactuatorbase.h"

#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>


#include <math.h>
#include <Preferences.h>

class NRCRemoteSolenoid : public NRCRemoteActuatorBase<NRCRemoteSolenoid>
{

public:
    NRCRemoteSolenoid(uint8_t togglePin, RnpNetworkManager &networkmanager): 
    NRCRemoteActuatorBase(networkmanager),
    _togglePin(togglePin)
    {};

    void setup();
   
protected:

    friend class NRCRemoteActuatorBase;
    friend class NRCRemoteBase;

    const uint8_t _togglePin;
    /**
     * @brief 0 - NC
     *        1 - NO
     * 
     */
    bool normalState = 0; // 0 = normally closed, 1 = normally open

    void execute_base(int32_t arg);

    void loadCalibration();
    void calibrate_impl(packetptr_t packetptr);

};