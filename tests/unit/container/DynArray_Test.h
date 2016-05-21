#ifndef DYNARRAY_TEST_H_
#define DYNARRAY_TEST_H_

#include <gtest/gtest.h>

#include "container/DynArray.h"


const int kDynArraySizeMax = 128;

//--------------------------------------------------
//
// StackTest
//
// Stack unit test
//
//--------------------------------------------------
class DynArrayTest: public ::testing::Test {

protected:
	DynArrayTest(): array(kDynArraySizeMax) {}

	// virtual void SetUp() {}
	// virtual void TearDown() {}

	DynArray<int> array;

};

#endif