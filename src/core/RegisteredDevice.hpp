#pragma once
#include <string>
#include "core/DeviceType.hpp"

struct RegisteredDevice {
    RegisteredDevice(const std::string& machineId, const std::string& type);
    auto operator<=>(const RegisteredDevice& ) const = default;
    std::string machineId;
    DeviceType type;
};