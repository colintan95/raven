#ifndef POOLALLOCATOR_TEST_H_
#define POOLALLOCATOR_TEST_H_

#include "base_include.h"

#include <gtest/gtest.h>

#include "allocator/PoolAllocator.h"

struct PoolTestStruct {
	double x;
	double y;
	double z;
	double w;
};

//--------------------------------------------------
// 
// PoolAllocatorTest
//
// PoolAllocator unit test
//
//--------------------------------------------------
class PoolAllocatorTest: public ::testing::Test {

protected:
	PoolAllocatorTest(): allocator(64) {}

	// virtual void SetUp() {}
	// virtual void TearDown() {} 

	PoolAllocator<PoolTestStruct> allocator;
};



#endif