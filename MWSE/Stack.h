/************************************************************************
               Stack.h - Copyright (c) 2008 The MWSE Project
                https://github.com/MWSE/MWSE/

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

**************************************************************************/

#pragma once

#define MWSE_DEBUG_STACK 0
#define MWSE_PRINT_DETAILED_STACK_DUMP 0

#include "mwseString.h"
#include "Flags.h"
#include "Log.h"
#include "StringUtil.h"

/**
 * This Stack class is a singleton. It provides an
 * efficient push/pop mechanism for a parameter stack.
 *
 * Items on the stack are native types plus the two MWSE types.
 * No validation across push/pop operations are performed; that is,
 * no error will occur in the sequence of:
 *      pushLong(value);
 *      float value = popFloat(value);
 * save where the value defines a cast operator that validates
 * its value (as may be the case for mwseString).
 *
 * Basic usage:
 *      Stack::getInstance().pushLong(value);
 *      ....
 *      long value = Stack::getInstance().popLong(value);
 */
namespace mwse {

    static const size_t initial_stack_size = 64;
    static const size_t stack_grow_size = 32;

    class Stack {
        public:
            static Stack &getInstance() { return singleton; };

            typedef unsigned int StackItem_t;

			void pushByte(const char value)
			{
				push(static_cast<StackItem_t>(value));
			}
            void pushShort(const short value)
            {
                push(static_cast<StackItem_t>(value));
            }
            void pushLong(const long value)
            {
                push(static_cast<StackItem_t>(value));
            }
            void pushFloat(float value)
            {
                push(*reinterpret_cast<StackItem_t*>(&value));
            }
            void pushString(const mwseString& value)
            {
                pushLong(value);
            }
			void pushString(const std::string& value)
			{
				pushLong(mwse::string::store::getOrCreate(value));
			}
			void pushString(const char* value)
			{
				if (value) {
					pushLong(mwse::string::store::getOrCreate(value));
				}
				else {
					pushLong(0);
				}
			}

			char popByte(void)
			{
				return static_cast<char>(pop());
			}
            short popShort(void)
            {
                return static_cast<short>(pop());
            }
            long popLong(void)
            {
                return static_cast<long>(pop());
            }
            float popFloat(void)
            {
				int temp = pop();
				return *reinterpret_cast<float*>(&temp);
            }
            //mwRef_t & popRef(void);
            void popFrames(size_t frame_count)  // pop <frame_count> frames from the stack`
            {
                stack_top -= frame_count > stack_top ? stack_top : frame_count;
            }

			// Returns the element count of the stack.
			size_t size()
			{
				return stack_top;
			}

			bool empty()
			{
				return (stack_top == 0);
			}

			// Clears the stack.
			void clear()
			{
				stack_top = 0;
			}

			// Prints information about the Stack to the MWSE log file.
			void dump()
			{
				log::getLog() << std::dec << "Stack dump (Size: " << stack_top << "; Buffer Size: " << stack_size << "):" << std::endl;
				for (size_t i = stack_top; i > 0; i--) {
					log::getLog() << "\t" << std::dec << i - 1 << "\t" << std::hex << stack_storage[i - 1] << "h" << std::endl;
#if MWSE_PRINT_DETAILED_STACK_DUMP
					log::getLog() << "\t\tShort: " << std::dec << *reinterpret_cast<short*>(&stack_storage[i - 1]) << std::endl;
					log::getLog() << "\t\tLong: " << std::dec << *reinterpret_cast<long*>(&stack_storage[i - 1]) << std::endl;
					log::getLog() << "\t\tFloat: " << std::dec << *reinterpret_cast<float*>(&stack_storage[i - 1]) << std::endl;
#endif
				}
			}

        private:
            Stack();

            static Stack singleton;

            void push(StackItem_t value)
            {
                if (stack_top >= stack_size) {
                    stack_size += stack_grow_size;
                    StackItem_t *new_stack = new StackItem_t[stack_size];
                    for (size_t i = 0; i < stack_top; i++) {
                        new_stack[i] = stack_storage[i];
                    }
                    delete [] stack_storage;
                    stack_storage = new_stack;
                }
#if MWSE_DEBUG_STACK
                log::getLog() << std::dec << "Stack: Pushing element " << stack_top << " as " << std::hex << value << "h" << std::endl;
#endif
                stack_storage[stack_top] = value;
                stack_top++;

				Flags::setFlags(value);	//set flags
            }
    
            StackItem_t pop()
            {
                if (stack_top == 0) {
#if _DEBUG
					mwse::log::getLog() << __FUNCTION__ << ": Stack is empty, but a pop was requested! Check function definition." << std::endl;
#endif
					return 0;
                }
                stack_top --;
#if MWSE_DEBUG_STACK
				log::getLog() << std::dec << "Stack: Popping element " << stack_top << " as " << std::hex << stack_storage[stack_top] << "h" << std::endl;
#endif
                return stack_storage[stack_top];
            }

            StackItem_t * stack_storage;  // dynamically sized array
            size_t      stack_size;     // current allocated size
            size_t      stack_top;      // current top (0=empty; 1=one item)
    };
};
