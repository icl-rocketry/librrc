#pragma once

#include <librnp/rnp_packet.h>
#include <librnp/rnp_serializer.h>

#include <vector>

class ServoCalibrationPacket : public RnpPacket{
    private:
    //serializer framework
        static constexpr auto getSerializer()
        {
            auto ret = RnpSerializer(
                &ServoCalibrationPacket::command,
                &ServoCalibrationPacket::min_angl,
                &ServoCalibrationPacket::max_angl,
                &ServoCalibrationPacket::home_angl
            );

            return ret;
        }
        
    public:
        ~ServoCalibrationPacket();

        ServoCalibrationPacket();
        /**
         * @brief Deserialize Packet
         * 
         * @param data 
         */
        ServoCalibrationPacket(const RnpPacketSerialized& packet);

        /**
         * @brief Serialize Packet
         * 
         * @param buf 
         */
        void serialize(std::vector<uint8_t>& buf) override;

        void deserializeBody(std::vector<uint8_t>& buf);
        
        uint8_t command;
        uint16_t min_angl;
        uint16_t max_angl;
        uint16_t home_angl;

        static constexpr size_t size(){
            return getSerializer().member_size();
        }

};


