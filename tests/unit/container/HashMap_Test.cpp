#include "hashmap_test.h"


TEST_F(HashMapTest, MaxLoad) {
	for (int i = 0; i < kHashMapSize; ++i) {
		intMap.Insert(i, i);
	}

	for (int i = 0; i < kHashMapSize; ++i) {
		auto it = intMap.Find(i);

		EXPECT_EQ(it.GetValue(), i);
	}

	for (int i = 0; i < kHashMapSize; ++i) {
		intMap.Remove(i);
	}

	for (int i = 0; i < kHashMapSize; ++i) {
		intMap.Insert(i, i);
	}

	for (int i = 0; i < kHashMapSize; ++i) {
		auto it = intMap.Find(i);

		EXPECT_EQ(it.GetValue(), i);
	}

	for (int i = 0; i < kHashMapSize; ++i) {
		intMap.Remove(i);
	}
}

TEST_F(HashMapTest, IteratorTest) {
	int counter = 0;
	for (auto it = intMap.Begin(); it != intMap.End(); ++it) {
		++counter;
	}

	EXPECT_EQ(counter, 0);

	for (int i = 0; i < kHashMapSize; ++i) {
		intMap.Insert(i, i);
	}

	counter = 0;
	for (auto it = intMap.Begin(); it != intMap.End(); ++it) {
		++counter;
	}

	EXPECT_EQ(counter, kHashMapSize);

	for (int i = 0; i < kHashMapSize; ++i) {
		intMap.Remove(i);
	}
}

TEST_F(HashMapTest, ClearTest) {
	for (int i = 0; i < kHashMapSize; ++i) {
		intMap.Insert(i, i);
	}

	for (int i = 0; i < kHashMapSize; ++i) {
		auto it = intMap.Find(i);

		EXPECT_EQ(it.GetValue(), i);
	}

	intMap.Clear();

	for (int i = 0; i < kHashMapSize; ++i) {
		intMap.Insert(i, i);
	}

	for (int i = 0; i < kHashMapSize; ++i) {
		auto it = intMap.Find(i);

		EXPECT_EQ(it.GetValue(), i);
	}

	for (int i = 0; i < kHashMapSize; ++i) {
		intMap.Remove(i);
	}
}

TEST_F(HashMapTest, StringTest) {
	map.Insert("a", 0);
	map.Insert("b", 1);
	map.Insert("c", 2);

	auto it = map.Find("a");

	EXPECT_EQ(it.GetValue(), 0);

	it = map.Find("b");

	EXPECT_EQ(it.GetValue(), 1);

	it = map.Find("c");

	EXPECT_EQ(it.GetValue(), 2);

	map.Clear();
}

TEST_F(HashMapTest, PostfixIncrementTest) {
	int counter = 0;
	for (auto it = intMap.Begin(); it != intMap.End(); it++) {
		++counter;
	}

	EXPECT_EQ(counter, 0);

	for (int i = 0; i < kHashMapSize; ++i) {
		intMap.Insert(i, i);
	}

	auto it = intMap.Begin();

	bool test = (intMap.Begin() == it++);

	EXPECT_EQ(test, true);

	counter = 0;
	for (auto it = intMap.Begin(); it != intMap.End(); it++) {
		++counter;
	}

	EXPECT_EQ(counter, kHashMapSize);

	for (int i = 0; i < kHashMapSize; ++i) {
		intMap.Remove(i);
	}
}