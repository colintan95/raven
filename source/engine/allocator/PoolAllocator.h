#ifndef POOLALLOCATOR_H_
#define POOLALLOCATOR_H_

#include "base_include.h"

//--------------------------------------------------
//
// PoolAllocator
//
// Allocates memory as chunks large enough to contain object T. Unallocated 
// chunks are tracked via a linked list embedded in the same memory space.
//
// The size of type T must be as large as the size of a pointer because a free 
// chunk will contain a pointer to the next chunk in the free list.
//
//--------------------------------------------------
template<typename T>
class PoolAllocator {

public:
	// Allocates memory for the number of objects T as specified by num
	//
	// num must be greater than 0
	explicit PoolAllocator(size_t num) {
		ASSERT(sizeof(T) >= sizeof(size_t)); // Object T must as large as size_t
		ASSERT(num > 0);

		m_Capacity = sizeof(T) * num;
		m_ChunkNum = num;
		m_ChunkSize = sizeof(T);

		m_Memory = MEM_NEW T[num];
		m_TopChunk = (T*)&m_Memory[0];
		m_FreeListHead = nullptr;
		m_MaxAddress = (size_t)m_Memory + m_Capacity;
	}

	// Deallocates memory
	~PoolAllocator() {
		m_MaxAddress = 0;
		m_FreeListHead = nullptr;
		m_TopChunk = nullptr;
		MEM_DELETE_ARR(m_Memory);

		m_ChunkSize = 0;
		m_ChunkNum = 0;
		m_Capacity = 0;
	}

	// Allocates a chunk from the free list if the list is not empty, else 
	// allocates from the top of the pool.
	T* Alloc() {
		// Allocates the dealloced chunk at the head of the free list
		if (m_FreeListHead != nullptr) {
			T* address = m_FreeListHead;
			m_FreeListHead = *((T**)m_FreeListHead); // Moves the head to the next chunk in th free list
			return address;
		}

		ASSERT((size_t)m_TopChunk < m_MaxAddress);

		// If the free list is empty, allocate from the top of the pool
		T* address = m_TopChunk;
		++m_TopChunk; 

		return address;
	}

	// Deallocates the chunk pointed to by ptr. Ptr must be a valid address
	// that points to a chunk in memory.
	//
	// Chunk is added to the front of the free list
	void Dealloc(T* ptr) {
		ASSERT((((size_t)ptr - (size_t)m_Memory) % m_ChunkSize) == 0);
		ASSERT(ptr >= m_Memory);
		ASSERT((size_t)ptr < m_MaxAddress);



		*((T**)ptr) = m_FreeListHead; // Chunk will point to the previous list head 

		m_FreeListHead = ptr;
	}

	// Deallocates all chunks
	void Clear() {
		m_TopChunk = m_Memory;
		m_FreeListHead = nullptr;
	}

	size_t GetChunkNum() const { return m_ChunkNum; }

private:
	T* m_Memory;
	size_t m_Capacity;

	// Tracks the most recent dealloced chunk.
	// Each dealloced chunk contains a pointer to the previous dealloced 
	/// chunk. The last chunk in the list will have a null pointer.
	T* m_FreeListHead;

	T* m_TopChunk;

	size_t m_MaxAddress; // Address must NOT exceed this value

	size_t m_ChunkSize;
	size_t m_ChunkNum;

private:
	// Allocator is uncopyable
	PoolAllocator(const PoolAllocator&);
	PoolAllocator& operator=(const PoolAllocator&);
};

#endif