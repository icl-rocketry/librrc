#pragma once

#include "nrcremotesensorbase.h"

#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>

#ifdef HAVE_MAX31856
#include "MAX31856.h"
#endif

template <typename Derived>

class NRCRemoteThermocouple : public NRCRemoteSensorBase<Derived>
{
    public:

        NRCRemoteThermocouple(RnpNetworkManager& networkmanager,
                    MAX31856 &MAX) : 
                NRCRemoteSensorBase(networkmanager),
                _ADS(MAX)
            {};
        
        float getValue(){return _MAX.getTemp()};
    
    protected:

        friend class NRCRemoteSensorBase;
        MAX31856 &_MAX;
        
};
