#pragma once

/**
 * @file FlowSensor.h
 * @author Soham More
 * @brief Class to read an analog flow sensor and convert it to a mass flow rate
 * @version 0.1
 * @date 2023-01-23
 */


#include <librrc/Remote/nrcremotesensorbase.h>
#include <librnp/rnp_networkmanager.h>
#include <libriccore/platform/esp32/PCNT.h> 
#include <driver/pcnt.h>
#include <stdint.h>
#include <libriccore/platform/esp32/LEDC.h>
#include <libriccore/filtering/movingAvg.h>


class NRCRemoteFlowSensor : public NRCRemoteSensorBase<NRCRemoteFlowSensor>
{

public:
   NRCRemoteFlowSensor(RnpNetworkManager &netman, pcnt_unit_t unit, pcnt_channel_t channel, uint8_t _gpioSig  ,float k):NRCRemoteSensorBase(netman),         
                                                                    pcnt(unit, channel, _gpioSig),
                                                                    // ledc(_gpioLEDC),
                                                                    movingAvg(10),
                                                                     _k(k){};



    float getValue() { return _AvgflowRate; };
    

    void setup()
    { 
    
        pcnt.setup();
        // ledc.setup();
    };

    void update(){
        // ledc.update();
        _flowSensorCurrTime = esp_timer_get_time();
        if (_flowSensorCurrTime - _flowSensorPrevTime < 5E3)return;
        pcnt.update();
        //get current count 
        _flowSensorCurrCount = pcnt.getCount();
        //find difference
        // uint16_t countDiff = (uint16_t)abs((_flowSensorCurrCount - _flowSensorPrevCount));


        uint16_t countDiff; 
        if (_flowSensorCurrCount < _flowSensorPrevCount)
        {
            countDiff = (_flowSensorCurrCount + INT16_MAX) - _flowSensorPrevCount;
        }
        else 
        {
            countDiff = _flowSensorCurrCount - _flowSensorPrevCount;
        }
        
        //  Serial.println(countDiff);
        //convert to flow rate and update
        _flowRate = 1E6*_k*((float)countDiff/(float)(_flowSensorCurrTime - _flowSensorPrevTime))*0.5;
        // Serial.println(_flowRate);
        movingAvg.update(_flowRate); 
        // _AvgflowRate = movingAvg.getAvg();
        _AvgflowRate = movingAvg.getAvg();
        //update previous count
         _flowSensorPrevCount = _flowSensorCurrCount;
        //update previous time
         _flowSensorPrevTime = _flowSensorCurrTime; 
    };

private:
    PCNT pcnt;
    // LEDC ledc;
    MovingAvg movingAvg;
    float _flowRate;
    float _AvgflowRate; 
    float _k;

       
    int16_t _flowSensorCurrCount;
    int16_t _flowSensorPrevCount;
    int32_t _flowSensorCurrTime;
    int32_t _flowSensorPrevTime;
    
};

