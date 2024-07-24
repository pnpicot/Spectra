#pragma once

#include <iostream>

namespace sp {
    class Logger {
        public:
            enum Level {
                INFO,
                WARNING,
                ERROR
            };

        public:
            static void log(Logger::Level level, const std::string &message);
    };
}