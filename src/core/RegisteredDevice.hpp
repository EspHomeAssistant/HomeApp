#pragma once
#include <string>

enum class DeviceType {
    Unknown,
    Temperature,
    Relay
};

struct RegisteredDevice {
    RegisteredDevice(const std::string& machineId, const std::string& type);
    auto operator<=>(const RegisteredDevice& ) const = default;
    std::string machineId;
    DeviceType type;
};