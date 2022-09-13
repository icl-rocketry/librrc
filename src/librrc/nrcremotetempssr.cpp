#include "nrcremotetempssr.h"

#include <Arduino.h>



void NRCRemoteTempSSR::setup(){

    pinMode(_gpio,OUTPUT);
    digitalWrite(_gpio,LOW);
    
}

void NRCRemoteTempSSR::update(float sensor_temperature){

    if (!_state.flagSet(COMPONENT_STATUS_FLAGS::NOMINAL)){
        digitalWrite(_gpio,LOW);
        return;
    }
    
    if(sensor_temperature< _setpoint){
        digitalWrite(_gpio,HIGH);
    }else{
         digitalWrite(_gpio,LOW);
    }


}

void NRCRemoteTempSSR::execute_impl(packetptr_t packetptr)
{
    SimpleCommandPacket execute_command(*packetptr);
    _setpoint = float(execute_command.arg); 
}

