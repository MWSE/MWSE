#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "MemoryUtil.h"

#include "TES3DataHandler.h"
#include "TES3Spell.h"

namespace mwse {
	class xDeleteSpell : InstructionInterface_t {
	public:
		xDeleteSpell();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xDeleteSpell xDeleteSpellInstance;

	xDeleteSpell::xDeleteSpell() : InstructionInterface_t(OpCode::xDeleteSpell) {}

	float xDeleteSpell::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get parameters.
		mwseString& id = virtualMachine.getString(stack.popLong());

		// Get spell.
		TES3::Spell* spell = TES3::DataHandler::get()->nonDynamicData->getSpellById(id.c_str());
		if (spell == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xDeleteSpell: No spell found with id '" << id << "'." << std::endl;
			}
			stack.pushLong(false);
			return 0.0f;
		}

		// Manipulate the record list to remove this object.
		auto spellsList = TES3::DataHandler::get()->nonDynamicData->spellsList;
		spellsList->erase_value(spell);

		// Delete the spell from memory.
		se::memory::free(spell->name);
		se::memory::free(spell->objectID);
		se::memory::free(spell);

		/*
			TODO: Calling AddSpell on a deleted spell does not cause an error. There
			must be some other data structure that should be updated when deleting a
			spell.
		*/

		stack.pushLong(true);

		return 0.0f;
	}
}
