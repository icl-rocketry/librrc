#pragma once

#include <esp32-hal-gpio.h>

class ArduinoGpio
{
    public:

        enum class PINMODE:uint8_t {
            GPIO_OUTPUT = OUTPUT,
            GPIO_INPUT = INPUT
        };

        // enum class LEVEL:uint8_t{
        //     GPIO_LOW = LOW,
        //     GPIO_HIGH = HIGH
        // };

        ArduinoGpio(uint8_t pin):
        m_pin(pin)
        {};

        void pinMode(PINMODE pinmode)
        {

            ::pinMode(m_pin,static_cast<uint8_t>(pinmode));

        };

        void digitalWrite(uint8_t level)
        {
            ::digitalWrite(m_pin,level);
        };

        int digitalRead()
        {
            return ::digitalRead(m_pin);
        }

    private:
        const uint8_t m_pin;
};