#include "nrcremotetempssr.h"

#include <librrc/Interface/rocketsensor.h>
#include <librrc/Interface/networksensor.h>
#include <librrc/componentstatusflags.h>

#include <Arduino.h>

#include <algorithm>

void NRCRemoteTempSSR::setup(){

    pinMode(_gpio,OUTPUT);
    digitalWrite(_gpio,LOW);
    
}

void NRCRemoteTempSSR::update()
{

    if (_netTemp1 && !_netTemp2)
    {
        // abuse flight check to handle the updates?? lmao
        if (!_netTemp1->flightCheck(1000, 250, "TempSSR"))
        {
            float temp1 = _netTemp1->getValue();
            update(temp1);
            return;
        }
    }
    else if (_netTemp1 && _netTemp2)
    {

        bool error_netTemp1 = _netTemp1->flightCheck(1000, 250, "TempSSR");
        bool error_netTemp2 = _netTemp2->flightCheck(1000, 250, "TempSSR");
        float temp1 = _netTemp1->getValue();
        float temp2 = _netTemp2->getValue();

        if (!(error_netTemp1 && error_netTemp2))
        {
            // 2 temp sensors provided, take the higher one
            update(std::max(temp1, temp2));
            return;
        }
        else
        {
            if (error_netTemp2)
            {
                update(temp1);
                return;
            }
            if (error_netTemp1)
            {
                update(temp2);
                return;
            }
        }
    }

    // catch any other case
    digitalWrite(_gpio, LOW);
    _state.newFlag(LIBRRC::COMPONENT_STATUS_FLAGS::ERROR);
}

void NRCRemoteTempSSR::update(float sensor_temperature){

    //ensures that if this component is diarmed or encounters an error, the SSR is disabled
    if (!_state.flagSet(LIBRRC::COMPONENT_STATUS_FLAGS::NOMINAL)){ 
        digitalWrite(_gpio,LOW);
        return;
    }
    
    if(sensor_temperature < _value){
        digitalWrite(_gpio,HIGH);
    }else{
         digitalWrite(_gpio,LOW);
    }


}

void NRCRemoteTempSSR::execute_impl(packetptr_t packetptr)
{
    SimpleCommandPacket execute_command(*packetptr);
    _value = execute_command.arg;
}
