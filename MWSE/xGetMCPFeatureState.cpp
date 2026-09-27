
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
		long id = mwse::Stack::getInstance().popLong();

		if (mwse::mcp::hasFeaturesFound()) {
			bool enabled = mwse::mcp::getFeatureEnabled(id);
			mwse::Stack::getInstance().pushShort(enabled);
		}
		else {
			mwse::Stack::getInstance().pushShort(-1);
		}

		return 0.0f;
	}
}
