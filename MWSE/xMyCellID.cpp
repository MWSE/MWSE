#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3Reference.h"
#include "TES3Cell.h"

namespace mwse {
	class xMyCellID : InstructionInterface_t {
	public:
		xMyCellID();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xMyCellID xMyCellIDInstance;

	xMyCellID::xMyCellID() : InstructionInterface_t(OpCode::xMyCellID) {}

	float xMyCellID::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xMyCellID: Called on invalid reference." << std::endl;
			}
			stack.pushLong(0);
			return 0.0f;
		}

		TES3::ReferenceList* referenceList = reference->owningCollection.asReferenceList;
		if (referenceList && referenceList->cell) {
			stack.pushString(referenceList->cell->name);
		}
		else {
			stack.pushLong(0);
		}

		return 0.0f;
	}
}
