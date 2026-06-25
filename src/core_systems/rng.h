#pragma once

#include <stdint.h>

namespace mouli::random
{
    inline uint32_t rngSeed = 123456789;

    inline uint32_t GetRandom()
    {
        uint32_t x = rngSeed;

        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;

        rngSeed = x;
        return x;
    }
}