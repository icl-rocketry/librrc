#pragma once

#include <librnp/rnp_packet.h>
#include <librnp/rnp_serializer.h>

#include <vector>

class PTapCalibrationPacket : public RnpPacket{
    private:
    //serializer framework
        static constexpr auto getSerializer()
        {
            auto ret = RnpSerializer(
                &PTapCalibrationPacket::grad,
                &PTapCalibrationPacket::c,
                &PTapCalibrationPacket::resistance
            );

            return ret;
        }
        
    public:
        ~PTapCalibrationPacket();

        PTapCalibrationPacket();
        /**
         * @brief Deserialize Packet
         * 
         * @param data 
         */
        PTapCalibrationPacket(const RnpPacketSerialized& packet);

        /**
         * @brief Serialize Packet
         * 
         * @param buf 
         */
        void serialize(std::vector<uint8_t>& buf) override;
        void deserializeBody(std::vector<uint8_t>& buf);

        float grad;
        float c;
        float resistance;

        static constexpr size_t size(){
            return getSerializer().member_size();
        }

};