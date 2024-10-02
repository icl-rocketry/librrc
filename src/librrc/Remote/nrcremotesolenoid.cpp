#include "nrcremotesolenoid.h"
#include <Arduino.h>
#include <Preferences.h>
#include <librrc/Packets/solenoidcalibrationpacket.h>


#include <librrc/Helpers/nvsstore.h>

void NRCRemoteSolenoid::setup()
{
    loadCalibration();
    // 1 always opens 0 always closes 

    //setup pin -> write default state
    pinMode(_togglePin, OUTPUT);
    digitalWrite(_togglePin, normalState);
    
    // if we have a NO valve (normalState = 1), we want te default value
    // to be CLOSED which means we digital write the pin so that the valve closes,
    //but this represents the closed state which maps to this->_value = 0
    this->_value = !normalState;

}

// void NRCRemoteSolenoid::execute_impl(packetptr_t packetptr)
// {
//     SimpleCommandPacket execute_command(*packetptr);
//     execute(execute_command.arg);
// }

void NRCRemoteSolenoid::execute_base(int32_t arg) 
{
    _value = arg; // 1 always opens 0 always closes    

    digitalWrite(_togglePin,normalState^arg);

}

void NRCRemoteSolenoid::loadCalibration(){
    
    std::string NVSName = "Soln" + std::to_string(_togglePin);
    NVSStore _NVS(NVSName, NVSStore::calibrationType::Solenoid);
    

    std::vector<uint8_t> calibSerialised = _NVS.loadBytes();
    if(calibSerialised.size() == 0){
        // setNormalState(0); // default is nominally closed
        normalState = 0; // default NC
        return;
    }
    SolenoidCalibrationPacket calibpacket;
    calibpacket.deserializeBody(calibSerialised);

    normalState = calibpacket.normalState;
    // setNormalState(calibpacket.normalState);

}

void NRCRemoteSolenoid::calibrate_impl(packetptr_t packetptr){
    
    SolenoidCalibrationPacket calibrate_comm(*packetptr);

    std::vector<uint8_t> serialisedvect = packetptr->getBody();

    std::string NVSName = "Soln" + std::to_string(_togglePin);
    NVSStore _NVS(NVSName, NVSStore::calibrationType::Solenoid);
    
    _NVS.saveBytes(serialisedvect);
    
    normalState = calibrate_comm.normalState;

    // setNormalState(calibrate_comm.normalState);
}
