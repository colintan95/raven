#ifndef HASHMULTIMAP_TEST_H_
#define HASHMULTIMAP_TEST_H_

#include "base_include.h"

#include <gtest/gtest.h>

#include "container/HashMultimap.h"


const int kHashMultimapSize = 128;

//--------------------------------------------------
// 
// HashMultimapTest
//
// HashMultimap unit test
//
//--------------------------------------------------
class HashMultimapTest: public ::testing::Test {

protected:
	HashMultimapTest():
        map(kHashMultimapSize),
        intMap(kHashMultimapSize) {}

	// virtual void SetUp() {}
	// virtual void TearDown() {}

	HashMultimap<const char*, int> map;
	HashMultimap<int, int> intMap;
};

#endif