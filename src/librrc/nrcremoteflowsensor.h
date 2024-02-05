#pragma once

/**
 * @file FlowSensor.h
 * @author Soham More
 * @brief Class to read an analog flow sensor and convert it to a mass flow rate
 * @version 0.1
 * @date 2023-01-23
 */

#include <stdint.h>

#include <librrc/nrcremotesensorbase.h>
#include <librnp/rnp_networkmanager.h>

#include <librrc/Helpers/nvsstore.h>
#include <librrc/packets/loadcellcalibrationpacket.h>

#include <driver/pcnt.h>

class NRCRemoteFlowSensor : public NRCRemoteSensorBase<NRCRemoteFlowSensor>
{

public:
    NRCRemoteFlowSensor(RnpNetworkManager &netman, uint8_t _gpioSig, float _k);

    int16_t _flowSensorCurrCount = 0;
    int16_t _flowSensorPrevCount = 0;
    int32_t _flowSensorCurrTime = 0;
    int32_t _flowSensorPrevTime = 0;

    float getValue() { return _flowRate; };

    void update();

    void setup()
    {
        /* Prepare configuration for the PCNT unit */
        pcnt_config_t pcnt_config = {
            // Set PCNT input signal and control GPIOs
            .pulse_gpio_num = _gpioSig,
            .ctrl_gpio_num = PCNT_PIN_NOT_USED,
            .pos_mode = PCNT_CHANNEL_EDGE_ACTION_HOLD, // Count up on the positive edge
            .neg_mode = PCNT_CHANNEL_EDGE_ACTION_INCREASE, // Keep the counter value on the negative edge
            .counter_h_lim = 32767,
            .counter_l_lim = 0,
            .unit = PCNT_UNIT_0,
            .channel = PCNT_CHANNEL_0

            // What to do on the positive / negative edge of pulse input?

            // What to do when control input is low or high?

            // Set the maximum and minimum limit values to watch

        };

        /* Initialize PCNT unit */
        pcnt_unit_config(&pcnt_config);

        pcnt_counter_pause(PCNT_UNIT_0);
        pcnt_counter_clear(PCNT_UNIT_0);
        pcnt_counter_resume(PCNT_UNIT_0);
    }

private:
    float _flowRate;
    // uint16_t _flowSensorCurrCount;
    // uint16_t _flowSensorPrevCount;
    // float _flowSensorCurrTime;
    // float _flowSensorPrevTime;

    float _minL = -10;
    float _maxL = 10;
    float _ThreshVal1 = 5;
    float _ThreshVal0 = -5;
    
    uint8_t _gpioSig;
    float _k;

    typedef struct
    {
        int unit;
        uint32_t status;
    } pcnt_evt_t;

    
};
