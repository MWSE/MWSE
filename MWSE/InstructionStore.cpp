#include "InstructionStore.h"

using namespace mwse;

constexpr auto table_size = 255;

InstructionStore InstructionStore::singleton;

InstructionStore::InstructionStore()
{

}

void InstructionStore::add(InstructionInterface_t& implementation) {
	size_t primaryIndex = (implementation.getOpCode() >> 8) & 0xFF;
	size_t secondaryIndex = implementation.getOpCode() & 0xFF;
	InstructionInterface_t** secondaryTable = opCodePrimaryTable[primaryIndex];
	if (secondaryTable == nullptr) {
		opCodePrimaryTable[primaryIndex] = secondaryTable = new InstructionInterface_t * [table_size];
		std::fill(secondaryTable, secondaryTable + table_size, nullptr);
	}
	secondaryTable[secondaryIndex] = &implementation;
}
