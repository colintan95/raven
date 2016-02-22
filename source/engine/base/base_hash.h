#ifndef BASE_HASH_H_
#define BASE_HASH_H_

#include "base_types.h"
#include "base_macros.h"

#include "hash/MurmurHash2.h"

//--------------------------------------------------
//
// Defines the hash functions
//
// Uses MurmurHash2
//
//--------------------------------------------------

const uint64_t kHashSeed = 0; // TODO: randomize this variable

uint64_t HashString(const char* str);

template<typename T>
class Hash {

public:
	uint64_t HashFunc(const T& key) {
		uint64_t res = MurmurHash64A((const void*)&key, sizeof(T), kHashSeed);

		return res;
	}
};

// Specialization for object pointers
template<typename T>
class Hash<T*> {

public:
	uint64_t HashFunc(const T* key) {
		uint64_t res = MurmurHash64A((const void*)key, sizeof(T), kHashSeed);

		return res;
	}
};


// Specialization for const char*
template<>
class Hash<const char*> {

public:
	uint64_t HashFunc(const char* key) {
		return HashString(key);
	}
};

// Specialization for char*
template<>
class Hash<char*> {

public:
	uint64_t HashFunc(char* key) {
		return HashString((const char*)key);
	}
};


#endif