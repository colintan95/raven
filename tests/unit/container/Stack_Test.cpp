#include "Stack_Test.h"

TEST_F(StackTest, MaxLoad) {
	EXPECT_EQ(stack.IsEmpty(), true);

	int cap = stack.GetCapacity();

	StackStruct ptrArr[cap];

	for (int i = 0; i < cap; ++i) {
		ptrArr[i].x = ptrArr[i].y = ptrArr[i].z = ptrArr[i].w = i;
		stack.Push(ptrArr[i]);
	}

	EXPECT_EQ(stack.IsFull(), true);

	for (int i = cap - 1; i >= 0; --i) {
		StackStruct res = stack.GetFront();
		EXPECT_EQ(ptrArr[i], res);
		stack.Pop();
	}

	EXPECT_EQ(stack.IsEmpty(), true);

	for (int i = 0; i < cap; ++i) {
		stack.Push(ptrArr[i]);
	}

	for (int i = cap - 1; i >= 0; --i) {
		StackStruct res = stack.GetFront();
		EXPECT_EQ(ptrArr[i], res);
		stack.Pop();
	}
}

TEST_F(StackTest, PushPop) {
	StackStruct str1 = {1.0, 1.0, 1.0, 1.0};
	StackStruct str2 = {2.0, 2.0, 2.0, 2.0};
	StackStruct str3 = {3.0, 3.0, 3.0, 3.0};

	stack.Push(str1);
	stack.Push(str2);
	stack.Push(str1);
	stack.Pop();

	stack.Push(str3);

	EXPECT_EQ(stack.GetFront(), str3);
}

TEST_F(StackTest, Clear) {
	int cap = stack.GetCapacity();

	StackStruct ptrArr[cap];

	for (int i = 0; i < cap; ++i) {
		ptrArr[i].x = ptrArr[i].y = ptrArr[i].z = ptrArr[i].w = i;
		stack.Push(ptrArr[i]);
	}

	stack.Clear();

	EXPECT_EQ(stack.IsEmpty(), true);

	for (int i = 0; i < cap; ++i) {
		stack.Push(ptrArr[i]);
	}

	for (int i = cap - 1; i >= 0; --i) {
		StackStruct res = stack.GetFront();
		EXPECT_EQ(ptrArr[i], res);
		stack.Pop();
	}
}