#pragma once

/**
 * @file FlowSensor.h
 * @author Soham More
 * @brief Class to read an analog flow sensor and convert it to a mass flow rate
 * @version 0.1
 * @date 2023-01-23
 */


#include <librrc/nrcremotesensorbase.h>
#include <librnp/rnp_networkmanager.h>
#include <libriccore/platform/esp32/PCNT.h> 
#include <driver/pcnt.h>
#include <stdint.h>

class NRCRemoteFlowSensor : public NRCRemoteSensorBase<NRCRemoteFlowSensor>
{

public:
   NRCRemoteFlowSensor(RnpNetworkManager &netman, pcnt_unit_t unit, pcnt_channel_t channel, uint8_t _gpioSig , float k):NRCRemoteSensorBase(netman),         
                                                                    pcnt(unit, channel, _gpioSig),
                                                                     _k(k){};



    float getValue() { return _flowRate; };
    

    void setup()
    { 
    
        pcnt.setup();
    };

    void update(){
        _flowSensorCurrTime = esp_timer_get_time();
        pcnt.update();
        //get current count 
        _flowSensorCurrCount = pcnt.getCount();
        //find difference
        uint16_t countDiff = (uint16_t)abs((_flowSensorCurrCount - _flowSensorPrevCount));
        // Serial.println(countDiff);
        //convert to flow rate and update
        _flowRate = 1E6*_k*((float)countDiff/(float)(_flowSensorCurrTime - _flowSensorPrevTime));            
        //update previous count
         _flowSensorPrevCount = _flowSensorCurrCount;
        //update previous time
         _flowSensorPrevTime = _flowSensorCurrTime; 
    };

private:
    PCNT pcnt;
    float _flowRate;
    float _k;
  
        
    int16_t _flowSensorCurrCount;
    int16_t _flowSensorPrevCount;
    int32_t _flowSensorCurrTime;
    int32_t _flowSensorPrevTime;
    
};

