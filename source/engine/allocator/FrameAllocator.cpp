#include "FrameAllocator.h"

#include "Allocator.h"

FrameAllocator::FrameAllocator(size_t capacity) {
	ASSERT(capacity > 0);

	m_Memory = new byte_t[capacity];

	m_Capacity = capacity;

	m_TopAddr = (size_t)m_Memory;
	m_Size = 0;
}

FrameAllocator::~FrameAllocator() {
	m_Size = 0;
	m_TopAddr = 0;

	m_Capacity = 0;

	delete[] m_Memory;
}

void* FrameAllocator::AllocAligned(size_t size, size_t alignment) {
    byte_t offset = CalcAlignedOffset(m_TopAddr, alignment);

	size_t newSize = size + offset;

	void* addr = AllocInternal(newSize);
    
    addr = (void*)((size_t)addr + offset);

	// Stores the offset information 1 byte before the returned memory address
    byte_t* byteAddr = (byte_t*)addr;
    --byteAddr;
    *byteAddr = offset;

	return addr;
}

void* FrameAllocator::Alloc(size_t size) {
	return AllocInternal(size);
}

void* FrameAllocator::AllocInternal(size_t size) {
	size_t returnAddr = m_TopAddr;

    m_TopAddr += size;

    m_Size += size;

	return (void*)returnAddr;
}


void FrameAllocator::Clear() {
	m_TopAddr = (size_t)m_Memory;
	m_Size = 0;
}

