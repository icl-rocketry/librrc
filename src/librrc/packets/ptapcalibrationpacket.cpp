#include "PTapCalibrationPacket.h"

#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>

#include <vector>



PTapCalibrationPacket::~PTapCalibrationPacket()
{};

PTapCalibrationPacket::PTapCalibrationPacket():
RnpPacket(0,
          105,
          size())
{};

PTapCalibrationPacket::PTapCalibrationPacket(const RnpPacketSerialized& packet):
RnpPacket(packet,size())
{
    getSerializer().deserialize(*this,packet.getBody());
};

void PTapCalibrationPacket::serialize(std::vector<uint8_t>& buf){
    RnpPacket::serialize(buf);
	size_t bufsize = buf.size();
	buf.resize(bufsize + size());
	std::memcpy(buf.data() + bufsize,getSerializer().serialize(*this).data(),size());
};