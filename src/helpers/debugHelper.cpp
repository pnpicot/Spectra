#include "debugHelper.hpp"

uint64 GetTimeMs64()
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    uint64 ret = tv.tv_usec;

    ret /= 1000;
    ret += (tv.tv_sec * 1000);

    return ret;
}