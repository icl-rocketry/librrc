#pragma once

#include "nrcremotesensorbase.h"

#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>
#include <librrc/packets/ptapcalibrationpacket.h>

#ifdef HAVE_ADS1219
#include "ADS1219.h"
#endif

template <typename Derived>

class NRCRemotePTap : public NRCRemoteSensorBase<Derived>
{
    public:


        NRCRemotePTap(RnpNetworkManager& networkmanager,
                    ADS1219 &ADS,
                    uint8_t adc_channel
                    ) : 
            NRCRemoteSensorBase(networkmanager),
            _ADS(ADS),
            _adc_channel(adc_channel)
        {};

        uint32_t getValue();

    protected:

        friend class NRCRemoteSensorBase;
        ADS1219 &_ADS;
        uint8_t _adc_channel;
        float _c = 0;
        float _grad = 1;
        float _resistance = 1;

        void calibrate(float c, float grad, float resistance);
        void calibrate_impl(packetptr_t packetptr);     
};