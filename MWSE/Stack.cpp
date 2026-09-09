#include "Stack.h"

using namespace mwse;

Stack Stack::singleton;

Stack::Stack() {
	stackSize = initial_stack_size;
	storage = new StackItem_t[stackSize];
	stackTop = 0;
}
