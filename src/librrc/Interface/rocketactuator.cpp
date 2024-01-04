#include "rocketactuator.h"

RocketActuator::~RocketActuator(){};

void RocketActuator::arm(){
    this->arm(defaultArmArg);
}

int32_t RocketActuator::getValue()
{
    return currentValue;
}