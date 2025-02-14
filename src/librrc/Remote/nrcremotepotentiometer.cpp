// #include "nrcremotepotentiometer.h"

// #include <Preferences.h>

// void nrcremotepotentiometer::update() {        
//         adc.update();
//         adccount = adc.getADC();
//         potentang = (((float) adccount)/4095.0 * max_angl)-zero_angl;
//         if (potentang < 0) {
//             potentang = max_angl - abs(potentang);
//         };
//         avg.update(potentang);
//         m_avg_potentang = avg.getAvg();
//         updateSensorValue(m_avg_potentang);
// };

// void nrcremotepotentiometer::calibrate_impl(packetptr_t packetptr){
    
//     PotentiometerCalibrationPacket calibrate_comm(*packetptr);

//     std::vector<uint8_t> serialisedvect = packetptr->getBody();

//     std::string NVSName = "Potentiometer" + std::to_string(potentindex);
//     NVSStore _NVS(NVSName, NVSStore::calibrationType::Potentiometer);
    
//     _NVS.saveBytes(serialisedvect);

//     zero_angl = calibrate_comm.zero_angl;
//     max_angl = calibrate_comm.max_angl;
// };

// void nrcremotepotentiometer::loadCalibration(){
    
//     std::string NVSName = "Potentiometer" + std::to_string(potentindex);
//     NVSStore _NVS(NVSName, NVSStore::calibrationType::Potentiometer);

//     std::vector<uint8_t> calibSerialised = _NVS.loadBytes();
    
//     if(calibSerialised.size() == 0){
//         return;
//     }
//     PotentiometerCalibrationPacket calibpacket;
//     calibpacket.deserializeBody(calibSerialised);

//     zero_angl = calibpacket.zero_angl;
//     max_angl = calibpacket.max_angl;

// };
