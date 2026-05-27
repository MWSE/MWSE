#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3NPC.h"
#include "TES3Reference.h"
#include "TES3Spell.h"

namespace mwse {
	class xSpellList : InstructionInterface_t {
	public:
		xSpellList();
		virtual float execute(VMExecuteInterface& virtualMachine);
	private:
		void pushErrorResponse();
	};

	static xSpellList xSpellListInstance;

	xSpellList::xSpellList() : mwse::InstructionInterface_t(OpCode::xSpellList) {}

	float xSpellList::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get our next node.
		auto node = reinterpret_cast<NI::IteratedList<TES3::Spell*>::Node*>(mwse::Stack::getInstance().popLong());

		// Arguments we will be returning.
		long spellCount = 0;
		char* spellId = NULL;
		char* spellName = NULL;
		long spellType = 0;
		long spellCost = 0;
		long spellEffectCount = 0;
		long spellFlags = 0;

		// Get the reference we're checking.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSpellList: Could not get reference." << std::endl;
			}
			pushErrorResponse();
			return 0.0f;
		}

		// Function only works on NPCs.
		if (reference->baseObject->objectType != TES3::ObjectType::NPC) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSpellList: Called on non-NPC reference." << std::endl;
			}
			pushErrorResponse();
			return 0.0f;
		}

		// Make sure we have the base NPC.
		TES3::NPC* npc = reinterpret_cast<TES3::NPC*>(reference->baseObject);
		if (!(npc->actorFlags & TES3::ActorFlag::IsBase)) {
			npc = reinterpret_cast<TES3::NPCInstance*>(npc)->baseNPC;
		}


		// If we're not provided a node, get the first node of the NPC.
		if (node == NULL) {
			node = npc->spellList.list.head;

			// If the node is still NULL, the reference has no spells.
			if (node == NULL) {
				pushErrorResponse();
				return 0.0f;
			}
		}

		// Get our data.
		TES3::Spell* spell = node->data;
		spellCount = npc->spellList.list.size();
		spellId = spell->objectID;
		spellName = spell->name;
		spellType = spell->castType;
		spellCost = spell->magickaCost;
		spellEffectCount = spell->getActiveEffectCount();
		spellFlags = spell->spellFlags;

		// Push the data back to mwscript.
		mwse::Stack::getInstance().pushLong((long)node->next);
		mwse::Stack::getInstance().pushLong(spellFlags);
		mwse::Stack::getInstance().pushLong(spellEffectCount);
		mwse::Stack::getInstance().pushLong(spellCost);
		mwse::Stack::getInstance().pushLong(spellType);
		mwse::Stack::getInstance().pushString(spellName);
		mwse::Stack::getInstance().pushString(spellId);
		mwse::Stack::getInstance().pushLong(spellCount);

		return 0.0f;
	}

	void xSpellList::pushErrorResponse() {
		for (auto i = 0; i < 8; ++i) {
			mwse::Stack::getInstance().pushLong(0);
		}
	}
}
