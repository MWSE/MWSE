#pragma once

#include "InstructionInterface.h"
#include "Log.h"

namespace mwse {
	// This can afford to be expensive for adding opcodes, as that's not done often.
	// Conversely, looking up opcodes needs to be as fast as possible.

	// The fastest would be to have an array (std::vector) but given that opcodes
	// are 16 bit, we don't want to allocate 64K for opcodes.
	class IllegalOpCode : public std::exception {
	public:
		IllegalOpCode(const OpCode::OpCode_t ctorBadOpCode) :
			badOpCode(ctorBadOpCode)
		{

		}

		virtual const char* what() const throw() {
			return "Illegal script instruction encountered";
		}

		const OpCode::OpCode_t badOpCode;
	};

	class InstructionStore {
	private:
		InstructionStore(); // private, this is a singleton
		static InstructionStore singleton;

		InstructionInterface_t** opCodePrimaryTable[256];

	public:
		static InstructionStore& getInstance() {
			return singleton;
		}

		void add(InstructionInterface_t& implementation);

		inline InstructionInterface_t* get(const OpCode::OpCode_t opcode) {
			unsigned int primaryIndex = (opcode >> 8) & 0xFF;
			unsigned int secondaryIndex = opcode & 0xFF;
			InstructionInterface_t** secondaryTable = opCodePrimaryTable[primaryIndex];
			if (secondaryTable == nullptr || secondaryTable[secondaryIndex] == nullptr) {
				// FIXME: This should probably be in the VM, where it can report script name and offset.
				log::getLog() << "Illegal or unimplemented opcode " << std::hex << opcode << std::endl;
				throw IllegalOpCode(opcode);
			}
			return secondaryTable[secondaryIndex];
		}

		//check if a certain opcode exists, inline to make it as fast as possible.
		inline bool isOpcode(const OpCode::OpCode_t opcode) {
			unsigned int primaryIndex = (opcode >> 8) & 0xFF;
			unsigned int secondaryIndex = opcode & 0xFF;
			InstructionInterface_t** secondary_table = opCodePrimaryTable[primaryIndex];

			return (secondary_table != nullptr) && (secondary_table[secondaryIndex] != nullptr);
		}
	};
}
