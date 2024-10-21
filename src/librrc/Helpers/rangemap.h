#pragma once
//stolen shamelessly from arduino but now templated and wiht bounds checking, linear interpolation between 2 ranges
namespace LIBRRC{
template<class T>
T rangemap(T x, T in_min, T in_max, T out_min, T out_max) {
    T out = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    
    return std::max(std::min(out,out_max),out_min);
};
};