#pragma once

#include "nrcremotebase.h"
#include <rnp_networkmanager.h>
#include "rocketcomponent.h"

template <typename Derived>

class NRCRemoteSensorBase : public NRCRemoteBase<Derived>
{
    public:
        NRCRemoteSensorBase(RnpNetworkManager& networkmanager) : NRCRemoteBase<Derived>(networkmanager)
        {
            this->_state.newFlag(COMPONENT_STATUS_FLAGS::NOMINAL); // initlaize sensor to nominal state
        };
    
};