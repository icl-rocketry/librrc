#include "nrcremoteptap.h"
#include <Preferences.h>

uint32_t NRCRemotePTap::getValue()
{
    return (_ADS.readAdjusted(_adc_channel)/_resistance - _c)/_grad;
}

void NRCRemotePTap::calibrate_impl(packetptr_t packetptr){
    
    // PTapCalibrationPacket calibrate_comm(*packetptr);

    // calibrate(calibrate_comm.c, calibrate_comm.grad, calibrate_comm.resistance);
}

void NRCRemotePTap::calibrate(float c, float grad, float resistance){
    
    _c = c;
    _grad = grad;
    _resistance = resistance;

    Preferences pref;

    auto NVSName = "PTap" + _adc_channel;

    pref.begin(NVSName, false);
    pref.putFloat("c", _c);
    pref.putFloat("_grad", _grad);
    pref.putFloat("resistance",_resistance);
    pref.end();
}