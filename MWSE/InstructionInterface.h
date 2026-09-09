#pragma once

#include "OpCodes.h"

namespace mwse {
	class VMExecuteInterface;

	class InstructionInterface_t {
	public:
		inline OpCode::OpCode_t getOpCode() const { return opcode; }
		virtual float execute(VMExecuteInterface& virtualMachine) = 0;
		virtual void loadParameters(VMExecuteInterface& virtualMachine) {}
	protected:
		InstructionInterface_t(const OpCode::OpCode_t ctorOpCode);
	private:
		const OpCode::OpCode_t opcode;
	};

	constexpr auto DEBUG_MWSCRIPT_FUNCTIONS = false;
};
