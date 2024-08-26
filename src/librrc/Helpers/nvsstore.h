#pragma once

#include <Preferences.h>
#include <librnp/rnp_packet.h>
#include <functional>
#include <vector>
#include <string>
#include <memory>

class NVSStore
{
public:

    enum class calibrationType : uint8_t
    {
        LoadCell = 0,
        PTap = 1,
        Thermistor = 2,
        Thermocouple = 3,
        Servo = 4,
        Canard = 5,
        CapFill = 6,
        Potentiometer = 7,
        Solenoid = 8
    };
    
    NVSStore(std::string NVSName, NVSStore::calibrationType calibrationType):
    _NVSName(NVSName),
    _calibtype(calibrationType)
    {};

    void saveBytes(std::vector<uint8_t> Bytes)
    {
        _NVS.begin(_NVSName.c_str());

        uint16_t calibLen = Bytes.size() * sizeof(uint8_t);

        _NVS.putUChar("Type",(uint8_t) _calibtype);

        _NVS.putBytes("SerialConsts", Bytes.data(), calibLen);
        //nvsPut("Checksum", genCRCremainder(Bytes, 0b00001, 0));

        _NVS.end();
    };

    std::vector<uint8_t> loadBytes()
    {
        _NVS.begin(_NVSName.c_str(), true);

        if(_NVS.getUChar("Type", 200) != (uint8_t) _calibtype){
            //throw an error here but haven't implemented that yet
           std::vector<uint8_t>emptyvect(0);
           return emptyvect;
        }

        uint8_t vectlen = _NVS.getBytesLength("SerialConsts");
        std::vector<uint8_t> bytesVector(vectlen);
        
        _NVS.getBytes("SerialConsts", bytesVector.data(), vectlen);
        _NVS.end();

        return bytesVector;
    };

    void erase();

private:
    Preferences _NVS;
    std::string _NVSName;

    // var stores the serivce type to calibrate for
    calibrationType _calibtype;

    
    /**
    * @brief method to generate a CRC checksum. Look at https://en.wikipedia.org/wiki/Cyclic_redundancy_check for more info.
    * 
    * @param datavect Vector of data to use for checksum generation.
    * @param CRCpoly  Generator polynomial to use. Note that this since this is defined as uint32, this method can at most implement CRC-31
    * @param fillervar The filler variable to use as the initial remainder which is added to the end of the data. This should be either 1 or 0.
    * 
    * @return This method will return the CRC remainder, which is also the checksum.
    */

    // uint32_t genCRCremainder(std::vector<uint8_t> datavect, uint32_t CRCpoly, bool fillervar){
    //     uint32_t poly = CRCpoly;


    //     uint32_t remainder;
    //     return remainder;
    // };

    bool checkCRC();
    
    // overloads depending on type of the value
    void nvsPut(std::string valKey, float value)
    {
        char *key = valKey.data();
        _NVS.putFloat(key, value);
    }

    void nvsPut(std::string valKey, int value)
    {
        char *key = valKey.data();
        _NVS.putInt(key, value);
    }

    void nvsPut(std::string valKey, unsigned int value)
    {
        char *key = valKey.data();
        _NVS.putUInt(key, value);
    }

    void nvsPut(std::string valKey, bool value)
    {
        char *key = valKey.data();
        _NVS.putBool(key, value);
    }

    void nvsPut(std::string valKey, short value)
    {
        char *key = valKey.data();
        _NVS.putShort(key, value);
    }

    void nvsPut(std::string valKey, unsigned short value)
    {
        char *key = valKey.data();
        _NVS.putUShort(key, value);
    }
};