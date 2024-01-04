#pragma once

#include "rocketcomponent.h"

class NetworkComponentState : public RocketComponent::RocketComponentState{

    public:

        /**
         * @brief Allows Component State to track a remote components state over the network
         * 
         * @param remoteState 
         */
        void trackRemoteState(LIBRRC::component_status_flags_t remoteState)
        {
            _statusString = remoteState;
        }

};