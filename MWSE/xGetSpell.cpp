#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3MobileNPC.h"
#include "TES3NPC.h"
#include "TES3Reference.h"
#include "TES3Spell.h"

namespace mwse {
	class xGetSpell : InstructionInterface_t {
	public:
		xGetSpell();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xGetSpell xGetSpellInstance;

	xGetSpell::xGetSpell() : mwse::InstructionInterface_t(OpCode::xGetSpell) {}

	float xGetSpell::execute(mwse::VMExecuteInterface& virtualMachine) {
		short result = 0;

		// Get spell id from the stack.
		mwseString& spellId = virtualMachine.getString(mwse::Stack::getInstance().popLong());

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetSpell: Could not find reference." << std::endl;
			}
			mwse::Stack::getInstance().pushShort(result);
			return 0.0f;
		}
		else if (reference->baseObject->objectType != TES3::ObjectType::NPC) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetSpell: Target is not an NPC." << std::endl;
			}
			mwse::Stack::getInstance().pushShort(result);
			return 0.0f;
		}

		TES3::NPC* npc = reinterpret_cast<TES3::NPCInstance*>(reference->baseObject)->baseNPC;
		NI::IteratedList<TES3::Spell*>::Node* currentNode = npc->spellList.list.head;
		while (currentNode != NULL) {
			TES3::Spell* spell = currentNode->data;
			if (strcmp(spell->objectID, spellId.c_str()) == 0) {
				result = 1;
				break;
			}
			currentNode = currentNode->next;
		}

		mwse::Stack::getInstance().pushShort(result);

		return 0.0f;
	}
}
