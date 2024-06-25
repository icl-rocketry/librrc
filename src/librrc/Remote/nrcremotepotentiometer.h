#pragma once

/**
 * @file Potentiometer.h
 * @author Rassa. The Chosen One. You know how we do it you know how we get downnnnn.
 * @brief Class that reads potentiometer angle 
 * @version 0.1
 * @date 11/06/2024
**/

#include <librrc/Remote/nrcremotesensorbase.h>
#include <librnp/rnp_networkmanager.h>
#include <libriccore/platform/esp32/ADC.h>
#include <libriccore/filtering/movingAvg.h>
#include <stdint.h>
#include <libriccore/riccorelogging.h>

class nrcremotepotentiometer : public NRCRemoteSensorBase<nrcremotepotentiometer>
{

public: 
    nrcremotepotentiometer(RnpNetworkManager &netman, uint8_t _gpioSig) : NRCRemoteSensorBase(netman), adc(_gpioSig){};


    void setup(){
        adc.setup();
    };

    void update(){
        
        adc.update();
        adccount = adc.getADC();
        potentang = ((float) adccount)/4096.0 * 360.0;
        updateSensorValue(potentang);
    }

    float getValue(){return potentang;}

private:
    ADC adc;
    int16_t adccount;
    float potentang;

};