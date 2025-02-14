// #pragma once

// /**
//  * @file Potentiometer.h
//  * @author Rassa. The Chosen One. You know how we (don't) do it you know how we get downnnnn.
//  * @brief Class that (doesn't) read potentiometer angle 
//  * @version 0.1
//  * @date 11/06/2024
// **/

// #include <librrc/Remote/nrcremotesensorbase.h>
// #include <librnp/rnp_networkmanager.h>
// #include <libriccore/platform/esp32/ADC.h>
// #include <libriccore/filtering/movingAvg.h>
// #include <stdint.h>
// #include <libriccore/riccorelogging.h>

// #include <librrc/Helpers/nvsstore.h>
// #include <librrc/Packets/potentiometercalibrationpacket.h>

// class nrcremotepotentiometer : public NRCRemoteSensorBase<nrcremotepotentiometer>
// {

// public: 
//     nrcremotepotentiometer(RnpNetworkManager &netman, uint8_t m_gpioSig, uint8_t m_potentindex, float m_zero_angl = 0, float m_max_angl = 360) : NRCRemoteSensorBase(netman), adc(m_gpioSig), avg(m_average_samples), potentindex(m_potentindex), zero_angl(m_zero_angl), max_angl(m_max_angl){loadCalibration();};


//     void setup(){
//         adc.setup();
//     };

//     void update();

//     float getValue(){return m_avg_potentang;}

//     void calibrate_impl(packetptr_t packetptr);

// private:
//     ADC adc;
//     MovingAvg avg;
//     uint8_t m_average_samples = 10;
//     uint8_t potentindex;
//     int16_t adccount;
//     float zero_angl;
//     float max_angl;
//     float potentang;
//     float m_avg_potentang;
//     void loadCalibration();
// };