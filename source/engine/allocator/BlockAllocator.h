#ifndef __BLOCKALLOCATOR_H__
#define __BLOCKALLOCATOR_H__

#include "base_include.h"

#include "Allocator.h"

const size_t kBlockAllocSizeMin = 128;


// The minimum allocation to free space ratio that each block must achieve to 
// be considered used
//
// Does NOT apply to blocks with no headers (e.g. blocks that stores data for
// large objects)
const double kBlockAllocLoadRatioMin = 0.8; 


struct BlockAllocHeader {
	bool partial;
	uint16_t allocCount; // Tracks the num of allocations
	uint16_t blocksUsed; // Num of blocks used to store the data of a large object (size > (blockSize - sizeof header))
	size_t blockTop; // Index to the top of the block measured in bytes. Remaining bytes = m_BlockMemSize - blockTop. 0 starts after the header

	BlockAllocHeader* prev; // Used when block is placed in a list
	BlockAllocHeader* next;
};

//--------------------------------------------------
//
// BlockAllocator
//
// Allocates memory as blocks
//
// Objects smaller than one block will be packed into a single block. An 
// object that is larger than one block will be allocated two or more blocks
// depending on the size of the object
//
// Once all objects have been deallocated from a block, the block will be 
// freed for reuse. 
//
// Allocator also supports defragmentation. Use it sparingly as it is a COSTLY
// operation
//
//--------------------------------------------------
class BlockAllocator {

public:
	// blockSize includes the size of the header and blockSize >= 
	// kBlockAllocSizeMin
	BlockAllocator(size_t blockSize, size_t blockCount);
	~BlockAllocator();

	//void* AllocAligned(size_t size, size_t alignment);
	//void DeallocAligned(void* ptr);

	void* Alloc(size_t size);
	void Dealloc(void* ptr);

	// Defragments the memory by repeatedly moving the last block to empty 
	// blocks in front of it
	//
	// COSTLY operation
	void Defragment();

public:
	size_t GetBlockMax() const { return m_BlockCount; }
	size_t GetBlockSize() const { return m_BlockSize; }
	size_t GetBlockMemSize() const { return m_BlockMemSize; }

private:

	// Allocates memory for the data of specified size. 
	//
	// If the data is small (size <= m_BlockMemSize), it will be allocated into
	// a single block. If the data is big (size > m_BlockMemSize), it will be 
	// allocated a number of contiguous blocks that, in total, are big enough 
	// to fit the object.
	void* AllocInternal(size_t size);

	// Deallocates memory that was allocated
	void DeallocInternal(void* ptr);


	// Returns the address of a single block that can fit the data
	// of the specified size
	//
	// The method first checks if a block can be allocated from the partial or
	// free list. If not, the top block in the pool is used
	void* RequestSingleBlock(size_t size);

	// Returns the address of a number of contiguous blocks that 
	// fits the data of specified size. 
	//
	// To reduce complexity, the method only allocate blocks from the top of 
	// the block pool. If there is not enough space, error. Moves 
	// m_BlockPoolTop after the allocation
	//
	// Size must be larger than m_BlockMemSize
	void* RequestContiguousBlocks(size_t size);

	// Iterates through the partial list to find a block that can fit the data
	BlockAllocHeader* AllocFromPartialList(size_t size);

	// If the free list is not empty, allocates a free block and transfers it
	// into the partial list
	BlockAllocHeader* AllocFromFreeList(size_t size);

	// Allocates a block from the top of the pool and transfers it into the partial list
	BlockAllocHeader* AllocFromTop(size_t size);


private:

	// Tests if there are partial blocks
	bool HasPartialBlocks();
	
	// Adds a new partial block into the partial list
	void PushPartialListBlock(BlockAllocHeader* header);

	// Removes the specified header
	void RemovePartialListBlock(BlockAllocHeader* header);


private:
	// Tests if the free list has blocks
	bool HasFreeBlocks();

	// Adds an empty block into the free list
	void PushFreeListBlock(BlockAllocHeader* header);

	// Returns the top free list block
	//
	// Moves the free list head to the next block, or set to nullptr if there 
	// are no more freed blocks
	BlockAllocHeader* PopFreeListBlock();


private:
	// Calculates the memory remaining in the specified block
	size_t CalcBlockMemRemaining(BlockAllocHeader* header);

	// Returns the address of the block at the specified index
	size_t GetBlockAddress(size_t index);

private:
	size_t m_BlockSize; // Includes the header
	size_t m_BlockCount;
	size_t m_BlockMemSize; // Does NOT include the header

	size_t m_BlockPoolTop; // Index to the top block in the pool - all blocks following this are empty 

	// Free list tracks all blocks that are available for reuse d
	BlockAllocHeader* m_FreeListHead;

	// Tracks all partial blocks - blocks that have yet to reach the 
	// the minimum load. These blocks must have headers and must not be at the
	// top of the block pool
	BlockAllocHeader* m_PartialListHead;

	// Minimum load size that each block must achieve
	size_t m_BlockLoadMin; 

	byte_t* m_Memory;
};

#endif