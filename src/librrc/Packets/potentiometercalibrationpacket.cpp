#include "potentiometercalibrationpacket.h"

#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>

#include <vector>



PotentiometerCalibrationPacket::~PotentiometerCalibrationPacket()
{};

PotentiometerCalibrationPacket::PotentiometerCalibrationPacket():
RnpPacket(0,
          108,
          size())
{};

PotentiometerCalibrationPacket::PotentiometerCalibrationPacket(const RnpPacketSerialized& packet):
RnpPacket(packet,size())
{
    getSerializer().deserialize(*this,packet.getBody());
};

void PotentiometerCalibrationPacket::deserializeBody(std::vector<uint8_t>& buf){
    getSerializer().deserialize(*this, buf);
};

void PotentiometerCalibrationPacket::serialize(std::vector<uint8_t>& buf){
    RnpPacket::serialize(buf);
	size_t bufsize = buf.size();
	buf.resize(bufsize + size());
	std::memcpy(buf.data() + bufsize,getSerializer().serialize(*this).data(),size());
};