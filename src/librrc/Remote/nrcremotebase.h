/**
 * @file NRCRemoteBase.h
 * @author Kiran de Silva
 * @brief Base class for the Networked Rocket Component (NRC) handler. Defines the basic required
 * command processing for NRC type components. Uses CRTP to reduce runtime polymorphism overhead 
 * as it is not expected to cast to parent class ever. SHould be straightforward to change in 
 * the future or simply provide a runtime polymorphic derived class from this. Extended command handler 
 * implemented if a more complex component is required.
 * @version 0.1
 * @date 2022-07-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include <functional>
#include <string>

#include <librrc/componentstatusflags.h>
#include <librrc/Helpers/bitwiseflagmanager.h>
#include <librrc/Packets/nrcpackets.h>
#include <librrc/Interface/rocketcomponent.h>

#include <librnp/default_packets/simplecommandpacket.h>
#include <librnp/rnp_networkmanager.h>


template<class Derived>
class NRCRemoteBase
{
public:
    NRCRemoteBase(std::string name,RnpNetworkManager& networkmanager):
    _networkmanager(networkmanager),
    _name(name)
    {};
    /*
     * @brief Returns network callback bound to instance
     *
     * @return std::function<void(packetptr_t)>
     */
    std::function<void(packetptr_t)> getThisNetworkCallback()
    {
        return [this](packetptr_t packetptr)
        { handlecommand(std::move(packetptr)); };
    };


    LIBRRC::component_status_flags_t getStatus() const {return _state.getStatus();};
    int32_t getValue() const {return _value;};

protected:

    RnpNetworkManager& _networkmanager;
    RocketComponent::RocketComponentState _state;
    int32_t _value;
    const std::string _name;

    void handlecommand(packetptr_t packetptr){
        //check packet is a command packet
        // if (packetptr->header.type == static_cast<uint8_t>(NRCPacket::TYPES::NRC_COMMAND)){
            
            const NRCPacket::NRC_COMMAND_ID commandID = static_cast<NRCPacket::NRC_COMMAND_ID>(CommandPacket::getCommand(*packetptr));
           
            switch(commandID){
                case NRCPacket::NRC_COMMAND_ID::GETSTATE:
                {
                    static_cast<Derived*>(this)->getstate_impl(std::move(packetptr));
                    break;
                }
                case NRCPacket::NRC_COMMAND_ID::CALIBRATE:
                {
                    static_cast<Derived*>(this)->calibrate_impl(std::move(packetptr));
                    break;
                }
                default:
                {
                    static_cast<Derived*>(this)->extendedCommandHandler_impl(commandID,std::move(packetptr));
                    break;
                }
            }

        // }
    }

    //!NRCRemote Interface
    /**
     * @brief Handle getstate requests
     * 
     * @param packetptr 
     */
    void getstate_impl(packetptr_t packetptr)
    {
        static_cast<Derived*>(this)->updateState_base();
        SimpleCommandPacket getstate_request(*packetptr);
        NRCStatePacket getstate_response;
        RnpHeader::generateResponseHeader(getstate_request.header,getstate_response.header);
        getstate_response.state = _state.getStatus();
        getstate_response.value = _value;
        _networkmanager.sendPacket(getstate_response);
    };
    
    /**
     * @brief Calibrate component. Does nothing by default
     *
     * @param packetptr
     */
    void calibrate_impl(packetptr_t packetptr){};

    void extendedCommandHandler_impl(const NRCPacket::NRC_COMMAND_ID commandID,packetptr_t packetptr){};

    //!Local Component Interface

    /**
     * @brief Update component state local interface implmentation, can often be left empty if the
     * underlying nrc component handles updating its value on its own and doesnt require a trigger. For example
     * event based components like the solneoid valve where the possible states are open or closed, and this is updated
     * when a new executor command is called.
     * A good example where this method should be implmeneted is in the pyro class where we need to update the continuity 
     * state as close to the original request time and we want to know exactly how long the pyro will be on for as possible to keep the data valid.
     *
     */
    void localUpdateState_impl()
    {
        static_cast<Derived*>(this)->updateState_base();
    }

    //! Base methods

    /**
     * @brief Update hook to update state in the case of polling. If event based or component is handling polling itself, this can 
     * be left empty, and is empty by default.
     * 
     */
    void updateState_base(){};
     
};