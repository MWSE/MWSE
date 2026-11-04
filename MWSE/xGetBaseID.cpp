#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3Reference.h"
#include "TES3Actor.h"

namespace mwse {
	class xGetBaseID : InstructionInterface_t {
	public:
		xGetBaseID();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xGetBaseID xGetBaseIDInstance;

	xGetBaseID::xGetBaseID() : InstructionInterface_t(OpCode::xGetBaseID) {}

	float xGetBaseID::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get reference.
		TES3::Reference* reference = getReference(virtualMachine, __FUNCTION__);
		if (reference == nullptr) {
			stack.pushLong(0);
			return 0.0f;
		}

		// Push the found objectId.
		const char* objectId = reference->getBaseObject()->getObjectID();
		if (objectId) {
			stack.pushString(objectId);
		}
		else {
			stack.pushString("unknown");
		}

		return 0.0f;
	}
}
