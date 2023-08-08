#include "nrcremoteservo.h"
#include "Helpers/rangemap.h"

#include "esp32-hal-ledc.h"

#include <Arduino.h>
#include <Preferences.h>

void NRCRemoteServo::setup(){

    ledcSetup(_channel,freq,timer_width);
    ledcAttachPin(_gpio,_channel);
    auto NVSName = "Srvo" + _channel;
    pref.begin(NVSName, true);
    _default_angle = (uint16_t) pref.getUInt("home");
    pref.end();
    
    reset(); // send servo to default position
    
}

void NRCRemoteServo::execute_impl(packetptr_t packetptr)
{

    SimpleCommandPacket execute_command(*packetptr);

    goto_Angle(execute_command.arg);
}

void NRCRemoteServo::calibrate_impl(packetptr_t packetptr){
    
    // ServoCalibrationPacket calibrate_comm(*packetptr);

    // setHome(calibrate_comm.home_angl);

    // setAngleLims(calibrate_comm.min_angl, calibrate_comm.max_angl);
}

void NRCRemoteServo::goto_Angle(uint16_t angle)
{
    /*Check if angle value is outside of angle limits. Would also have added checking for the min_angle and max_angle but 
    rangemap function already has checking for that so there's no point. */

    if (angle > _angl_lim_max)
    {
        _value = _angl_lim_max;
    }
    else if (angle < _angl_lim_min)
    {
        _value = _angl_lim_min;
    }
    else
    {
        _value = angle; // update new position of actuator
    }

    ledcWrite(_channel, angletocounts((uint16_t)_value));
}

uint16_t NRCRemoteServo::angletocounts(uint16_t angle)
{
    return LIBRRC::rangemap<uint16_t>(angle,_min_angle,_max_angle,_min_counts,_max_counts); 
}

void NRCRemoteServo::reset()
{
    goto_Angle(_default_angle);
}