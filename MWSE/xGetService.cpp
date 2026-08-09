#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3MobileNPC.h"
#include "TES3NPC.h"
#include "TES3Reference.h"
#include "TES3AIConfig.h"


namespace mwse {
	class xGetService : InstructionInterface_t {
	public:
		xGetService();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xGetService xGetServiceInstance;

	xGetService::xGetService() : mwse::InstructionInterface_t(OpCode::xGetService) {}

	float xGetService::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (!reference) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetRace: No reference provided." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		long flags = 0;
		long mask = mwse::Stack::getInstance().popLong();

		// Get the AI configuration from the NPC;
		TES3::AIConfig* aiConfig = reference->baseObject->getAIConfig();
		if (aiConfig) {
			flags = aiConfig->merchantFlags & mask;
		}
		else {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetService: Could not resolve AI configuration." << std::endl;
			}
		}

		mwse::Stack::getInstance().pushLong(flags);

		return 0.0f;
	}
}
