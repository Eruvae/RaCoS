#ifndef MURMUR_H
#define MURMUR_H

#include "rodos.h"

class Murmur
{
    static uint32_t rotl32 (uint32_t x, int8_t r);
    static uint32_t fmix32(uint32_t h);

public:
    Murmur();
    static uint32_t mm_hash_32(const uint8_t *data, int len, uint32_t seed = 0x1337);
};

#endif // MURMUR_H
