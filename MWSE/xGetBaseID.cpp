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
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xGetBaseID xGetBaseIDInstance;

	xGetBaseID::xGetBaseID() : mwse::InstructionInterface_t(OpCode::xGetBaseID) {}

	float xGetBaseID::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetBaseID: Called without reference." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		// Push the found objectId.
		const char* objectId = reference->getBaseObject()->getObjectID();
		if (objectId) {
			mwse::Stack::getInstance().pushString(objectId);
		}
		else {
			mwse::Stack::getInstance().pushString("unknown");
		}

		return 0.0f;
	}
}
