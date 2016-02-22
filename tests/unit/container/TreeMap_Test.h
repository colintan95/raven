#ifndef TREEMAP_TEST_H_
#define TREEMAP_TEST_H_

#include "base_include.h"

#include <gtest/gtest.h>

#include "container/TreeMap.h"


const int kTreeMapSize = 128;

//--------------------------------------------------
// 
// TreeMapTest
//
// TreeMap unit test
//
//--------------------------------------------------
class TreeMapTest: public ::testing::Test {

protected:
	TreeMapTest():
        map(kTreeMapSize) {}

	// virtual void SetUp() {}
	// virtual void TearDown() {}

	TreeMap<int, int> map;
};

#endif