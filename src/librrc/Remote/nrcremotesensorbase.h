#pragma once

#include "nrcremotebase.h"

#include <string>

#include <librnp/rnp_networkmanager.h>
#include <librrc/componentstatusflags.h>

template <typename Derived>

class NRCRemoteSensorBase : public NRCRemoteBase<Derived>
{
    public:
        NRCRemoteSensorBase(std::string name, RnpNetworkManager& networkmanager) : NRCRemoteBase<Derived>(name,networkmanager)
        {
            this->_state.newFlag(LIBRRC::COMPONENT_STATUS_FLAGS::NOMINAL); // initlaize sensor to nominal state
        };

         NRCRemoteSensorBase(RnpNetworkManager& networkmanager) : NRCRemoteSensorBase("",networkmanager){};

    protected:
        void updateSensorValue(float value)
        {
            this->_value = *reinterpret_cast<int32_t*>(&value); // convert float to int32_t bytes
        }
};