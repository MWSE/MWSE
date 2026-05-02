#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3Reference.h"
#include "TES3Script.h"
#include "TES3GameFile.h"

namespace mwse {
	class xNextRef : InstructionInterface_t {
	public:
		xNextRef();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xNextRef xNextRefInstance;

	xNextRef::xNextRef() : mwse::InstructionInterface_t(OpCode::xNextRef) {}

	float xNextRef::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get REFR pointer as an argument.
		TES3::Reference* reference = (TES3::Reference*)mwse::Stack::getInstance().popLong();

		// Start looking for our next reference.
		TES3::Reference* next = NULL;
		__try {
			if (reference) {
				// Try to get the next non-removed reference linked down from the passed one.
				next = static_cast<TES3::Reference*>(reference->nextInCollection->skipDeletedObjects());

				// If we found nothing, check the stored exterior references.
				if (next == NULL && mwse::tes3::exteriorRefs[0] != NULL) {
					next = mwse::tes3::exteriorRefs[0];
					for (auto i = 0; i < 8; ++i) {
						mwse::tes3::exteriorRefs[i] = mwse::tes3::exteriorRefs[i + 1];
					}
				}
			}
			else {
				if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
					mwse::log::getLog() << "xNextRef: Null argument." << std::endl;
				}
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			TES3::Script* script = virtualMachine.getScript();
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xNextRef: Invalid object given in script " << script->sourceMod->filename << "/" << script->header.name << ". Fix script to not save variables across saves!" << std::endl;
			}
			next = NULL;
		}

		mwse::Stack::getInstance().pushLong((long)next);

		return 0.0f;
	}
}
