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
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xGetService xGetServiceInstance;

	xGetService::xGetService() : InstructionInterface_t(OpCode::xGetService) {}

	float xGetService::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get reference.
		TES3::Reference* reference = getReference(virtualMachine, __FUNCTION__);
		if (reference == nullptr) {
			stack.pushLong(0);
			return 0.0f;
		}

		long flags = 0;
		long mask = stack.popLong();

		// Get the AI configuration from the NPC;
		TES3::AIConfig* aiConfig = reference->baseObject->getAIConfig();
		if (aiConfig) {
			flags = aiConfig->merchantFlags & mask;
		}
		else {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetService: Could not resolve AI configuration." << std::endl;
			}
		}

		stack.pushLong(flags);

		return 0.0f;
	}
}
