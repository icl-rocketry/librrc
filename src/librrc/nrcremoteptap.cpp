#include "nrcremoteptap.h"

uint32_t NRCRemotePTap::getValue()
{
    return (_ADS.readAdjusted(_adc_channel)/_resistance - _c)/_grad;
}