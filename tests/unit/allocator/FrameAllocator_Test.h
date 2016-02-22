#ifndef FRAMEALLOCATOR_TEST_H_
#define FRAMEALLOCATOR_TEST_H_

#include "base_include.h"

#include <gtest/gtest.h>

#include "allocator/FrameAllocator.h"

const int kFrameAllocatorCapacity = 1024;

//--------------------------------------------------
// 
// FrameAllocatorTest
//
// FrameAllocator unit test
//
//--------------------------------------------------
class FrameAllocatorTest: public ::testing::Test {

protected:
	FrameAllocatorTest(): allocator(kFrameAllocatorCapacity) {}

	// virtual void SetUp() {}
	// virtual void TearDown() {} 

	FrameAllocator allocator;
};



#endif