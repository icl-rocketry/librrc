#include "rocketsensor.h"

#include <memory>

#include <librnp/rnp_networkmanager.h>



RocketSensor::~RocketSensor(){};

float RocketSensor::getValue()
{
    return sensorValue;
}