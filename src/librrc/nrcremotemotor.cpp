#include "nrcremotemotor.h"
#include "Helpers/rangemap.h"

#include "esp32-hal-ledc.h"

#include <Arduino.h>

void NRCRemoteMotor::setup(){

    ledcSetup(_channel1, freq, timer_width);
    ledcAttachPin(_gpio1, _channel1);

    ledcSetup(_channel2, freq, timer_width);
    ledcAttachPin(_gpio2, _channel2);

}

void NRCRemoteMotor::execute_impl(packetptr_t packetptr){

    SimpleCommandPacket execute_command(*packetptr);

    if(execute_command.arg < 0){    //reverse rotation
        ledcWrite(_channel1, 0);
        ledcWrite(_channel2, LIBRRC::rangemap<uint32_t>(static_cast<uint32_t>(execute_command.arg), 0, 4095, 0, 100));
    }
    if(execute_command.arg > 0){    //forward rotation
        ledcWrite(_channel1, LIBRRC::rangemap<uint32_t>(static_cast<uint32_t>(execute_command.arg), 0, 4095, 0, 100));
        ledcWrite(_channel2, 0);
    }
    if(execute_command.arg == 0){   //stop and freewheel
        ledcWrite(_channel1, 0);
        ledcWrite(_channel2, 0);
    }
    
}