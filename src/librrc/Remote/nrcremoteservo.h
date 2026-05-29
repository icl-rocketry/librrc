#pragma once

#include "nrcremoteactuatorbase.h"

#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>

#include <librrc/HAL/localpwm.h>
#include <librrc/Packets/servocalibrationpacket.h>
#include <librrc/Helpers/nvsstore.h>
#include <librrc/Helpers/rangemap.h>
#include <librrc/Local/remoteactuatoradapter.h>

#include <math.h>
#include <Preferences.h>

template<typename PWMHAL = LocalPWM>
class NRCRemoteServo : public NRCRemoteActuatorBase<NRCRemoteServo<PWMHAL>>
{
     //type alias for explict NRCRemoteBase type
    using NRCRemoteActuatorBase_T = NRCRemoteActuatorBase<NRCRemoteServo>;
    using NRCRemoteBase_T = NRCRemoteBase<NRCRemoteServo>;

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
                   uint32_t maxAngle = 100,
                   uint32_t minWidth = 1100,
                   uint32_t maxWidth = 1940,
                   uint32_t minAngleLimit = 0,
                   uint32_t maxAngleLimit = 100) : NRCRemoteActuatorBase_T(name,networkmanager),
                                                        m_pwmOut(pwmOut)
                                                         {
                                                            calibration.defaultAngle = defaultAngle;
                                                            calibration.minAngle = minAngle;
                                                            calibration.maxAngle = maxAngle;
                                                            calibration.minWidth = minWidth;
                                                            calibration.maxWidth = maxWidth;
                                                            calibration.minAngleLimit = minAngleLimit;
                                                            calibration.maxAngleLimit = maxAngleLimit;
                                                            calibration.pwmFreq = pwmOut.getFrequency();
                                                            calibration.pwmRes = pwmOut.getResolution();
                                                         };



    NRCRemoteServo() = delete; 


    void setup(bool useNVS = true)
    {
        if (useNVS) {
            loadFromNVS();
        }

        m_pwmOut.setPWMParam(calibration.pwmFreq,calibration.pwmRes);
        execute_base(calibration.defaultAngle);
    };


    void setAngleLims(uint32_t min, uint32_t max)
    {
        calibration.minAngleLimit = min;
        calibration.maxAngleLimit = max;
    };

    void setDefault(uint32_t defaultAngle)
    {
        calibration.defaultAngle = defaultAngle;
    };

 


protected:
    
    //! NRCRemote Interface Implmentation

    void calibrate_impl(packetptr_t packetptr)
    {
        //get serialized packet data
        std::vector<uint8_t> serializedData = packetptr->getBody();

        NVSStore _NVS(this->_name, NVSStore::calibrationType::Servo);
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
        
        //bound input angle on limits
        int32_t angle = std::max(std::min(static_cast<uint32_t>(arg),calibration.maxAngleLimit),calibration.minAngleLimit);
        this->_value = angle; //update angle

        int32_t pulseWidth = LIBRRC::rangemap<int32_t>(angle, calibration.minAngle, calibration.maxAngle, calibration.minWidth, calibration.maxWidth);
        
        m_pwmOut.writeWidth(pulseWidth);

    };


private:

    void loadFromNVS()
    {
        NVSStore _NVS(this->_name, NVSStore::calibrationType::Servo);

        std::vector<uint8_t> calibSerialised = _NVS.loadBytes();

        if (calibSerialised.size() == 0)
        {
            return;
        }

        if (calibSerialised.size() != calibration.size())
        {
            return;
        }

        calibration.deserializeBody(calibSerialised); // load calibration from nvs into local calibraiton object
    };
// void loadFromNVS()
// {
//     delay(7000);
//     Serial.println("===== ENTERING loadFromNVS =====");

//     NVSStore _NVS(this->_name, NVSStore::calibrationType::Servo);

//     std::vector<uint8_t> calibSerialised = _NVS.loadBytes();

//     Serial.print("Servo name: ");
//     Serial.println(this->_name.c_str());

//     Serial.print("Loaded byte count: ");
//     Serial.println((int)calibSerialised.size());

//     if (calibSerialised.size() == 0)
//     {
//         Serial.println("No calibration stored in NVS.");
//         return;
//     }

//     if (calibSerialised.size() != calibration.size())
//     {
//         Serial.print("Size mismatch. Stored = ");
//         Serial.print((int)calibSerialised.size());
//         Serial.print(", expected = ");
//         Serial.println((int)calibration.size());
//         return;
//     }

//     calibration.deserializeBody(calibSerialised);
//     Serial.println("Calibration loaded from NVS.");
//     Serial.println("---- Loaded calibration values ----");
//     Serial.print("command: ");
//     Serial.println(calibration.command);

//     Serial.print("defaultAngle: ");
//     Serial.println(calibration.defaultAngle);

//     Serial.print("minAngle: ");
//     Serial.println(calibration.minAngle);

//     Serial.print("maxAngle: ");
//     Serial.println(calibration.maxAngle);

//     Serial.print("minWidth: ");
//     Serial.println(calibration.minWidth);

//     Serial.print("maxWidth: ");
//     Serial.println(calibration.maxWidth);

//     Serial.print("minAngleLimit: ");
//     Serial.println(calibration.minAngleLimit);

//     Serial.print("maxAngleLimit: ");
//     Serial.println(calibration.maxAngleLimit);
// }
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