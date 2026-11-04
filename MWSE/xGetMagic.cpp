#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3MobileNPC.h"
#include "TES3Enchantment.h"
#include "TES3Reference.h"
#include "TES3Spell.h"

namespace mwse {
	class xGetMagic : InstructionInterface_t {
	public:
		xGetMagic();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xGetMagic xGetMagicInstance;

	xGetMagic::xGetMagic() : InstructionInterface_t(OpCode::xGetMagic) {}

	float xGetMagic::execute(VMExecuteInterface& virtualMachine) {
		// Return values.
		long type = 0;
		const char* id = nullptr;

		// Get reference to what we're finding enchantment information for.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference) {
			TES3::ObjectType::ObjectType recordType = reference->baseObject->objectType;
			if (recordType == TES3::ObjectType::Creature || recordType == TES3::ObjectType::NPC) {
				auto mobileObject = reference->getAttachedMobileActor();
				if (mobileObject && mobileObject->currentSpell.source.asGeneric) {
					TES3::Object* spellSource = mobileObject->currentSpell.source.asGeneric;
					id = spellSource->getObjectID();
					type = spellSource->objectType;
				}
				else {
					if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
						log::getLog() << "xGetMagic: Could not obtain MACP record for reference." << std::endl;
					}
				}
			}
			else {
				if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
					log::getLog() << "xGetMagic: Invalid reference type:" << recordType << std::endl;
				}
			}
		}
		else {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetMagic: Could not obtain reference." << std::endl;
			}
		}

		// Return type/id.
		auto& stack = Stack::getInstance();
		stack.pushString(id);
		stack.pushLong(type);

		return 0.0f;
	}
}
