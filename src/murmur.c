#include <string.h>
#include <murmur.h>

static inline INT_t MURMUR_scramble(INT_t k) {
    k *= 0xcc9e2d51;
    k = (k << 15) | (k >> 17);
    k *= 0x1b873593;
    return k;
}

INT_t MURMUR_hash(const char *key, size_t n, INT_t seed) {
    INT_t h = seed;
    INT_t k;

    for(size_t i = n >> 2; i; i--) {
        memcpy(&k, key, sizeof(k));
        key += sizeof(k);
        h ^= MURMUR_scramble(k);
        h = (h << 13) | (h >> 19);
        h = h * 5 + 0xe6546b64;
    }

    k = 0;
    for(size_t i = n & 3; i; i--) {
        k <<= 8;
        k |= key[i-1];
    }

    h ^= MURMUR_scramble(k);
    h ^= n;
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}

INT_t MURMUR_strhash(const char *key, INT_t seed) {
    return MURMUR_hash(key, strlen(key), seed);
}
