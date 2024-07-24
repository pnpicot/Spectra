#include "randomHelper.hpp"

namespace sp {
    int RandomHelper::randInt(int min, int max)
    {
        return min + (rand() % static_cast<int>(max - min + 1));
    }

    float RandomHelper::randFloat(float min, float max)
    {
        return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
    }
}