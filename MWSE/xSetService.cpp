#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Class.h"
#include "TES3Util.h"
#include "TES3NPC.h"
#include "TES3Reference.h"


namespace mwse {
	class xSetService : InstructionInterface_t {
	public:
		xSetService();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xSetService xSetServiceInstance;

	xSetService::xSetService() : mwse::InstructionInterface_t(OpCode::xSetService) {}

	float xSetService::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameters.
		long flags = mwse::Stack::getInstance().popLong() & TES3::ServiceFlag::AllServicesMask;

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetService: Called on invalid reference." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Set service mask.
		TES3::AIConfig* aiConfig = reference->baseObject->getAIConfig();
		if (aiConfig) {
			aiConfig->merchantFlags = flags;
		}
		else {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetService: Could not obtain AI configuration." << std::endl;
			}
		}

		return 0.0f;
	}
}
