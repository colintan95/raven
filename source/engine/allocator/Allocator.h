#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include "base_include.h"

#include <stdlib.h>

//--------------------------------------------------
//
// Allocator.h
//
// Allocator interface and utility functions used by all allocators
//
//--------------------------------------------------


const int kAllocatorDefaultAlignment = 16;

//--------------------------------------------------
//
// IAllocator
//
// Interface for all memory allocators
//
//--------------------------------------------------
class IAllocator {

public:
	virtual ~IAllocator() {}

	virtual void AllocAligned(size_t size, size_t alignment) = 0;

	virtual void Alloc(size_t size) = 0;
};

// Returns the num of bytes needed to offset the address so that it is 
// aligned
//
// Alignment must be a power of 2 (usually 4 or 16) and below 32
inline byte_t CalcAlignedOffset(size_t address, size_t alignment) {
	ASSERT(2 <= alignment && alignment <= 32);

	return (byte_t)(alignment - (address & (alignment - 1)));
}


#endif