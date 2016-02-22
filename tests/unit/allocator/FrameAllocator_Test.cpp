#include "FrameAllocator_Test.h"

TEST_F(FrameAllocatorTest, MaxCapacity) {
	EXPECT_EQ(allocator.GetCapacity(), kFrameAllocatorCapacity);
	EXPECT_EQ(allocator.GetSize(), 0);

	allocator.Alloc(kFrameAllocatorCapacity);

	EXPECT_EQ(allocator.GetCapacity(), kFrameAllocatorCapacity);
	EXPECT_EQ(allocator.GetSize(), kFrameAllocatorCapacity);

	allocator.Clear();

	EXPECT_EQ(allocator.GetCapacity(), kFrameAllocatorCapacity);
	EXPECT_EQ(allocator.GetSize(), 0);
}

TEST_F(FrameAllocatorTest, Clear) {
	allocator.Alloc(kFrameAllocatorCapacity);
	allocator.Clear();

	allocator.Alloc(kFrameAllocatorCapacity);

	EXPECT_EQ(allocator.GetSize(), kFrameAllocatorCapacity);

	allocator.Clear();

	EXPECT_EQ(allocator.GetSize(), 0);
}

TEST_F(FrameAllocatorTest, SmallAlloc) {
	for (int i = 0; i < kFrameAllocatorCapacity; ++i) {
		allocator.Alloc(1);
	}

	EXPECT_EQ(allocator.GetSize(), kFrameAllocatorCapacity);

	allocator.Clear();
}

TEST_F(FrameAllocatorTest, AlignedAlloc) {
	allocator.AllocAligned(16, 16);

	EXPECT_EQ(allocator.GetSize(), 32);
}