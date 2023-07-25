#include "nrcremoteservo.h"
#include "Helpers/rangemap.h"

#include "esp32-hal-ledc.h"

#include <Arduino.h>

void NRCRemoteServo::setup(){

    ledcSetup(_channel,freq,timer_width);
    ledcAttachPin(_gpio,_channel);
    reset(); // send servo to default position
    
}

void NRCRemoteServo::execute_impl(packetptr_t packetptr)
{

    SimpleCommandPacket execute_command(*packetptr);

    /*Check if angle value is outside of angle limits. Would also have added checking for the min_angle and max_angle but 
    rangemap function already has checking for that so there's no point. */
    if (execute_command.arg > _angl_lim_max)
    {
        _value = _angl_lim_max;
    }
    else if (execute_command.arg < _angl_lim_min)
    {
        _value = _angl_lim_min;
    }
    else
    {
        _value = execute_command.arg; // update new position of actuator
    }

    ledcWrite(_channel, angletocounts((uint16_t)_value));
}

uint16_t NRCRemoteServo::angletocounts(uint16_t angle)
{
    return LIBRRC::rangemap<uint16_t>(angle,_min_angle,_max_angle,_min_counts,_max_counts);
    
}

void NRCRemoteServo::reset()
{
    ledcWrite(_channel,angletocounts(_default_angle));
}