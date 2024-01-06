#pragma once

#include <stdint.h>
#include <type_traits>

namespace LIBRRC{

    enum class COMPONENT_STATUS_FLAGS:uint16_t{
        NOMINAL = (1<<0),
        DISARMED = (1<<1),
        ERROR_NORESPONSE = (1<<2), //Deprecated
        ERROR_CONTINUITY = (1<<3),
        ERROR_PINS = (1<<4),
        ERROR_I2C = (1<<5),
        ERROR = (1<<6) //generic error
    };

    using component_status_flags_t = typename std::underlying_type<COMPONENT_STATUS_FLAGS>::type;

};