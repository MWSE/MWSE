/************************************************************************
               InstructionStore.h - Copyright (c) 2008 The MWSE Project
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

#include "InstructionInterface.h"
#include "Log.h"

namespace mwse {
        // This can afford to be expensive for adding opcodes, as that's not done often.
        // Conversely, looking up opcodes needs to be as fast as possible.

        // The fastest would be to have an array (std::vector) but given that opcodes
        // are 16 bit, we don't want to allocate 64K for opcodes.
    class IllegalOpCode: public std::exception {
        public:
            IllegalOpCode(const OpCode::OpCode_t ctor_bad_opcode):
                bad_opcode(ctor_bad_opcode)
            {
            }
            virtual const char *what() const throw()
            {
                return "Illegal script instruction encountered";
            }
        const OpCode::OpCode_t bad_opcode;
    };

    class InstructionStore {
        private:
            InstructionStore(); // private, this is a singleton
            static InstructionStore single_instance;

            InstructionInterface_t **opCode_primary_table[256];

        public:
            static InstructionStore& getInstance()
			{
				return single_instance;
			}

            void add(InstructionInterface_t &implementation);

            inline InstructionInterface_t *get(const OpCode::OpCode_t opcode)
            {
                unsigned int primary_index = (opcode >> 8) & 0xFF;
                unsigned int secondary_index = opcode & 0xFF;
                InstructionInterface_t **secondary_table = opCode_primary_table[primary_index];
                if (secondary_table == NULL || secondary_table[secondary_index] == NULL) {
                    // FIXME: This should probably be in the VM, where it can report script name and offset.
                    log::getLog() << "Illegal or unimplemented opcode " << std::hex << opcode << std::endl;
                    throw IllegalOpCode(opcode);
                }
                return secondary_table[secondary_index];
            }

			//check if a certain opcode exists, inline to make it as fast as possible.
			inline bool isOpcode(const OpCode::OpCode_t opcode)
			{
                unsigned int primary_index = (opcode >> 8) & 0xFF;
                unsigned int secondary_index = opcode & 0xFF;
                InstructionInterface_t **secondary_table = opCode_primary_table[primary_index];
                
				return (secondary_table != NULL) && (secondary_table[secondary_index] != NULL);
			}
    };
};
