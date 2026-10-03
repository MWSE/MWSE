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
		float execute(VMExecuteInterface& virtualMachine) override;
	private:
		void pushErrorResponse();
	};

	static xSpellList xSpellListInstance;

	xSpellList::xSpellList() : InstructionInterface_t(OpCode::xSpellList) {}

	float xSpellList::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get our next node.
		auto node = stack.popPointer<NI::IteratedList<TES3::Spell*>::Node*>();

		// Arguments we will be returning.
		long spellCount = 0;
		char* spellId = nullptr;
		char* spellName = nullptr;
		long spellType = 0;
		long spellCost = 0;
		long spellEffectCount = 0;
		long spellFlags = 0;

		// Get the reference we're checking.
		TES3::Reference* reference = getReference(virtualMachine, __FUNCTION__);
		if (reference == nullptr) {
			pushErrorResponse();
			return 0.0f;
		}

		// Function only works on NPCs.
		if (reference->baseObject->objectType != TES3::ObjectType::NPC) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xSpellList: Called on non-NPC reference." << std::endl;
			}
			pushErrorResponse();
			return 0.0f;
		}

		// Make sure we have the base NPC.
		TES3::NPC* npc = reinterpret_cast<TES3::NPC*>(reference->baseObject);
		if (!npc->isBaseActor()) {
			npc = reinterpret_cast<TES3::NPCInstance*>(npc)->baseNPC;
		}


		// If we're not provided a node, get the first node of the NPC.
		if (node == nullptr) {
			node = npc->spellList.list.head;

			// If the node is still nullptr, the reference has no spells.
			if (node == nullptr) {
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
		stack.pushPointer(node->next);
		stack.pushLong(spellFlags);
		stack.pushLong(spellEffectCount);
		stack.pushLong(spellCost);
		stack.pushLong(spellType);
		stack.pushString(spellName);
		stack.pushString(spellId);
		stack.pushLong(spellCount);

		return 0.0f;
	}

	void xSpellList::pushErrorResponse() {
		for (auto i = 0; i < 8; ++i) {
			Stack::getInstance().pushLong(0);
		}
	}
}
