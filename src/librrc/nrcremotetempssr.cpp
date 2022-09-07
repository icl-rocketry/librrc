#include "nrcremotetempssr.h"

#include <Arduino.h>



void NRCRemoteTempSSR::setup(){

    pinMode(_gpio,OUTPUT);
    
}

void NRCRemoteTempSSR::update(float sensor_temperature){

    _sensor_temperature = sensor_temperature;    
    
    execute_impl(packetptr_t packetptr);

}

void NRCRemoteTempSSR::execute_impl(packetptr_t packetptr)
{
    
    SimpleCommandPacket execute_command(*packetptr);

    if(_sensor_temperature < static_cast<float>(execute_command.arg)){
        digitalWrite(_gpio,HIGH);
    }

    if(_sensor_temperature > static_cast<float>(execute_command.arg)){
        digitalWrite(_gpio,LOW);
    }

    if(execute_command.arg == 0){
        digitalWrite(_gpio,LOW);
    }

}
