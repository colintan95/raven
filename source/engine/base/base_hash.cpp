#include "base_hash.h"

#include "hash/MurmurHash2.h"

uint64_t HashString(const char* str) {
	uint64_t res = MurmurHash64A((const void*)str, strlen(str), kHashSeed);

	return res;
}