#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3Reference.h"

namespace mwse {
	class xGetName : InstructionInterface_t {
	public:
		xGetName();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xGetName xGetNameInstance;

	xGetName::xGetName() : mwse::InstructionInterface_t(OpCode::xGetName) {}

	float xGetName::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetName: No reference provided." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		const char* name = NULL;

		// Get the base record.
		TES3::BaseObject* record = reference->baseObject;
		if (record) {
			name = reference->baseObject->vTable.object->getName(reference->baseObject);
		}
		else {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetName: Could not obtain record from reference." << std::endl;
			}
		}

		mwse::Stack::getInstance().pushString(name);

		return 0.0f;
	}
}
