#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3ItemData.h"
#include "TES3Reference.h"

namespace mwse {
	class xGetStackSize : InstructionInterface_t {
	public:
		xGetStackSize();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xGetStackSize xGetStackSizeInstance;

	xGetStackSize::xGetStackSize() : InstructionInterface_t(OpCode::xGetStackSize) {}

	float xGetStackSize::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetStackSize: No reference provided." << std::endl;
			}
			stack.pushLong(0);
			return 0.0f;
		}

		// Get the associated variable node and its item count.
		long count = 0;
		auto varNode = reference->getAttachedItemData();
		if (varNode) {
			count = varNode->count;
		}

		stack.pushLong(count);

		return 0.0f;
	}
}
