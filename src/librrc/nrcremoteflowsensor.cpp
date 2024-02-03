#include "nrcremoteflowsensor.h"

#include <librrc/nrcremotesensorbase.h>
#include <librnp/rnp_networkmanager.h>

NRCRemoteFlowSensor::NRCRemoteFlowSensor(RnpNetworkManager &netman,  uint8_t flowSensorIndex, uint8_t& PCNT_INPUT_SIG_IO,  ADS131M06& ADS, uint8_t ADCchannel) : NRCRemoteSensorBase(netman),
                                                                                                                           _FSindex(flowSensorIndex),
                                                                                                                           _ADS(ADS),
                                                                                                                            _adc_channel(ADCchannel),


void NRCRemoteSensorBase::update() {
        float _flowSensorCurrTime = esp_timer_get_time();
        //get current count 
        _flowSensorCurrCount= pcnt_get_counter_value(PCNT_UNIT_0, &_flowSensorCurrCount);
        //find difference
        uint16_t countDiff = _flowSensorCurrCount - _flowSensorPrevCount;
        //convert to flow rate and update
        float _flowRate = (countDiff/(_flowSensorCurrTime - _flowSensorPrevTime))/k;            
        //update previous count
        float _flowSensorPrevCount = _flowSensorCurrCount;
        //update previous time
        float _flowSensorPrevTime = _flowSensorCurrTime; 
      
}