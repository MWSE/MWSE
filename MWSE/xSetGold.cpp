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

	xSetGold::xSetGold() : InstructionInterface_t(OpCode::xSetGold) {}

	float xSetGold::execute(VMExecuteInterface& virtualMachine) {
		long gold = Stack::getInstance().popLong();

		TES3::Reference* reference = getReference(virtualMachine, __FUNCTION__);
		if (reference == nullptr) {
			return 0.0f;
		}

		TES3::MobileActor* mobile = reference->getAttachedMobileActor();
		if (mobile == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xSetGold: Could not find attached mobile actor." << std::endl;
			}
			return 0.0f;
		}

		mobile->barterGold = gold;

		return 0.0f;
	}
}
