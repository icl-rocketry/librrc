#pragma once

/**
 * @file nrcremoteloadcell.h
 * @author Andrei Paduraru
 * @brief Class to read an analog load cell.
 * @version 0.2
 * @date 2024-08-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdint.h>

#include <librrc/Remote/nrcremotesensorbase.h>
#include <librnp/rnp_networkmanager.h>

#include <librrc/Helpers/nvsstore.h>
#include <librrc/Packets/loadcellcalibrationpacket.h>

class NRCRemoteLoadcell : public NRCRemoteSensorBase<NRCRemoteLoadcell>
{
public:

    NRCRemoteLoadcell(RnpNetworkManager& netman,  uint8_t loadCellIndex, uint32_t zeroReading = 0, float grad = 1, float localgval = 9.81);

    float getWeight(){return _Weight;};
    float getMass(){return (float)(_Weight)/(float)_g;};

    void calibrate_impl(packetptr_t packetptr);
    
    void setup(){loadCalibration();};
    void update(int32_t ADCReading);

private: 
    uint8_t _LCindex;
    int32_t _const;
    float _gradient;
    float _g;
    float _Weight;

    void loadCalibration();
};