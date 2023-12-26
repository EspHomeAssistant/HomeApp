#include <string>

class Mqtt {
public:
    Mqtt(const std::string& ipAddr);
    ~Mqtt();
    Mqtt(const Mqtt&) = delete;
    Mqtt(Mqtt&&) = delete;
    Mqtt& operator=(const Mqtt&) = delete;
    Mqtt& operator=(Mqtt&&) = delete;
};