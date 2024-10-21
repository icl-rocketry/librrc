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
                &ServoCalibrationPacket::defaultAngle,
                &ServoCalibrationPacket::minAngle,
                &ServoCalibrationPacket::maxAngle,
                &ServoCalibrationPacket::minWidth,
                &ServoCalibrationPacket::maxWidth,
                &ServoCalibrationPacket::minAngleLimit,
                &ServoCalibrationPacket::maxAngleLimit
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

        /**
         * @brief Default angle servo goes to
         * 
         */
        uint32_t defaultAngle;
        /**
         * @brief angle corresponding to minium pulse width
         * 
         */
        uint32_t minAngle;
        /**
         * @brief angle corresponding to maximum pulse width
         * 
         */
        uint32_t maxAngle;
        /**
         * @brief Minium pulse width in Usec
         * 
         */
        uint32_t minWidth;
        /**
         * @brief Maximum pulse width in Usec
         * 
         */
        uint32_t maxWidth;
        /**
         * @brief Minimum Angle Limit
         * 
         */
        uint32_t minAngleLimit;
        /**
         * @brief Maximum Angle Limit
         * 
         */
        uint32_t maxAngleLimit;

        /**
         * @brief Base PWM frequency
         * 
         */
        uint32_t pwmFreq;
        /**
         * @brief PWM resolution in n bits. NB for some pwm HAL types, this paramter
         * is not changeable and so sending a different resolution will have no effect
         * 
         */
        uint8_t pwmRes;

        static constexpr size_t size(){
            return getSerializer().member_size();
        }

};