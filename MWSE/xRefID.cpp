#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3Reference.h"


namespace mwse {
	class xRefID : InstructionInterface_t {
	public:
		xRefID();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xRefID xRefIDInstance;

	xRefID::xRefID() : mwse::InstructionInterface_t(OpCode::xRefID) {}

	float xRefID::execute(mwse::VMExecuteInterface& virtualMachine) {
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xRefID: Called on invalid reference." << std::endl;
			}
			return 0.0f;
		}

		const char* objectId = reference->baseObject->getObjectID();

		Stack::getInstance().pushString(objectId);

		return 0.0f;
	}
}
