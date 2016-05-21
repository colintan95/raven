#include "BlockAllocator.h"

#include <cstring>

BlockAllocator::BlockAllocator(size_t blockSize, size_t blockCount) {
	ASSERT(blockSize >= kBlockAllocSizeMin);
	ASSERT(blockCount > 0);

	m_BlockSize = blockSize;
	m_BlockCount = blockCount;
	m_BlockMemSize = m_BlockSize - sizeof(BlockAllocHeader);

	m_BlockPoolTop = 0;
	m_FreeListHead = nullptr;
	m_PartialListHead = nullptr;

	m_BlockLoadMin = (size_t)((double)m_BlockMemSize * kBlockAllocLoadRatioMin);

	m_Memory = MEM_NEW byte_t[blockSize * blockCount];

	memset((void*)m_Memory, 0, m_BlockSize * m_BlockCount);
}

BlockAllocator::~BlockAllocator() {
	m_PartialListHead = nullptr;
	m_FreeListHead = nullptr;
	MEM_DELETE_ARR(m_Memory);
}

void* BlockAllocator::Alloc(size_t size) {
	return AllocInternal(size);
}

void BlockAllocator::Dealloc(void* ptr) {
	return DeallocInternal(ptr);
}

void* BlockAllocator::AllocInternal(size_t size) {
	ASSERT(size > 0);

	void* address = nullptr;

	if (size <= m_BlockMemSize) {
		address = RequestSingleBlock(size);
	}
	else {
		address = RequestContiguousBlocks(size);
	}

	return address;
}

void* BlockAllocator::RequestSingleBlock(size_t size) {
	ASSERT(size <= m_BlockMemSize);

	BlockAllocHeader* header = nullptr;

	header = AllocFromPartialList(size);

	if (header == nullptr) {
		header = AllocFromFreeList(size);
	}
    
	if (header == nullptr) {
		header = AllocFromTop(size);
	}

	ASSERT(header != nullptr);

    size_t returnAddr = (size_t)header + sizeof(BlockAllocHeader) + header->blockTop;

    header->allocCount += 1;
    header->blocksUsed = 0;
    header->blockTop += size;

	return (void*)returnAddr;
}

BlockAllocHeader* BlockAllocator::AllocFromPartialList(size_t size) {
	if (!HasPartialBlocks()) { 
		return nullptr;
	}

	BlockAllocHeader* header = nullptr;

	BlockAllocHeader* headerIt = m_PartialListHead;

	while (headerIt != nullptr) {
		size_t blockMemRemaining = CalcBlockMemRemaining(headerIt);

		// Uses the block if there is enough space for the data
		if (size <= blockMemRemaining) {
			header = headerIt;
            break;
		}

		// Removes the block from the partial list if the block has reached
		// its minimum load requirement
		if (blockMemRemaining < m_BlockLoadMin) {
			BlockAllocHeader* removedHeader = headerIt;

			// Removes the block from the list
			headerIt = headerIt->prev;

			RemovePartialListBlock(removedHeader);
		}
		else {
			// Moves to the next block
			headerIt = headerIt->prev;
		}
	}

	return header;
}

BlockAllocHeader* BlockAllocator::AllocFromFreeList(size_t size) {
	ASSERT(size <= m_BlockMemSize);

	if (!HasFreeBlocks()) {
		return nullptr;
	}

	// Inserts a new free block into the partial block
	BlockAllocHeader* header = PopFreeListBlock();

	header->allocCount = 0;
	header->blocksUsed = 0;
	header->blockTop = 0;
	header->prev = nullptr;
    header->next = nullptr;

    PushPartialListBlock(header);

    return header;
}

BlockAllocHeader* BlockAllocator::AllocFromTop(size_t size) {
	ASSERT(m_BlockPoolTop < m_BlockCount);
	ASSERT(size <= m_BlockMemSize);

	// Allocate from the top of the block pool
	size_t headerAddr = GetBlockAddress(m_BlockPoolTop);

	BlockAllocHeader* header = (BlockAllocHeader*)headerAddr;

	header->allocCount = 0;
	header->blocksUsed = 0;
	header->blockTop = 0;
	header->prev = nullptr;
	header->next = nullptr;
	
	// Push the top block into the partial list
	PushPartialListBlock(header);

	++m_BlockPoolTop; // Move the top to the next block

	return header;
}

void* BlockAllocator::RequestContiguousBlocks(size_t size) {
	ASSERT(m_BlockPoolTop < m_BlockCount);
	ASSERT(size > m_BlockMemSize);

	// Remaining number of blocks in the pool
	size_t blocksRemaining = m_BlockCount - m_BlockPoolTop;

	// Calculates the maximum size that the pool can allocate
	// First block must be of size m_BlockMemSize; the rest will be of size 
	// m_BlockSize since they have no headers
	size_t sizeMax = m_BlockMemSize + (blocksRemaining - 1) * m_BlockSize;

	ASSERT(size <= sizeMax);

	
	// The size of all additional blocks after excluding the header block
	size_t addBlocksSize = size - m_BlockMemSize; 

	// Calculates the number of contiguous blocks to allocate
	size_t allocBlocksCount = 1; // Starts as 1 as it includes the header block
	
	if ((addBlocksSize % m_BlockSize) == 0) {
		allocBlocksCount += (addBlocksSize / m_BlockSize);
	}
	else {
		// Adds 1 since addBlocksSize / m_BlockSize rounds down
		allocBlocksCount += (addBlocksSize / m_BlockSize + 1);
	}

	ASSERT(allocBlocksCount <= blocksRemaining);


	// Address to header of first block
	size_t headerAddr = GetBlockAddress(m_BlockPoolTop);

	// Edit the header of the block
	BlockAllocHeader* header = (BlockAllocHeader*)headerAddr;
	header->allocCount = 1;
	header->blocksUsed = allocBlocksCount;
    header->blockTop = 0;
	header->prev = nullptr;
	header->next = nullptr;

	// Move the top block
	m_BlockPoolTop += allocBlocksCount;


	size_t returnAddr = headerAddr + sizeof(BlockAllocHeader);

	return (void*)returnAddr;
}

void BlockAllocator::DeallocInternal(void* ptr) {
	// Asserts if the ptr is within the memory of the allocator
	ASSERT((size_t)ptr >= (size_t)m_Memory);
	ASSERT((size_t)ptr < ((size_t)m_Memory + m_BlockSize * m_BlockCount));

	// Relative position of the ptr from the start of m_Memory
	size_t ptrMemLoc = (size_t)ptr - (size_t)m_Memory;

	// Gets the block that the ptr is in
	size_t ptrBlockIndex = ptrMemLoc / m_BlockSize;

	// Calculates the location of the ptr in the block 
	size_t ptrLocInBlock = ptrMemLoc - ptrBlockIndex * m_BlockSize;

	// Asserts that the ptr is not in the header of the block
	ASSERT(ptrLocInBlock >= sizeof(BlockAllocHeader));


	BlockAllocHeader* header = (BlockAllocHeader*)GetBlockAddress(ptrBlockIndex);
	header->allocCount -= 1;

	size_t allocCount = header->allocCount;
	size_t blocksUsed = header->blocksUsed;

	// Push blocks that were allocated for a large object into the free list
	if (allocCount == 0 && blocksUsed > 0) {
		for (size_t i = 0; i < blocksUsed; ++i) {
            BlockAllocHeader* headerIt = (BlockAllocHeader*)((size_t)header + i * m_BlockSize);
			PushFreeListBlock(headerIt);
		}
	}
	// Pushes the single block into the free list
	else if (allocCount == 0) {
		if (header->partial) {
			RemovePartialListBlock(header);
		}

		PushFreeListBlock(header);
	}
}


bool BlockAllocator::HasPartialBlocks() {
	return m_PartialListHead != nullptr;
}

void BlockAllocator::PushPartialListBlock(BlockAllocHeader* header) {
    
	header->partial = true;

    if (m_PartialListHead == nullptr) {
        m_PartialListHead = header;
    }
    else {
        ASSERT(m_PartialListHead->next == nullptr);
        
        if (m_PartialListHead != nullptr) {
            m_PartialListHead->next = header;
        }

        header->prev = m_PartialListHead;
        header->next = nullptr;

        m_PartialListHead = header;
    }
}

void BlockAllocator::RemovePartialListBlock(BlockAllocHeader* header) {
	header->partial = false;

	if (header->next != nullptr) {
		header->next->prev = header->prev;
	}

	if (header->prev != nullptr) {
		header->prev->next = header->next;
	}

	if (m_PartialListHead == header) {
		m_PartialListHead = header->prev;
	}
}

bool BlockAllocator::HasFreeBlocks() {
	return m_FreeListHead != nullptr;
}

void BlockAllocator::PushFreeListBlock(BlockAllocHeader* header) {
    if (m_FreeListHead == nullptr) {
        m_FreeListHead = header;
    }
    else {
        ASSERT(m_FreeListHead->next == nullptr);
        
        /*if (m_FreeListHead->next != nullptr) {
            m_FreeListHead->next = header;
        }*/

        m_FreeListHead->next = header;
        
        header->prev = m_FreeListHead;
        header->next = nullptr;
        
        m_FreeListHead = header;
    }
}

BlockAllocHeader* BlockAllocator::PopFreeListBlock() {
	ASSERT(m_FreeListHead != nullptr);
	ASSERT(m_FreeListHead->next == nullptr);

	BlockAllocHeader* header = m_FreeListHead;

	if (header->prev != nullptr) {
		header->prev->next = header->next;
	}
	
	m_FreeListHead = m_FreeListHead->prev;
    
    return header;
}

size_t BlockAllocator::CalcBlockMemRemaining(BlockAllocHeader* header) {
	return m_BlockMemSize - header->blockTop;
}

size_t BlockAllocator::GetBlockAddress(size_t index) {
	ASSERT(index < m_BlockCount);

	return (size_t)m_Memory + (index * m_BlockSize);
}