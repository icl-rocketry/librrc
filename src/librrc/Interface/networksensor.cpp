#include "networksensor.h"
#include <librrc/Packets/nrcpackets.h>
#include <librnp/default_packets/simplecommandpacket.h>
#include <Arduino.h> // needed for millis


NetworkSensor::~NetworkSensor(){};

void NetworkSensor::networkCallback(packetptr_t packetptr){
    if (packetptr->header.source != _address){
        return; // packet had incorrect data
    }
    switch (packetptr->header.type){
        case(static_cast<uint8_t>(NRCPacket::TYPES::NRC_STATE)):
        {
            NRCStatePacket nrcstatepacket(*packetptr); //deserialize state packet
            lastStateUpdateTime = millis();
            sensorValue = *reinterpret_cast<float*>(&nrcstatepacket.value); // reinterpret int32 as a float
            _state.trackRemoteState(nrcstatepacket.state);
        }
    
    }

   
}

void NetworkSensor::updateState(){
    
    SimpleCommandPacket getstate_packet(static_cast<uint8_t>(NRCPacket::NRC_COMMAND_ID::GETSTATE),0); 
    getstate_packet.header.type = static_cast<uint8_t>(NRCPacket::TYPES::NRC_COMMAND);
    getstate_packet.header.source = _networkmanager.getAddress();
    getstate_packet.header.source_service = _sourceService;
    getstate_packet.header.destination = _address;
    getstate_packet.header.destination_service = _destinationService;

    _networkmanager.sendPacket(getstate_packet);
    
    lastStateRequestTime = millis();
}