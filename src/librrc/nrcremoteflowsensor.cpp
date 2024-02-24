#include "nrcremoteflowsensor.h"
#include <librrc/nrcremotesensorbase.h>
#include <librnp/rnp_networkmanager.h>

NRCRemoteFlowSensor::NRCRemoteFlowSensor(RnpNetworkManager &netman,  uint8_t _gpioSig, float _k) :  NRCRemoteSensorBase(netman),
                                                                    _gpioSig(_gpioSig),
                                                                     _k(_k){};


void NRCRemoteFlowSensor::update() {
        _flowSensorCurrTime = esp_timer_get_time();
        //get current count 
        pcnt_get_counter_value(PCNT_UNIT_0, &_flowSensorCurrCount);
        //find difference
        uint16_t countDiff = (uint16_t)abs((_flowSensorCurrCount - _flowSensorPrevCount));
        // Serial.println(countDiff);
        //convert to flow rate and update
        _flowRate = 1E6*_k*((float)countDiff/(float)(_flowSensorCurrTime - _flowSensorPrevTime)); 
        MovingAvgSamples.push_back(_flowRate);
            if (MovingAvgSamples.size() > _numSamples)
            {
                MovingAvgSamples.pop_front();   
            }
            float MovingAverageSum = 0.0;
            for (float sample: MovingAvgSamples)
            {
                MovingAverageSum += sample;
            }
            _AvgflowRate = MovingAverageSum*sampleCoeff;           
        //update previous count
         _flowSensorPrevCount = _flowSensorCurrCount;
        //update previous time
         _flowSensorPrevTime = _flowSensorCurrTime; 
      
}