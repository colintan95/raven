#include "hashmultimap_test.h"

TEST_F(HashMultimapTest, BucketIteration) {

	for (int i = 0; i < 5; ++i) {
		map.Insert("a", i * 10);
	}

	int counter = 0;
	for (auto it = map.Begin("a"); it != map.End("a"); ++it) {
		EXPECT_EQ(it.GetValue(), counter * 10);
		++counter;
	}
}

TEST_F(HashMultimapTest, ValueRemoval) {
	for (int i = 0; i < 5; ++i) {
		map.Insert("a", i * 10);
	}

	EXPECT_EQ(map.GetSize(), 5);


	for (int i = 0; i < 5; ++i) {
		map.Remove("a", i * 10);
	}

	EXPECT_EQ(map.GetSize(), 0);

	for (int i = 0; i < 5; ++i) {
		map.Insert("a", i * 10);
	}

	int counter = 0;
	for (auto it = map.Begin("a"); it != map.End("a"); ++it) {
		EXPECT_EQ(it.GetValue(), counter * 10);
		++counter;
	}

	for (int i = 5 - 1; i >= 0; --i) {
		map.Remove("a", i * 10);
	}
}

TEST_F(HashMultimapTest, KeyRemoval) {
	for (int i = 0; i < 5; ++i) {
		map.Insert("a", i * 10);
	}

	for (int i = 0; i < 5; ++i) {
		map.Insert("b", i * 10);
	}

	map.RemoveKey("a");

	EXPECT_EQ(map.GetSize(), 5);

	int counter = 0;
	for (auto it = map.Begin("b"); it != map.End("b"); ++it) {
		EXPECT_EQ(it.GetValue(), counter * 10);
		++counter;
	}

	EXPECT_EQ(counter, 5);

	map.RemoveKey("b");

	EXPECT_EQ(map.GetSize(), 0);
}

TEST_F(HashMultimapTest, MaxLoad) {
	for (int i = 0; i < kHashMultimapSize; ++i) {
		intMap.Insert(i, i);
	}

	for (int i = 0; i < kHashMultimapSize; ++i) {
		auto it = intMap.Begin(i);
		EXPECT_EQ(it.GetValue(), i);
	}

	for (int i = 0; i < kHashMultimapSize; ++i) {
		intMap.Remove(i, i);
	}

	for (int i = kHashMultimapSize - 1; i >= 0; --i) {
		intMap.Insert(i, i);
	}

	for (int i = 0; i < kHashMultimapSize; ++i) {
		auto it = intMap.Begin(i);
		EXPECT_EQ(it.GetValue(), i);
	}

	for (int i = 0; i < kHashMultimapSize; ++i) {
		intMap.Remove(i, i);
	}
}

TEST_F(HashMultimapTest, WorstCaseLoad) {
	for (int i = 0; i < kHashMultimapSize; ++i) {
		intMap.Insert(0, i);
	}

	int counter = 0;
	for (auto it = intMap.Begin(0); it != intMap.End(0); ++it) {
		EXPECT_EQ(it.GetValue(), counter);
		++counter;
	}

	for (int i = 0; i < kHashMultimapSize; ++i) {
		intMap.Remove(i, i);
	}
}