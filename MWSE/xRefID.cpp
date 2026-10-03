#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3Reference.h"


namespace mwse {
	class xRefID : InstructionInterface_t {
	public:
		xRefID();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xRefID xRefIDInstance;

	xRefID::xRefID() : InstructionInterface_t(OpCode::xRefID) {}

	float xRefID::execute(VMExecuteInterface& virtualMachine) {
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xRefID: Called on invalid reference." << std::endl;
			}
			return 0.0f;
		}

		const char* objectId = reference->baseObject->getObjectID();

		Stack::getInstance().pushString(objectId);

		return 0.0f;
	}
}
