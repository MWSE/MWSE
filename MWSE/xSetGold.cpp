#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3MobileNPC.h"
#include "TES3Reference.h"

namespace mwse {
	class xSetGold : InstructionInterface_t {
	public:
		xSetGold();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xSetGold xSetGoldInstance;

	xSetGold::xSetGold() : mwse::InstructionInterface_t(OpCode::xSetGold) {}

	float xSetGold::execute(mwse::VMExecuteInterface& virtualMachine) {
		long gold = mwse::Stack::getInstance().popLong();

		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetGold: No reference provided." << std::endl;
			}
			return 0.0f;
		}

		TES3::MobileActor* mobile = reference->getAttachedMobileActor();
		if (mobile == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetGold: Could not find attached mobile actor." << std::endl;
			}
			return 0.0f;
		}

		mobile->barterGold = gold;

		return 0.0f;
	}
}
