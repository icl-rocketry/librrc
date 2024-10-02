#pragma once

/**
 * @file nrcremoteptap.h
 * @author Andrei Paduraru
 * @brief Class to read an analog pressure transducer.
 * @version 0.2
 * @date 2024-08-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <librrc/Remote/nrcremotesensorbase.h>

#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>

#include <librrc/Helpers/nvsstore.h>
#include <librrc/Packets/ptapcalibrationpacket.h>

class NRCRemotePTap : public NRCRemoteSensorBase<NRCRemotePTap>
{
    public:


        NRCRemotePTap(RnpNetworkManager& networkmanager,
                    uint8_t PTAPindex,
                    float c = 1,
                    float grad = 1
                    ) : 
            NRCRemoteSensorBase(networkmanager),
            _ptapIndex(PTAPindex),
            _c(c),
            _grad(grad)
        {loadCalibration();};

        float getConst(){return _c;};

        float getGrad(){return _grad;};

        void calibrate_impl(packetptr_t packetptr);

        float pressure;

        void update(int32_t ADCReading);
        void setup(){loadCalibration();};
        
        float getPressure(){return _Pressure;};

    protected:

        friend class NRCRemoteSensorBase<NRCRemotePTap>;
        uint8_t _ptapIndex;
        float _c = 0;
        float _grad = 1;
        float _Pressure;

        void loadCalibration();

        
        

};