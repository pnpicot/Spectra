#pragma once

#include <random>

namespace sp {
    class RandomHelper {
        public:
            static int randInt(int min, int max);
            static float randFloat(float min, float max);
    };
}