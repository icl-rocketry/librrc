#include "nrcremoteservo.h"
#include "Helpers/rangemap.h"

#include "esp32-hal-ledc.h"

#include <Arduino.h>

void NRCRemoteServo::setup(){

    ledcSetup(_channel,50,16);
    ledcAttachPin(_gpio,_channel);
}

void NRCRemoteServo::execute_impl(packetptr_t packetptr)
{
    
    SimpleCommandPacket execute_command(*packetptr);
    _value = execute_command.arg; // update new position of actuator

    ledcWrite(_channel,LIBRRC::rangemap<uint16_t>(execute_command.arg,_min_angle,_max_angle,_min_counts,_max_counts));

}

void NRCRemoteServo::reset()
{
    ledcWrite(_channel,_min_angle);
}