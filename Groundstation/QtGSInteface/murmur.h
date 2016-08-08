#ifndef MURMUR_H
#define MURMUR_H

#include <QObject>

class Murmur : public QObject
{
    static uint32_t rotl32 (uint32_t x, int8_t r);
    static uint32_t fmix32(uint32_t h);

public:
    static uint32_t mm_hash_32(const uint8_t *data, int len, uint32_t seed = 0x1337);
};

#endif // MURMUR_H
