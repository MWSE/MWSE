#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3Reference.h"

namespace mwse {
	class xGetMaxCondition : InstructionInterface_t {
	public:
		xGetMaxCondition();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xGetMaxCondition xGetMaxConditionInstance;

	xGetMaxCondition::xGetMaxCondition() : InstructionInterface_t(OpCode::xGetMaxCondition) {}

	float xGetMaxCondition::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetMaxCondition: No reference provided." << std::endl;
			}
			stack.pushLong(0);
			return 0.0f;
		}

		// Get the max condition.
		TES3::Object* object = reference->baseObject;
		long value = object->getDurability();

		stack.pushLong(value);

		return 0.0f;
	}
}
