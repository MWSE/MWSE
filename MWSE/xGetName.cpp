#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3Reference.h"

namespace mwse {
	class xGetName : InstructionInterface_t {
	public:
		xGetName();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xGetName xGetNameInstance;

	xGetName::xGetName() : InstructionInterface_t(OpCode::xGetName) {}

	float xGetName::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetName: No reference provided." << std::endl;
			}
			stack.pushLong(0);
			return 0.0f;
		}

		const char* name = nullptr;

		// Get the base record.
		TES3::BaseObject* record = reference->baseObject;
		if (record) {
			name = reference->baseObject->getName();
		}
		else {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetName: Could not obtain record from reference." << std::endl;
			}
		}

		stack.pushString(name);

		return 0.0f;
	}
}
