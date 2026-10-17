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
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xSetService xSetServiceInstance;

	xSetService::xSetService() : InstructionInterface_t(OpCode::xSetService) {}

	float xSetService::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get parameters.
		long flags = stack.popLong() & TES3::ServiceFlag::AllServicesMask;

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xSetService: Called on invalid reference." << std::endl;
			}
			stack.pushLong(false);
			return 0.0f;
		}

		// Set service mask.
		TES3::AIConfig* aiConfig = reference->baseObject->getAIConfig();
		if (aiConfig) {
			aiConfig->merchantFlags = flags;
		}
		else {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xSetService: Could not obtain AI configuration." << std::endl;
			}
		}

		return 0.0f;
	}
}
