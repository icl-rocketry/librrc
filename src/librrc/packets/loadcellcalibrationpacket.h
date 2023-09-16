#pragma once

#include <librnp/rnp_packet.h>
#include <librnp/rnp_serializer.h>

#include <vector>

class LoadCellCalibrationPacket : public RnpPacket{
    private:
    //serializer framework
        static constexpr auto getSerializer()
        {
            auto ret = RnpSerializer(
                &LoadCellCalibrationPacket::command,
                &LoadCellCalibrationPacket::c,
                &LoadCellCalibrationPacket::grad
            );

            return ret;
        }
        
    public:
        ~LoadCellCalibrationPacket();

        LoadCellCalibrationPacket();
        /**
         * @brief Deserialize Packet
         * 
         * @param data 
         */
        LoadCellCalibrationPacket(const RnpPacketSerialized& packet);
        
        void deserializeBody(std::vector<uint8_t>& buf);
        /**
         * @brief Serialize Packet
         * 
         * @param buf 
         */
        void serialize(std::vector<uint8_t>& buf) override;

        uint8_t command;
        float c;
        float grad;

        static constexpr size_t size(){
            return getSerializer().member_size();
        }

};