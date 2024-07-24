#include "logger.hpp"

namespace sp {
    void Logger::log(Logger::Level level, const std::string &message)
    {
        switch (level) {
            case INFO:
                std::cout << "\033[1;34m";
                break;
            case WARNING:
                std::cout << "\033[1;33m";
                break;
            default:
                std::cout << "\033[1;31m";
                break;
        }

        std::cout << message << std::endl;

        if (level == ERROR)
            throw new std::runtime_error(message.c_str());
    }
}