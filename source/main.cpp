#include <iostream>
#include <cstdlib>

#include "base_include.h"

#include "container/Stack.h"

using namespace std;

int main(int argc, char* argv[]) {

	Stack<int> stack(10);

    stack.Push(20);

	cout << stack.GetFront() << endl;
	
	return 0;
}