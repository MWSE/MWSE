#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3MobileNPC.h"
#include "TES3Reference.h"

namespace mwse {
	class xGetCombat : InstructionInterface_t {
	public:
		xGetCombat();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xGetCombat xGetCombatInstance;

	xGetCombat::xGetCombat() : mwse::InstructionInterface_t(OpCode::xGetCombat) {}

	float xGetCombat::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get MACP record.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetCombat: No reference provided." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		auto mobileObject = reference->getAttachedMobileActor();
		if (mobileObject == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetCombat: No mach node found." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		// Push the reference of the combat target, or 0 if no target reference is found.
		if (mobileObject->actionData.target && mobileObject->actionData.target->reference) {
			mwse::Stack::getInstance().pushLong((long)mobileObject->actionData.target->reference);
		}
		else {
			mwse::Stack::getInstance().pushLong(0);
		}

		return 0.0f;
	}
}
