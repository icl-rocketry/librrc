#pragma once
#include <functional>
#include <string>

namespace RRCLog{
    using LogCallback_t = std::function<void(const std::string&)>;
}