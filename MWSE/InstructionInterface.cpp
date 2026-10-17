#include "InstructionInterface.h"
#include "InstructionStore.h"
#include "Log.h"
#include "VMExecuteInterface.h"

using namespace mwse;

TES3::Reference* InstructionInterface_t::getReference(VMExecuteInterface& virtualMachine, std::string_view functionName) const {
	TES3::Reference* reference = virtualMachine.getReference();
	if (reference == nullptr) {
		if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
			log::getLog() << functionName << ": Called on invalid reference." << std::endl;
		}
	}
	return reference;
}

InstructionInterface_t::InstructionInterface_t(const OpCode::OpCode_t ctor_opcode) :
	opcode(ctor_opcode)
{
	InstructionStore::getInstance().add(*this);
}
