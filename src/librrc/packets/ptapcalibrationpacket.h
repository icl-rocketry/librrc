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
                &PTapCalibrationPacket::command,
                &PTapCalibrationPacket::c,
                &PTapCalibrationPacket::grad
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