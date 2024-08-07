#pragma once

#include <librnp/rnp_packet.h>
#include <librnp/rnp_serializer.h>

#include <vector>

class PotentiometerCalibrationPacket : public RnpPacket{
    private:
    //serializer framework
        static constexpr auto getSerializer()
        {
            auto ret = RnpSerializer(
                &PotentiometerCalibrationPacket::command,
                &PotentiometerCalibrationPacket::zero_angl,
                &PotentiometerCalibrationPacket::max_angl
            );

            return ret;
        }

        public:
        ~PotentiometerCalibrationPacket();

        PotentiometerCalibrationPacket();
        /**
         * @brief Deserialize Packet
         * 
         * @param data 
         */
        PotentiometerCalibrationPacket(const RnpPacketSerialized& packet);
        
        void deserializeBody(std::vector<uint8_t>& buf);
        /**
         * @brief Serialize Packet
         * 
         * @param buf 
         */
        void serialize(std::vector<uint8_t>& buf) override;
   

        uint8_t command;
        float zero_angl;
        float max_angl;

        static constexpr size_t size(){
            return getSerializer().member_size();
        }

};