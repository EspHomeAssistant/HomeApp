#include <gmock/gmock.h>

#include "api/Signal.hpp"
#include "core/Registration.hpp"
#include "core/RegisteredDevice.hpp"

class RegistrationMock : public IRegistration {
public:
    MOCK_METHOD(void, handleRegistration, (const std::string&));
    MOCK_METHOD(Signal<RegisteredDevice>&, onRegistration, ());
};