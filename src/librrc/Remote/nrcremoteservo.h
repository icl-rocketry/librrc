#pragma once

#include "nrcremoteactuatorbase.h"

#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>

#include <librrc/HAL/localpwm.h>
#include <librrc/Packets/servocalibrationpacket.h>
#include <librrc/Helpers/nvsstore.h>
#include <librrc/Helpers/rangemap.h>

#include <math.h>
#include <Preferences.h>

template<typename PWMHAL = LocalPWM>
class NRCRemoteServo : public NRCRemoteActuatorBase<NRCRemoteServo>
{

public:
    /**
     * @brief Using this constructor initialises the object using counts, i.e. the value written
     * to the PWM generator and not the timing values like what would be given on a servo datasheet.
     *
     * @param gpio GPIO pin number the servo control wire is connected to
     * @param channel PWM generator channel
     * @param networkmanager RNP network manager object
     * @param default_angle Angle the servo will go to when setup is called
     * @param min_angle Minimum angle the servo can reach. Note that this is not the minimum angle
     * the servo will be restricted to, but the minimum angle the servo si capable of going to.
     * @param max_angle Maximum angle the servo can reach. Note that this is not the maxmium angle
     * the servo will be restricted to, but the maximum angle the servo si capable of going to.
     * @param angl_lim_min Minimum angle the servo will be allowed to go to
     * @param angl_lim_min Maximum angle the servo will be allowed to go to. Note that this should be within
     * the range of the min and max angle parameters.
     * @param min_counts Minimum pulse width in Usec
     * @param max_counts Maximum pulse width in Usec
     */

    NRCRemoteServo(PWMHAL pwmOut,
                   RnpNetworkManager &networkmanager,
                   uint32_t default_angle = 0,
                   uint32_t min_angle = 0,
                   uint32_t max_angle = 180,
                   uint32_t min_width = 500,
                   uint32_t max_width = 2500) : NRCRemoteActuatorBase(networkmanager),
                                                        m_pwmOut(pwmOut)
                                                        m_default_angle(default_angle),
                                                        m_min_angle(min_angle),
                                                        m_max_angle(max_angle),
                                                        m_min_counts(min_width),
                                                        m_max_counts(max_width) {};

    void setup()
    {

        // ledcSetup(_channel, freq, timer_width);
        // ledcAttachPin(_gpio, _channel);

        loadCalibration();
        reset(); // send servo to default position
    };

    /**
     * @brief Temporary implementation of reset. Drives servo to min angle position. This needs to be updated
     * to be configurable.
     *
     */
    void reset()
    {
        goto_Angle(_default_angle);
    };

    void setAngleLims(uint16_t minangle, uint16_t maxangle)
    {
        _angl_lim_min = minangle;
        _angl_lim_max = maxangle;
    };

    void setHome(uint16_t homeangle)
    {
        _default_angle = homeangle;
    };


protected:
    friend class NRCRemoteActuatorBase;
    friend class NRCRemoteBase;



    PWMHAL m_pwmOut;

    uint16_t _default_angle;
    const uint16_t _min_angle;
    const uint16_t _max_angle;

    uint16_t _angl_lim_min;
    uint16_t _angl_lim_max;

    const uint16_t _min_counts;
    const uint16_t _max_counts;

    void loadCalibration()
    {

        std::string NVSName = "Srvo" + std::to_string(_channel);
        NVSStore _NVS(NVSName, NVSStore::calibrationType::Servo);

        std::vector<uint8_t> calibSerialised = _NVS.loadBytes();

        if (calibSerialised.size() == 0)
        {
            return;
        }
        ServoCalibrationPacket calibpacket;
        calibpacket.deserializeBody(calibSerialised);

        setHome(calibpacket.home_angl);
        setAngleLims(calibpacket.min_angl, calibpacket.max_angl);
    };

    void execute_impl(packetptr_t packetptr)
    {

        SimpleCommandPacket execute_command(*packetptr);
        if (execute_command.arg < 0)
        {
            return;
        }

        goto_Angle(execute_command.arg);
    };

    void calibrate_impl(packetptr_t packetptr)
    {

        ServoCalibrationPacket calibrate_comm(*packetptr);

        std::vector<uint8_t> serialisedvect = packetptr->getBody();

        std::string NVSName = "Srvo" + std::to_string(_channel);
        NVSStore _NVS(NVSName, NVSStore::calibrationType::Servo);

        _NVS.saveBytes(serialisedvect);

        setHome(calibrate_comm.home_angl);

        setAngleLims(calibrate_comm.min_angl, calibrate_comm.max_angl);
    };

    uint16_t angletocounts(uint16_t angle)
    {
        return LIBRRC::rangemap<uint16_t>(angle, _min_angle, _max_angle, _min_counts, _max_counts);
    };
    float angletocountsHighRes(float angle)
    {
        return LIBRRC::rangemap<float>(angle, _min_angle, _max_angle, _min_counts, _max_counts);
    };

    static constexpr int timer_width = 14;
    static constexpr int freq = 50;

    static constexpr int counts(int usec)
    {
        return (int)(float(usec) / (float(1000000 / freq) / float(std::pow(2, timer_width))));
    }
};