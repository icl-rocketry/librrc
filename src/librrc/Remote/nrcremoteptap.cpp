#include "nrcremoteptap.h"


void NRCRemotePTap::update(int32_t ADCReading){
    _Pressure = (((float) ADCReading - (float)_c)/_grad);
    updateSensorValue(_Pressure);
}

void NRCRemotePTap::calibrate_impl(packetptr_t packetptr){
    
    PTapCalibrationPacket calibrate_comm(*packetptr);

    std::vector<uint8_t> serialisedvect = packetptr->getBody();

    std::string NVSName = "PTAP" + std::to_string(_ptapIndex);
    NVSStore _NVS(NVSName, NVSStore::calibrationType::PTap);
    
    _NVS.saveBytes(serialisedvect);

    _c = calibrate_comm.c;
    _grad = calibrate_comm.grad;
}

void NRCRemotePTap::loadCalibration(){
    
    std::string NVSName = "PTAP" + std::to_string(_ptapIndex);
    NVSStore _NVS(NVSName, NVSStore::calibrationType::PTap);

    std::vector<uint8_t> calibSerialised = _NVS.loadBytes();
    
    if(calibSerialised.size() == 0){
        return;
    }
    PTapCalibrationPacket calibpacket;
    calibpacket.deserializeBody(calibSerialised);

    _c = calibpacket.c;
    _grad = calibpacket.grad;

}