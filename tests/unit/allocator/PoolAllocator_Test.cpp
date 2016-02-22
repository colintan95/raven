#include "PoolAllocator_Test.h"

// Tests allocation and deallocation if all chunks are used
TEST_F(PoolAllocatorTest, AllocDeallocMax) {
	size_t chunkNum = allocator.GetChunkNum();

    PoolTestStruct** ptrArr = new PoolTestStruct*[chunkNum];
    PoolTestStruct** ptrArr2 = new PoolTestStruct*[chunkNum];

	for (int i = 0; i < chunkNum; ++i) {
		ptrArr[i] = allocator.Alloc();
	}

	for (int i = 0; i < chunkNum; ++i) {
		allocator.Dealloc(ptrArr[i]);
	}

	for (int i = chunkNum - 1; i >= 0; --i) {
		ptrArr2[i] = allocator.Alloc();
	}

	for (int i = 0; i < chunkNum; ++i) {
		EXPECT_EQ(ptrArr[i], ptrArr2[i]);
	}
    
    delete[] ptrArr2;
    delete[] ptrArr;
}

// Tests if clear resets the pool
TEST_F(PoolAllocatorTest, AllocClear) {
	size_t chunkNum = allocator.GetChunkNum();

    PoolTestStruct** ptrArr = new PoolTestStruct*[chunkNum];
    PoolTestStruct** ptrArr2 = new PoolTestStruct*[chunkNum];

	for (int i = 0; i < chunkNum; ++i) {
		ptrArr[i] = allocator.Alloc();
	}

	allocator.Clear();

	for (int i = 0; i < chunkNum; ++i) {
		ptrArr2[i] = allocator.Alloc();
	}

	for (int i = 0; i < chunkNum; ++i) {
		EXPECT_EQ(ptrArr[i], ptrArr2[i]);
	}
    
    delete[] ptrArr2;
    delete[] ptrArr;
}

// Tests allocation and deallocation of chunks from the free list
TEST_F(PoolAllocatorTest, FreeList) {
	PoolTestStruct* ptr1 = allocator.Alloc();
	PoolTestStruct* ptr2 = allocator.Alloc();
	PoolTestStruct* ptr3 = allocator.Alloc();
	PoolTestStruct* ptr4 = nullptr;
	PoolTestStruct* ptr5 = nullptr;
	PoolTestStruct* ptr6 = nullptr;

	allocator.Dealloc(ptr1);
	allocator.Dealloc(ptr2);
	allocator.Dealloc(ptr3);

	ptr4 = allocator.Alloc();
	ptr5 = allocator.Alloc();
	ptr6 = allocator.Alloc();

	EXPECT_EQ(ptr1, ptr6);
	EXPECT_EQ(ptr2, ptr5);
	EXPECT_EQ(ptr3, ptr4);

	allocator.Clear();

	ptr1 = allocator.Alloc();
	ptr2 = allocator.Alloc();
	ptr3 = allocator.Alloc();

	allocator.Dealloc(ptr3);
	allocator.Dealloc(ptr1);
	allocator.Dealloc(ptr2);

	ptr4 = allocator.Alloc();
	ptr5 = allocator.Alloc();
	ptr6 = allocator.Alloc();

	EXPECT_EQ(ptr4, ptr2);
	EXPECT_EQ(ptr5, ptr1);
	EXPECT_EQ(ptr6, ptr3);
}

// Tests that pool allocates from top of pool if the free list is empty
TEST_F(PoolAllocatorTest, FreeEmpty) {
	PoolTestStruct* ptr1 = allocator.Alloc();
	PoolTestStruct* ptr2 = allocator.Alloc();
	PoolTestStruct* ptr3 = allocator.Alloc();

	allocator.Dealloc(ptr2);
	allocator.Dealloc(ptr1);

	PoolTestStruct* ptr4 = allocator.Alloc();
	PoolTestStruct* ptr5 = allocator.Alloc();

	EXPECT_EQ(ptr4, ptr1);
	EXPECT_EQ(ptr5, ptr2);

	PoolTestStruct* ptr6 = allocator.Alloc();

	EXPECT_EQ(ptr6, (ptr3 + 1));
}