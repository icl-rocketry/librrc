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
                    uint8_t adc_channel,
                    uint32_t grad = 1,
                    uint32_t c = 0,
                    uint32_t resistance = 1
                    ) : 
            NRCRemoteSensorBase(networkmanager),
            _ADS(ADS),
            _adc_channel(adc_channel),
            _grad(grad),
            _c(c),
            _resistance(resistance)
        {};


        NRCRemotePTap(RnpNetworkManager& networkmanager,
                    ADS1219 &ADS,
                    uint8_t adc_channel,
                    uint32_t grad,
                    uint32_t c,
                    ) : 
            NRCRemoteSensorBase(networkmanager),
            _ADS(ADS),
            _adc_channel(adc_channel),
            _grad(grad),
            _c(c),
            _resistance(0)
        {};

        uint32_t getValue();

    protected:

        friend class NRCRemoteSensorBase;
        ADS1219 &_ADS;
        uint8_t _adc_channel;
        float _c;
        float _grad;
        float _resistance;

        void calibrate_impl(packetptr_t packetptr);     
};