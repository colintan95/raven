#ifndef STACK_TEST_H_
#define STACK_TEST_H_

//--------------------------------------------------
//
// Stack_Test.h
//
// Stack unit test
//
//--------------------------------------------------


#include "base_include.h"

#include <gtest/gtest.h>

#include "container/Stack.h"

struct StackStruct {
	double x;
	double y;
	double z;
	double w;
};

bool operator==(const StackStruct& s1, const StackStruct& s2) {
	return (s1.x == s2.x && s1.y == s2.y && s1.z == s2.z && s1.w == s2.w);
}

class StackTest: public ::testing::Test {

protected:
	StackTest(): stack(64) {}

	// virtual void SetUp() {}
	// virtual void TearDown() {}

	Stack<StackStruct> stack;

};

#endif