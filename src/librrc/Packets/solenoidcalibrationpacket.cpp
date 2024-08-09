#include "solenoidcalibrationpacket.h"

#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>

#include <vector>



SolenoidCalibrationPacket::~SolenoidCalibrationPacket()
{};

SolenoidCalibrationPacket::SolenoidCalibrationPacket():
RnpPacket(0,
          115,
          size())
{};

SolenoidCalibrationPacket::SolenoidCalibrationPacket(const RnpPacketSerialized& packet):
RnpPacket(packet,size())
{
    getSerializer().deserialize(*this,packet.getBody());
};

void SolenoidCalibrationPacket::deserializeBody(std::vector<uint8_t>& buf){
    getSerializer().deserialize(*this, buf);
}

void SolenoidCalibrationPacket::serialize(std::vector<uint8_t>& buf){
    RnpPacket::serialize(buf);
	size_t bufsize = buf.size();
	buf.resize(bufsize + size());
	std::memcpy(buf.data() + bufsize,getSerializer().serialize(*this).data(),size());
};