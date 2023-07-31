#pragma once

#include <Preferences.h>
#include <map>

class NRCNVSCalibration{
    public:
        NRCNVSCalibration(std::string NVSName, std::map<std::string,int> constsMap);

        void saveConsts(){
            _NVS.begin(_NVSName);

            for(uint16_t i; i < _constsMap.size(); i++){
                
            }

            _NVS.end();
        };

        void loadConsts();

        void erase();

    private:
        Preferences _NVS;
        char* _NVSName;

        std::map<std::string,int> _constsMap;

        //overloads depending on type of the value
        void nvsPut(std::string Key, float value){
            char* key = static_cast<char*>(Key); 
            _NVS.putFloat(key, value);
        }

        void nvsPut(std::string Key, int value){
            char* key = static_cast<char*>(Key); 
            _NVS.putInt(key, value);
        }

        void nvsPut(std::string Key, unsigned int value){
            char* key = static_cast<char*>(Key); 
            _NVS.putUInt(key, value);
        }

        void nvsPut(std::string Key, bool value){
            char* key = static_cast<char*>(Key); 
            _NVS.putBool(key, value);
        }

        void nvsPut(std::string Key, short value){
            char* key = static_cast<char*>(Key); 
            _NVS.putShort(key, value);
        }

        void nvsPut(std::string Key, unsigned short value){
            char* key = static_cast<char*>(Key); 
            _NVS.putUShort(key, value);
        }

};