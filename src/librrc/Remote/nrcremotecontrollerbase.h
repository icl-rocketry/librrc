#pragma once
#include "nrcremotebase.h"

#include <string>
#include <librnp/rnp_networkmanager.h>
#include <librrc/componentstatusflags.h>


template <typename Derived>
class NRCRemoteControllerBase : public NRCRemoteBase<Derived>
{
public:
    NRCRemoteControllerBase(std::string name,RnpNetworkManager &networkmanager) : NRCRemoteBase<Derived>(name,networkmanager)
    {
        this->_state.newFlag(LIBRRC::COMPONENT_STATUS_FLAGS::DISARMED);//initalize disarmed
    };

protected:
    //! NRC Interface Implementation 
    void extendedCommandHandler_impl(const NRCPacket::NRC_COMMAND_ID commandID, packetptr_t packetptr)
    {
        switch (commandID)
        {
        case NRCPacket::NRC_COMMAND_ID::EXECUTE:
        {
            if (this->_state.flagSet(LIBRRC::COMPONENT_STATUS_FLAGS::NOMINAL))
            {
                static_cast<Derived *>(this)->execute_impl(std::move(packetptr));
            }
            break;
        }
        case NRCPacket::NRC_COMMAND_ID::ARM:
        {
            static_cast<Derived *>(this)->arm_impl(std::move(packetptr));
            break;
        }
        case NRCPacket::NRC_COMMAND_ID::DISARM:
        {
            static_cast<Derived *>(this)->disarm_impl(std::move(packetptr));
            break;
        }
        case NRCPacket::NRC_COMMAND_ID::TELEMETRY:
        {
            static_cast<Derived *>(this)->telemetry_impl(std::move(packetptr));
            break;
        }
        default:
        {
            break;
        }
        }
    }

    void arm_impl(packetptr_t packetptr){
        SimpleCommandPacket armingpacket(*packetptr);
        static_cast<Derived *>(this)->arm_base(armingpacket.arg);
    };

    void disarm_impl(packetptr_t packetptr){
        static_cast<Derived *>(this)->disarm_base();
    };

    void execute_impl(packetptr_t packetptr)
    {
        SimpleCommandPacket execute_command(*packetptr);
        static_cast<Derived *>(this)->execute_base(execute_command.arg);
    }

    void telemetry_impl(packetptr_t packetptr) {};


    // adaptor stuff maybe?
    void localArm_impl(int32_t arg) { static_cast<Derived *>(this)->arm_base(arg); };
    void localDisarm_impl() { static_cast<Derived *>(this)->disarm_base(); };
    void localExecute_impl(int32_t arg) { static_cast<Derived *>(this)->execute_base(arg); };
    
    void arm_base(int32_t arg)
    {
        // check for any other errors, only arm if we are error free
        if (this->_state.getStatus() == static_cast<uint16_t>(LIBRRC::COMPONENT_STATUS_FLAGS::DISARMED)){
            this->_state.deleteFlag(LIBRRC::COMPONENT_STATUS_FLAGS::DISARMED);
            this->_state.newFlag(LIBRRC::COMPONENT_STATUS_FLAGS::NOMINAL);
        }
    };

    void disarm_base()
    {
        this->_state.deleteFlag(LIBRRC::COMPONENT_STATUS_FLAGS::NOMINAL);
        this->_state.newFlag(LIBRRC::COMPONENT_STATUS_FLAGS::DISARMED);
    }

    void execute_base(int32_t arg){};

protected:
    //variables
    friend class NRCRemoteBase<Derived>; 

 
};