#include "InstructionStore.h"

using namespace mwse;

constexpr auto table_size = 255;

InstructionStore InstructionStore::single_instance;

InstructionStore::InstructionStore()
{

}

void InstructionStore::add(InstructionInterface_t& implementation) {
	size_t primary_index = (implementation.getOpCode() >> 8) & 0xFF;
	size_t secondary_index = implementation.getOpCode() & 0xFF;
	InstructionInterface_t** secondary_table = opCode_primary_table[primary_index];
	if (secondary_table == nullptr) {
		opCode_primary_table[primary_index] = secondary_table = new InstructionInterface_t * [table_size];
		std::fill(secondary_table, secondary_table + table_size, nullptr);
	}
	secondary_table[secondary_index] = &implementation;
}
