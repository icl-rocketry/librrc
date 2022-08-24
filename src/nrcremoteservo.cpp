#include "nrchandlerservo.h"
#include "Helpers/rangemap.h"

#include "esp32-hal-ledc.h"

#include <Arduino.h>

void NRCHandlerServo::setup(){

    ledcSetup(_channel,50,16);
    ledcAttachPin(_gpio,_channel);
}

void NRCHandlerServo::execute_impl(packetptr_t packetptr)
{
    
    SimpleCommandPacket execute_command(*packetptr);

    ledcWrite(_channel,rangemap<uint16_t>(execute_command.arg,_min_angle,_max_angle,_min_counts,_max_counts));

}