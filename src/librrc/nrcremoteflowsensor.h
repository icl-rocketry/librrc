#pragma once

/**
 * @file FlowSensor.h
 * @author Soham More 
 * @brief Class to read an analog flow sensor and convert it to a mass flow rate
 * @version 0.1
 * @date 2023-01-23
 */


#include "Sensors/ADS131M06.h"
#include <stdint.h>

#include <librrc/nrcremotesensorbase.h>
#include <librnp/rnp_networkmanager.h>

#include <librrc/Helpers/nvsstore.h>
#include <librrc/packets/loadcellcalibrationpacket.h>

#include <driver/pcnt.h>

class NRCRemoteFlowSensor : public NRCRemoteSensorBase<NRCRemoteFlowSensor> {
 
    public: 

        NRCRemoteFlowSensor(uint8_t& gpioSig, RnpNetworkManager& netman, float k) : NRCRemoteSensorBase(netman),_gpioSig(gpioSig) _k(k){};

            uint16_t _flowSensorCurrCount = 0.0;
            uint16_t _flowSensorPrevCount = 0.0;
            float _flowSensorCurrTime = 0.0;
            float _flowSensorPrevTime = 0.0;
        

        float getValue(){ return _flowRate;};

        void update(){};

            
    private: 

        float _flowRate;
        uint16_t _flowSensorCurrCount;
        uint16_t _flowSensorPrevCount;
        float _flowSensorCurrTime;
        float _flowSensorPrevTime;

        float _minL = -10;
        float _maxL = 10;
        float _ThreshVal1 = 5;
        float _ThreshVal0 = -5;

        //Control and signal Pins
        uint8_t gpioSig = 4;
        uint8_t gpioCtrl;

        typedef struct{
            int unit;
            uint32_t status;    
        } pcnt_evt_t;
        
};
