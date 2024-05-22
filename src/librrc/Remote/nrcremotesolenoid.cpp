#include "nrcremotesolenoid.h"
#include <Arduino.h>
#include <Preferences.h>

#include <librrc/Helpers/nvsstore.h>

void NRCRemoteSolenoid::setup()
{
    // 1 always opens 0 always closes 
    pinMode(_togglePin, OUTPUT);
    digitalWrite(_togglePin, LOW);
    pinMode(_contPin, INPUT); 
    this->_state.newFlag(LIBRRC::COMPONENT_STATUS_FLAGS::DISARMED);
    updateContinuity();
}

void NRCRemoteSolenoid::execute_impl(packetptr_t packetptr)
{
    SimpleCommandPacket execute_command(*packetptr);
    execute(execute_command.arg);
}

void NRCRemoteSolenoid::execute(int32_t arg)
{
    if (arg < 0)
    {
        return;
    }
    if (!this->_state.flagSet(LIBRRC::COMPONENT_STATUS_FLAGS::NOMINAL))
    { 
        return;
    }       
    if (arg == 1)
    {
        digitalWrite(_togglePin, HIGH);
    }
    if (arg == 0)
    {
        digitalWrite(_togglePin, LOW);
    }
    

}
void NRCRemoteSolenoid::arm_impl(packetptr_t packetptr)
{
    SimpleCommandPacket armingpacket(*packetptr);
    arm(armingpacket.arg);
}

void NRCRemoteSolenoid::arm(int32_t arg)
{
    if (arg != 1)
    {
        m_contCheckOverride = false;
    }
    else
    {
        m_contCheckOverride = true;
    }

    updateContinuity();

    if (this->_state.getStatus() == static_cast<uint16_t>(LIBRRC::COMPONENT_STATUS_FLAGS::DISARMED) || (this->_state.flagSetAnd(LIBRRC::COMPONENT_STATUS_FLAGS::DISARMED, LIBRRC::COMPONENT_STATUS_FLAGS::ERROR_CONTINUITY) && m_contCheckOverride))
    {
        this->_state.deleteFlag(LIBRRC::COMPONENT_STATUS_FLAGS::DISARMED);
        this->_state.newFlag(LIBRRC::COMPONENT_STATUS_FLAGS::NOMINAL);
    }
};
void NRCRemoteSolenoid::updateContinuity()
{
    if (digitalRead(_contPin)) // high if continuity - True
    {
        if (this->_state.flagSet(LIBRRC::COMPONENT_STATUS_FLAGS::ERROR_CONTINUITY))
        {
            this->_state.deleteFlag(LIBRRC::COMPONENT_STATUS_FLAGS::ERROR_CONTINUITY);
        }
    }
    else
    {
        if (!this->_state.flagSet(LIBRRC::COMPONENT_STATUS_FLAGS::ERROR_CONTINUITY))
        {
            this->_state.newFlag(LIBRRC::COMPONENT_STATUS_FLAGS::ERROR_CONTINUITY);
        }
    }
}


// 1 this pc
// 0 serial direct connection
// 10 ID closest to usb
// 3 arm (arg is 1 for overide continuity) 2 fire (seconds in ms)
// 0 (arg)



// std arming is arg 0
// need this to not work if no continuity
