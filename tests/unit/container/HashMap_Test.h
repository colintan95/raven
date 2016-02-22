#ifndef HASHMAP_TEST_H_
#define HASHMAP_TEST_H_

#include "base_include.h"

#include <gtest/gtest.h>

#include "container/HashMap.h"


const int kHashMapSize = 128;

//--------------------------------------------------
// 
// HashMapTest
//
// HashMap unit test
//
//--------------------------------------------------
class HashMapTest: public ::testing::Test {

protected:
	HashMapTest(): 
	map(kHashMapSize),
	intMap(kHashMapSize) {}

	// virtual void SetUp() {}
	// virtual void TearDown() {}

	HashMap<const char*, int> map;
	HashMap<int, int> intMap;
};


#endif