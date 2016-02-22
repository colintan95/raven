#ifndef FRAMEALLOCATOR_H_
#define FRAMEALLOCATOR_H_

#include "base_include.h"

//--------------------------------------------------
//
// FrameAllocator
//
// Allocates memory at the top of the allocator. Allocator is of fixed size and
// will not resize if the limit is reached
//
// Suitable for single use buffers or to store permanent objects that will only
// be removed at the end of the game
//
//--------------------------------------------------
class FrameAllocator {

public:
	explicit FrameAllocator(size_t capacity);
	~FrameAllocator();

	// Allocates memory of the specified size with the correct alignment
	//
	// Alignment must be a power of 2 (usually 4 or 16) and <= 32
	void* AllocAligned(size_t size, size_t alignment);

	// Allocates memory of the specified size
	void* Alloc(size_t size);

	// Clears the entire stack
	void Clear();

	size_t GetSize() const { return m_Size; }
	size_t GetCapacity() const { return m_Capacity; }

private:
	void* AllocInternal(size_t size);

private:
	byte_t* m_Memory;

	// Location of the top in memory
	size_t m_TopAddr;
	// Current number of bytes being used
	size_t m_Size;

	// Total number of bytes of memory that can be allocated
	size_t m_Capacity;
};

#endif