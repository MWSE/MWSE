#include "Stack.h"

using namespace mwse;

Stack Stack::singleton;

Stack::Stack() {
	stack_size = initial_stack_size;
	stack_storage = new StackItem_t[stack_size];
	stack_top = 0;
}

Stack::~Stack() {
	delete[] stack_storage;
}
