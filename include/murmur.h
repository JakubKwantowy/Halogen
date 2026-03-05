#ifndef MURMUR_H
#define MURMUR_H

#include <common.h>

#define MURMUR_DEFAULTSEED 0x9747b28c
#define MURMUR_strhash_noseed(key) MURMUR_strhash((key), MURMUR_DEFAULTSEED)

extern INT_t MURMUR_hash(const char *key, size_t n, INT_t seed);
extern INT_t MURMUR_strhash(const char *key, INT_t seed);

#endif
