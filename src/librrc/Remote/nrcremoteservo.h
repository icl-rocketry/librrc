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
     * @param networkmanager RNP network manager object
     * @param defaultAngle Angle the servo will go to when setup is called
     * @param name Unique name for channle. Used to save calirbations in NVS [TEMPORARY] //!Go annoy ANDU if this is still here
     * @param minAngle Minimum angle the servo can reach. Note that this is not the minimum angle
     * the servo will be restricted to, but the minimum angle the servo si capable of going to.
     * @param maxAngle Maximum angle the servo can reach. Note that this is not the maxmium angle
     * the servo will be restricted to, but the maximum angle the servo si capable of going to.
     * @param minWidth Minimum pulse width in Usec
     * @param maxWidth Maximum pulse width in Usec
     * @param minAngleLimit minimum angle limit
     * @param maxAngleLimit maximum angle limit
     */

    explicit NRCRemoteServo(PWMHAL pwmOut,
                   RnpNetworkManager &networkmanager,
                   std::string name = "",
                   uint32_t defaultAngle = 0,
                   uint32_t minAngle = 0,
                   uint32_t maxAngle = 180,
                   uint32_t minWidth = 500,
                   uint32_t maxWidth = 2500,
                   uint32_t minAngleLimit = 0,
                   uint32_t maxAngleLimit = 360) : NRCRemoteActuatorBase(networkmanager),
                                                        m_pwmOut(pwmOut),
                                                        calibration{
                                                            .defaultAngle=default_angle,
                                                            .minAngle = minAngle,
                                                            .maxAngle = maxAngle,
                                                            .minWidth = minWidth,
                                                            .maxWidth = maxWidth,
                                                            .minAngleLimit = minAngleLimit,
                                                            .maxAngleLimit = maxAngleLimit,
                                                            .pwmFreq = pwmOut.getFrequency(),
                                                            .pwmRes = pwmOut.getResolution()
                                                            }
                                                         {};



    NRCRemoteServo() = delete; 


    void setup()
    {
        //check for any pre-saved calibraiton
        loadCalibration();
        m_pwmOut.setPWMParam(calibration.pwmFreq,calibration.pwmRes);
        execute_base(calibration.defaultAngle); // send servo to default position
    };


    void setAngleLims(uint16_t min, uint16_t max)
    {
        calibration.minAngleLimit = min;
        calibration.maxAngleLimit = max;
    };

    void setDefault(uint16_t defaultAngle)
    {
        calibration.defaultAngle = defaultAngle;
    };

 


protected:
    
    //! NRCRemote Interface Implmentation

    void calibrate_impl(packetptr_t packetptr)
    {
        //get serialized packet data
        std::vector<uint8_t> serializedData = packetptr->getBody();

        NVSStore _NVS(_name, NVSStore::calibrationType::Servo);
        //save to nvs store
        _NVS.saveBytes(serializedData);
        //deserialzie into calibration object
        calibration.deserializeBody(serializedData);
        // write calibration to udnerlying pwm channel
        m_pwmOut.setPWMParam(calibration.pwmFreq,calibration.pwmRes);
    };

   
    //! Base Methods

    void execute_base(int32_t arg)
    {

        if (arg < 0)
        {
            return;
        }

        int32_t pulseWidth = LIBRRC::rangemap<int32_t>(angle, calibration.minAngle, calibration.maxAngle, calibration.minWidth, calibration.maxWidth);
        m_pwmOut.writeWidth(pulseWidth);

    };


private:

    void loadFromNVS()
    {
        NVSStore _NVS(_name, NVSStore::calibrationType::Servo);

        std::vector<uint8_t> calibSerialised = _NVS.loadBytes();

        if (calibSerialised.size() == 0)
        {
            return;
        }

        calibration.deserializeBody(calibSerialised); // load calibration from nvs into local calibraiton object
    };

protected:
    //variables
    
    template<typename T> friend class NRCRemoteBase;

    //! NRCRemote Interface
    template<typename T> friend class NRCRemoteActuatorBase;

    //! Local Component Interface
    friend class RemoteActuatorAdapter<NRCRemoteServo>;

    /**
     * @brief Pwm output
     * 
     */
    PWMHAL m_pwmOut;

    ServoCalibrationPacket calibration;

  
};