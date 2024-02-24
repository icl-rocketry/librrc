#pragma once

#include <stdint.h>

#include <librrc/nrcremotesensorbase.h>
#include <librnp/rnp_networkmanager.h>

#include <librrc/Helpers/nvsstore.h>

#include <driver/pcnt.h>

class NRCRemoteFlowSensor : public NRCRemoteSensorBase<NRCRemoteFlowSensor>
{

    private:
        float _flowRate; 
        float _AvgflowRate;
        uint8_t _gpioSig;
        float _k;

        typedef struct
        {
            int unit;
            uint32_t status;
        } pcnt_evt_t;

        std::deque<float> MovingAvgSamples;
        

    public:
        NRCRemoteFlowSensor(RnpNetworkManager &netman, uint8_t _gpioSig, float _k);

        int16_t _flowSensorCurrCount = 0;
        int16_t _flowSensorPrevCount = 0;
        int32_t _flowSensorCurrTime = 0;
        int32_t _flowSensorPrevTime = 0;

       
        float getValue() { return _AvgflowRate; };
        void update();

        void setup()
        {
            /* Prepare configuration for the PCNT unit */
            pcnt_config_t pcnt_config = {
                // Set PCNT input signal and control GPIOs
                .pulse_gpio_num = _gpioSig,
                .ctrl_gpio_num = PCNT_PIN_NOT_USED,
                .pos_mode = PCNT_CHANNEL_EDGE_ACTION_HOLD, 
                .neg_mode = PCNT_CHANNEL_EDGE_ACTION_INCREASE, 
                .counter_h_lim = 32767,
                .counter_l_lim = 0,
                .unit = PCNT_UNIT_0,
                .channel = PCNT_CHANNEL_0
            };

            /* Initialize PCNT unit */
            pcnt_unit_config(&pcnt_config);

            pcnt_counter_pause(PCNT_UNIT_0);
            pcnt_counter_clear(PCNT_UNIT_0);
            pcnt_counter_resume(PCNT_UNIT_0);
        }
        
        int _numSamples = 10;
        float sampleCoeff = 0.1;
};
