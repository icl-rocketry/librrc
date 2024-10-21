#pragma once

#include <librnp/rnp_packet.h>
#include <librnp/rnp_serializer.h>

#include <vector>

class SolenoidCalibrationPacket : public RnpPacket{
    private:
    //serializer framework
        static constexpr auto getSerializer()
        {
            auto ret = RnpSerializer(
                &SolenoidCalibrationPacket::command,
                &SolenoidCalibrationPacket::normalState
            );

            return ret;
        }
        
    public:
        ~SolenoidCalibrationPacket();

        SolenoidCalibrationPacket();
        /**
         * @brief Deserialize Packet
         * 
         * @param data 
         */
        SolenoidCalibrationPacket(const RnpPacketSerialized& packet);

        /**
         * @brief Serialize Packet
         * 
         * @param buf 
         */
        void serialize(std::vector<uint8_t>& buf) override;

        void deserializeBody(std::vector<uint8_t>& buf);
        
        uint8_t command;
        uint16_t normalState;

        static constexpr size_t size(){
            return getSerializer().member_size();
        }

};


