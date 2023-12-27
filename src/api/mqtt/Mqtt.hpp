#include <string>
#include <memory>

namespace spdlog 
{
    class logger;
}

class Mqtt {
public:
    Mqtt(const std::string& broker, std::shared_ptr<spdlog::logger> logger);
    ~Mqtt();
    Mqtt(const Mqtt&) = delete;
    Mqtt(Mqtt&&) = delete;
    Mqtt& operator=(const Mqtt&) = delete;
    Mqtt& operator=(Mqtt&&) = delete;

    std::shared_ptr<spdlog::logger> getLogger();

private:
    const std::string broker_;
    std::shared_ptr<spdlog::logger> logger_;
};