/**
 * @file localpwm.h
 * @author Kiran de Silva (kd619@ic.ac.uk)
 * @brief Wrapper around esp32 ledc api for a unified pwm interface
 * @version 0.1
 * @date 2024-08-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <esp32-hal-ledc.h>
#include <cstdint>
#include <math.h>

class LocalPWM
{
public:
    LocalPWM(uint8_t pin, uint32_t freq = 50, uint8_t res = 14) : m_pin(pin),
                                                                                   m_freq(freq),
                                                                                   m_res(res),
                                                                                   m_initialized(false)
    {
        setupLEDC();
    };

    /**
     * @brief Change duty of pwm channel specifing duty within range [0,2^m_res]. MaxDuty = 2^m_res
     *      Hint: use LIBRRC::RangeMap to remap your required range to the range [0, MaxDuty]
     *
     * @param duty
     */
    void writeDuty(uint32_t duty)
    {
        if (!m_initialized)
        {
            return; //? maybe log not initlaized??
        }

        ledcWrite(m_pin, duty); // this auto checks for if duty is out of range but maybe we should check in this wrapper instead?
    };

    /**
     * @brief Change duty cycle of pwm channel, specifiyng pulse width. 
     *        Hint: Keep track of what the underlying base frequency is...
     *
     * @param usec
     */
    void writeWidth(uint32_t usec)
    {
        writeDuty(toCounts(usec));
    };

    /**
     * @brief Get the Returns the max duty of the pwm channel
     *
     * @return uint32_t
     */
    uint32_t getMaxDuty()
    {
        return std::pow(2, m_res);
    };

    uint8_t getResolution()
    {
        return m_res;
    }

    uint32_t getFrequency()
    {
        return m_freq;
    };

    void setPWMParam(uint32_t freq, uint8_t res)
    {
        //detach channel
        ledcDetach(m_pin);
        m_freq = freq;
        m_res = res;
        // re-init channel
        setupLEDC();
    }

public:
    const uint8_t m_pin;
    uint32_t m_freq;
    uint8_t m_res;

    bool m_initialized;

    bool setupLEDC()
    {
        if (!ledcAttach(m_pin, m_freq, m_res))
        {
            m_initialized = false; //? logging?
            return false;
        }
        m_initialized = true;
        return true;
    };

    /**
     * @brief ESP32 duty counts = [0, 2**res], scale pulse width to this range
     *
     * @param usec
     * @return uint32_t
     */
    uint32_t toCounts(uint32_t usec)
    {

        uint32_t maxDuty = std::pow(2, m_res);
        float fractionOfPeriod = (static_cast<float>(usec) * static_cast<float>(m_freq)) / static_cast<float>(1e6);
        float duty = fractionOfPeriod * static_cast<float>(maxDuty);

        return static_cast<uint32_t>(duty);
    };
};