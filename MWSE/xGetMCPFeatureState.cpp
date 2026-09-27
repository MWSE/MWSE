
#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

#include "CodePatchUtil.h"

namespace mwse {
	class xGetMCPFeatureState : InstructionInterface_t {
	public:
		xGetMCPFeatureState();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xGetMCPFeatureState xGetMCPFeatureStateInstance;

	xGetMCPFeatureState::xGetMCPFeatureState() : mwse::InstructionInterface_t(OpCode::xGetMCPFeatureState) {}

	float xGetMCPFeatureState::execute(mwse::VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		long id = stack.popLong();

		if (mwse::mcp::hasFeaturesFound()) {
			bool enabled = mwse::mcp::getFeatureEnabled(id);
			stack.pushShort(enabled);
		}
		else {
			stack.pushShort(-1);
		}

		return 0.0f;
	}
}
