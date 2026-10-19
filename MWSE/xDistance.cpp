#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Reference.h"

namespace mwse {
	class xDistance : InstructionInterface_t {
	public:
		xDistance();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xDistance xDistanceInstance;

	xDistance::xDistance() : InstructionInterface_t(OpCode::xDistance) {}

	float xDistance::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get target reference
		auto targetref = stack.popPointer<TES3::Reference*>();
		if (targetref == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xModProgressSkill: Target reference is invalid." << std::endl;
			}
			stack.pushFloat(0.0f);
			return 0.0f;
		}

		// Get script reference.
		TES3::Reference* thisref = getReference(virtualMachine, __FUNCTION__);
		if (thisref == nullptr) {
			stack.pushFloat(0.0f);
			return 0.0f;
		}

		float xDistance = targetref->position.distance(&thisref->position);
		stack.pushFloat(xDistance);

		return 0.0f;
	}
}
