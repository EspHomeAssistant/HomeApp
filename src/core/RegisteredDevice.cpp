#include "RegisteredDevice.hpp"
#include <algorithm>

RegisteredDevice::RegisteredDevice(const std::string &machineId, const std::string &type)
    : machineId {machineId}
{
    std::string lower;
    std::ranges::transform(type, std::back_inserter(lower), [](const char c){ return std::tolower(c);});
    if (lower == "temperature") {
        this->type = DeviceType::Temperature; 
    } else if (lower == "relay") {
        this->type = DeviceType::Relay;
    } else {
        this->type = DeviceType::Unknown;
    }
}
