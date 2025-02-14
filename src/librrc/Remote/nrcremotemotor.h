#pragma once

#include "nrcremoteactuatorbase.h"

#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>

#include <librrc/HAL/localpwm.h>
#include <librrc/Local/remoteactuatoradapter.h>

template <typename PWMHAL = LocalPWM>
class NRCRemoteMotor : public NRCRemoteActuatorBase<NRCRemoteMotor<PWMHAL>>
{

    // type alias for explict NRCRemoteBase type
    using NRCRemoteActuatorBase_T = NRCRemoteActuatorBase<NRCRemoteMotor>;
    using NRCRemoteBase_T = NRCRemoteBase<NRCRemoteMotor>;

public:
    NRCRemoteMotor(RnpNetworkManager &networkmanager,
                   PWMHAL pwm0,
                   PWMHAL pwm1) : NRCRemoteActuatorBase_T(networkmanager),
                                  m_pwm0(pwm0),
                                  m_pwm1(pwm1)
    {
        m_pwm0.setPWMParam(freq, timer_width);
        m_pwm1.setPWMParam(freq, timer_width);
    };

    void setup()
    {
        m_pwm0.writeDuty(0);
        m_pwm1.writeDuty(0);
    };

protected:
    PWMHAL m_pwm0;
    PWMHAL m_pwm1;

    void execute_impl(packetptr_t packetptr)
    {
        SimpleCommandPacket execute_command(*packetptr);
        execute_base(execute_command.arg);
    }

    //! Base method
    void execute_base(int32_t speed)
    {
        this->_value = speed; // update last position of actuator
        if ((speed > 100) && (speed <= 200))
        { // reverse rotation
            m_pwm0.writeDuty(0);
            m_pwm1.writeDuty(LIBRRC::rangemap<uint32_t>(speed, 100, 200, 0, m_max_duty));
        }
        if ((speed > 0) && speed <= 100)
        { // forward rotation
            m_pwm0.writeDuty(LIBRRC::rangemap<uint32_t>(speed, 0, 100, 0, m_max_duty));
            m_pwm1.writeDuty(0);
        }
        if (speed == 0)
        { // stop and freewheel
            m_pwm0.writeDuty(0);
            m_pwm1.writeDuty(0);
        }
    }

    template <typename T> friend class NRCRemoteBase;

    //! NRCRemote Interface
    template <typename T> friend class NRCRemoteActuatorBase;

    //! Local Component Interface
    friend class RemoteActuatorAdapter<NRCRemoteMotor>;

    //! TEMP - should eventually be a calibration thing
    static constexpr int timer_width = 8;
    static constexpr int freq = 30000;
    static constexpr int m_max_duty = 255;
};