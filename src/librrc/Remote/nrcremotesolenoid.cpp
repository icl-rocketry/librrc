#include "nrcremotesolenoid.h"
#include <Arduino.h>
#include <Preferences.h>
#include <librrc/Packets/solenoidcalibrationpacket.h>


#include <librrc/Helpers/nvsstore.h>

void NRCRemoteSolenoid::setup()
{
    loadCalibration();
    // 1 always opens 0 always closes 
    pinMode(_togglePin, OUTPUT);
    digitalWrite(_togglePin, normalState);
    
    this->_state.newFlag(LIBRRC::COMPONENT_STATUS_FLAGS::DISARMED);
    this->_value = normalState;

}

void NRCRemoteSolenoid::execute_impl(packetptr_t packetptr)
{
    SimpleCommandPacket execute_command(*packetptr);
    execute(execute_command.arg);
}

void NRCRemoteSolenoid::execute(int32_t arg) 
{
    if (arg < 0)
    {
        return;
    }
    if (!this->_state.flagSet(LIBRRC::COMPONENT_STATUS_FLAGS::NOMINAL))
    { 
        return;
    }  
    _value = arg; // 1 always opens 0 always closes    
    if (normalState == 0 && arg == 0) { 
        digitalWrite(_togglePin, LOW);
    }
    if (normalState == 0 && arg == 1) { 
        digitalWrite(_togglePin, HIGH);
    }
    if (normalState == 1 && arg == 0) { 
        digitalWrite(_togglePin, HIGH);
    }
    if (normalState == 1 && arg == 1) { 
        digitalWrite(_togglePin, LOW);
    }

}

void NRCRemoteSolenoid::loadCalibration(){
    
    std::string NVSName = "Soln" + std::to_string(_togglePin);
    NVSStore _NVS(NVSName, NVSStore::calibrationType::Solenoid);
    

    std::vector<uint8_t> calibSerialised = _NVS.loadBytes();
    if(calibSerialised.size() == 0){
        setNormalState(0); // default is nominally closed
        return;
    }
    SolenoidCalibrationPacket calibpacket;
    calibpacket.deserializeBody(calibSerialised);

    setNormalState(calibpacket.normalState);

}

void NRCRemoteSolenoid::calibrate_impl(packetptr_t packetptr){
    
    SolenoidCalibrationPacket calibrate_comm(*packetptr);

    std::vector<uint8_t> serialisedvect = packetptr->getBody();

    std::string NVSName = "Soln" + std::to_string(_togglePin);
    NVSStore _NVS(NVSName, NVSStore::calibrationType::Solenoid);
    
    _NVS.saveBytes(serialisedvect);
    
    setNormalState(calibrate_comm.normalState);
}
