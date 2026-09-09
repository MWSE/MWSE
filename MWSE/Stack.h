#pragma once

#include "mwseString.h"
#include "Flags.h"
#include "Log.h"
#include "StringUtil.h"

/*
This Stack class is a singleton. It provides an
efficient push/pop mechanism for a parameter stack.

Items on the stack are native types plus the two MWSE types.
No validation across push/pop operations are performed; that is,
no error will occur in the sequence of:
	pushLong(value);
	float value = popFloat(value);
save where the value defines a cast operator that validates
its value (as may be the case for mwseString).

Basic usage:
	Stack::getInstance().pushLong(value);
	....
	long value = Stack::getInstance().popLong(value);
*/
namespace mwse {

	constexpr auto MWSE_DEBUG_STACK = false;
	constexpr auto MWSE_PRINT_DETAILED_STACK_DUMP = false;

	constexpr auto initial_stack_size = 64u;
	constexpr auto stack_grow_size = 32u;

	class Stack {
	public:
		static Stack& getInstance() {
			return singleton;
		}

		typedef unsigned int StackItem_t;

		void pushByte(const char value) {
			push(static_cast<StackItem_t>(value));
		}

		void pushShort(const short value) {
			push(static_cast<StackItem_t>(value));
		}

		void pushLong(const long value) {
			push(static_cast<StackItem_t>(value));
		}

		void pushFloat(float value) {
			push(*reinterpret_cast<StackItem_t*>(&value));
		}

		void pushString(const mwseString& value) {
			pushLong(value);
		}

		void pushString(const std::string& value) {
			pushLong(se::string::store::getOrCreate(value));
		}

		void pushString(const char* value) {
			if (value) {
				pushLong(se::string::store::getOrCreate(value));
			}
			else {
				pushLong(0);
			}
		}

		char popByte(void) {
			return static_cast<char>(pop());
		}

		short popShort(void) {
			return static_cast<short>(pop());
		}

		long popLong(void) {
			return static_cast<long>(pop());
		}

		float popFloat(void) {
			int temp = pop();
			return *reinterpret_cast<float*>(&temp);
		}

		// pop <frameCount> frames from the stack`
		void popFrames(size_t frameCount) {
			stackTop -= frameCount > stackTop ? stackTop : frameCount;
		}

		// Returns the element count of the stack.
		size_t size() {
			return stackTop;
		}

		bool empty() {
			return (stackTop == 0);
		}

		// Clears the stack.
		void clear() {
			stackTop = 0;
		}

		// Prints information about the Stack to the MWSE log file.
		void dump() {
			log::getLog() << std::dec << "Stack dump (Size: " << stackTop << "; Buffer Size: " << stackSize << "):" << std::endl;
			for (size_t i = stackTop; i > 0; i--) {
				log::getLog() << "\t" << std::dec << i - 1 << "\t" << std::hex << storage[i - 1] << "h" << std::endl;
				if constexpr (MWSE_PRINT_DETAILED_STACK_DUMP) {
					log::getLog() << "\t\tShort: " << std::dec << *reinterpret_cast<short*>(&storage[i - 1]) << std::endl;
					log::getLog() << "\t\tLong: " << std::dec << *reinterpret_cast<long*>(&storage[i - 1]) << std::endl;
					log::getLog() << "\t\tFloat: " << std::dec << *reinterpret_cast<float*>(&storage[i - 1]) << std::endl;
				}
			}
		}

	private:
		Stack();

		static Stack singleton;

		void push(StackItem_t value) {
			if (stackTop >= stackSize) {
				stackSize += stack_grow_size;
				StackItem_t* newStack = new StackItem_t[stackSize];
				std::copy(storage, storage + stackTop, newStack);
				delete[] storage;
				storage = newStack;
			}

			if constexpr (MWSE_DEBUG_STACK) {
				log::getLog() << std::dec << "Stack: Pushing element " << stackTop << " as " << std::hex << value << "h" << std::endl;
			}

			storage[stackTop] = value;
			stackTop++;

			Flags::setFlags(value);	//set flags
		}

		StackItem_t pop() {
			if (stackTop == 0) {
				if constexpr (MWSE_DEBUG_STACK) {
					mwse::log::getLog() << __FUNCTION__ << ": Stack is empty, but a pop was requested! Check function definition." << std::endl;
				}

				return 0;
			}
			stackTop--;

			if constexpr (MWSE_DEBUG_STACK) {
				log::getLog() << std::dec << "Stack: Popping element " << stackTop << " as " << std::hex << storage[stackTop] << "h" << std::endl;
			}

			return storage[stackTop];
		}

		StackItem_t* storage;  // dynamically sized array
		size_t stackSize;     // current allocated size
		size_t stackTop;      // current top (0=empty; 1=one item)
	};
};
