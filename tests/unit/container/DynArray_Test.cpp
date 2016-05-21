#include "DynArray_Test.h"

TEST_F(DynArrayTest, MaxCapacity) {
	EXPECT_EQ(array.GetCapacity(), kDynArraySizeMax);

	EXPECT_EQ(array.GetSize(), 0);
	EXPECT_EQ(array.IsEmpty(), true);
	EXPECT_EQ(array.IsFull(), false);

    for (int i = 0; i < kDynArraySizeMax; ++i) {
		array.PushBack(i);
	}

	EXPECT_EQ(array.GetSize(), kDynArraySizeMax);
	EXPECT_EQ(array.IsEmpty(), false);
	EXPECT_EQ(array.IsFull(), true);

    for (int i = 0; i < kDynArraySizeMax; ++i) {
		array.PopBack();
	}

	EXPECT_EQ(array.GetSize(), 0);
	EXPECT_EQ(array.IsEmpty(), true);
	EXPECT_EQ(array.IsFull(), false);
}

TEST_F(DynArrayTest, InsertRemove) {
	for (int i = 0; i < kDynArraySizeMax; ++i) {
		array.Insert(i, 0);
	}

	EXPECT_EQ(array.GetSize(), kDynArraySizeMax);

	EXPECT_EQ(array.GetFront(), kDynArraySizeMax - 1);
	EXPECT_EQ(array.GetBack(), 0);

	for (int i = 0; i < kDynArraySizeMax; ++i) {
		EXPECT_EQ(array[kDynArraySizeMax - i - 1], i);
	}

	for (int i = 0; i < kDynArraySizeMax; ++i) {
		array.Remove(0);
	}

	EXPECT_EQ(array.GetSize(), 0);
}

TEST_F(DynArrayTest, ResizeDown) {
	for (int i = 0; i < kDynArraySizeMax; ++i) {
		array.PushBack(i);
	}

	array.Resize(32);

	EXPECT_EQ(array.GetCapacity(), 32);
	EXPECT_EQ(array.GetSize(), 32);
	EXPECT_EQ(array.IsFull(), true);

	for (int i = 0; i < 32; ++i) {
		EXPECT_EQ(array[i], i);
	}
}

TEST_F(DynArrayTest, ResizeUp) {
	for (int i = 0; i < kDynArraySizeMax; ++i) {
		array.PushBack(i);
	}

	array.Resize(160);

	EXPECT_EQ(array.GetCapacity(), 160);
	EXPECT_EQ(array.GetSize(), kDynArraySizeMax);
	EXPECT_EQ(array.IsFull(), false);

	for (int i = 0; i < kDynArraySizeMax; ++i) {
		EXPECT_EQ(array[i], i);
	}
}
