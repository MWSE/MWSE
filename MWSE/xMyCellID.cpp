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
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xMyCellID xMyCellIDInstance;

	xMyCellID::xMyCellID() : mwse::InstructionInterface_t(OpCode::xMyCellID) {}

	float xMyCellID::execute(mwse::VMExecuteInterface& virtualMachine) {
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xMyCellID: Called on invalid reference." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		TES3::ReferenceList* referenceList = reference->owningCollection.asReferenceList;
		if (referenceList && referenceList->cell) {
			mwse::Stack::getInstance().pushString(referenceList->cell->name);
		}
		else {
			mwse::Stack::getInstance().pushLong(0);
		}

		return 0.0f;
	}
}
