#pragma once

// #include <Arduino.h>
#include <esp32-hal-gpio.h>

class ArduinoGpio
{
    public:

        enum class PINMODE:uint8_t {
            GPIO_OUTPUT = OUTPUT,
            GPIO_INPUT = INPUT
        };

        enum class LEVEL:uint8_t{
            GPIO_LOW = LOW,
            GPIO_HIGH = HIGH
        };

        void pinMode(PINMODE pinmode)
        {

            pinMode(pin,static_cast<uint8_t>(pinmode));

        };

        void digitalWrite(LEVEL level)
        {
            digitalWrite(pin,static_cast<uint8_t>(level));
        };

        int digitalRead()
        {
            return digitalRead(pin);
        }

    private:
        const uint8_t pin;
};