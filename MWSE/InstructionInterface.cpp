#include "InstructionInterface.h"
#include "InstructionStore.h"

using namespace mwse;

InstructionInterface_t::InstructionInterface_t(const OpCode::OpCode_t ctorOpCode) :
	opcode(ctorOpCode)
{
	InstructionStore::getInstance().add(*this);
}
