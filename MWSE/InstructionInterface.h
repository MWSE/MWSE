#pragma once

#include "OpCodes.h"

#include "TES3Defines.h"

namespace mwse {
	class VMExecuteInterface;

	class InstructionInterface_t {
	public:
		inline OpCode::OpCode_t getOpCode() const { return opcode; }
		TES3::Reference* getReference(VMExecuteInterface& virtualMachine, std::string_view functionName) const;
		virtual float execute(VMExecuteInterface& virtualMachine) = 0;
		virtual void loadParameters(VMExecuteInterface& virtualMachine) {}
	protected:
		InstructionInterface_t(const OpCode::OpCode_t ctor_opcode);
	private:
		const OpCode::OpCode_t opcode;
	};

	constexpr auto DEBUG_MWSCRIPT_FUNCTIONS = false;
};
