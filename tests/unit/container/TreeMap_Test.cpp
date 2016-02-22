#include "treemap_test.h"

TEST_F(TreeMapTest, MaxInsertion) {

	for (int i = 0; i < kTreeMapSize; ++i) {
		map.Insert(i, i);
	}

    int counter = 0;
	for (auto it = map.Begin(); it != map.End(); ++it) {
		EXPECT_EQ(it.GetValue(), counter);
        ++counter;
	}
    
    EXPECT_EQ(counter, kTreeMapSize);

    for (int i = 0; i < kTreeMapSize; ++i) {
		map.Remove(i);
	}

	for (int i = kTreeMapSize - 1; i >= 0; --i) {
		map.Insert(i, i);
	}

    counter = 0;
	for (auto it = map.Begin(); it != map.End(); ++it) {
		EXPECT_EQ(it.GetValue(), counter);
        ++counter;
	}
    
    EXPECT_EQ(counter, kTreeMapSize);

    for (int i = 0; i < kTreeMapSize; ++i) {
		map.Remove(i);
	}
}

TEST_F(TreeMapTest, Find) {

	for (int i = 0; i < kTreeMapSize; ++i) {
		map.Insert(i, i);
	}

    for (int i = 0; i < kTreeMapSize; ++i) {
    	EXPECT_EQ(map.Find(i).GetValue(), i);
    }

    for (int i = 0; i < kTreeMapSize; ++i) {
		map.Remove(i);
	}

	for (int i = 0; i < kTreeMapSize; ++i) {
		auto it = map.Find(i);

		bool res = (it == map.End());

		EXPECT_EQ(res, true);
	}
}

TEST_F(TreeMapTest, Clear) {
	for (int i = 0; i < kTreeMapSize; ++i) {
		map.Insert(i, i);
	}

	map.Clear();

	for (int i = 0; i < kTreeMapSize; ++i) {
		map.Insert(i, i);
	}

    for (int i = 0; i < kTreeMapSize; ++i) {
    	EXPECT_EQ(map.Find(i).GetValue(), i);
    }

    map.Clear();
}