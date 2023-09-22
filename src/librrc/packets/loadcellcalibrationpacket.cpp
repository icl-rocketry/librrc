#include "loadcellcalibrationpacket.h"

#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>

#include <vector>



LoadCellCalibrationPacket::~LoadCellCalibrationPacket()
{};

LoadCellCalibrationPacket::LoadCellCalibrationPacket():
RnpPacket(0,
          107,
          size())
{};

LoadCellCalibrationPacket::LoadCellCalibrationPacket(const RnpPacketSerialized& packet):
RnpPacket(packet,size())
{
    getSerializer().deserialize(*this,packet.getBody());
};

void LoadCellCalibrationPacket::deserializeBody(std::vector<uint8_t>& buf){
    getSerializer().deserialize(*this, buf);
};

void LoadCellCalibrationPacket::serialize(std::vector<uint8_t>& buf){
    RnpPacket::serialize(buf);
	size_t bufsize = buf.size();
	buf.resize(bufsize + size());
	std::memcpy(buf.data() + bufsize,getSerializer().serialize(*this).data(),size());
};