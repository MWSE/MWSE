#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3Reference.h"

namespace mwse {
	class xGetMaxCondition : InstructionInterface_t {
	public:
		xGetMaxCondition();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xGetMaxCondition xGetMaxConditionInstance;

	xGetMaxCondition::xGetMaxCondition() : mwse::InstructionInterface_t(OpCode::xGetMaxCondition) {}

	float xGetMaxCondition::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetMaxCondition: No reference provided." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		// Get the max condition.
		TES3::Object* object = reference->baseObject;
		long value = object->getDurability();

		mwse::Stack::getInstance().pushLong(value);

		return 0.0f;
	}
}
