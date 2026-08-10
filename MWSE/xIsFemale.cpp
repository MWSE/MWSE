#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3NPC.h"
#include "TES3Reference.h"


namespace mwse {
	class xIsFemale : InstructionInterface_t {
	public:
		xIsFemale();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xIsFemale xIsFemaleInstance;

	xIsFemale::xIsFemale() : mwse::InstructionInterface_t(OpCode::xIsFemale) {}

	float xIsFemale::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xIsFemale: Called on invalid reference." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		long isFemale = reference->baseObject->isFemale();
		mwse::Stack::getInstance().pushLong(isFemale);

		return 0.0f;
	}
}
