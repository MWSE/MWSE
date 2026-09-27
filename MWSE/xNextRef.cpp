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
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xNextRef xNextRefInstance;

	xNextRef::xNextRef() : InstructionInterface_t(OpCode::xNextRef) {}

	float xNextRef::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get REFR pointer as an argument.
		auto reference = (TES3::Reference*)stack.popLong();

		// Start looking for our next reference.
		TES3::Reference* next = nullptr;
		__try {
			if (reference) {
				// Try to get the next non-removed reference linked down from the passed one.
				next = static_cast<TES3::Reference*>(reference->nextInCollection->skipDeletedObjects());

				// If we found nothing, check the stored exterior references.
				if (next == nullptr && tes3::exteriorRefs[0] != nullptr) {
					next = tes3::exteriorRefs[0];
					for (auto i = 0; i < 8; ++i) {
						tes3::exteriorRefs[i] = tes3::exteriorRefs[i + 1];
					}
				}
			}
			else {
				if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
					log::getLog() << "xNextRef: Null argument." << std::endl;
				}
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			TES3::Script* script = virtualMachine.getScript();
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xNextRef: Invalid object given in script " << script->sourceMod->filename << "/" << script->header.name << ". Fix script to not save variables across saves!" << std::endl;
			}
			next = nullptr;
		}

		stack.pushLong((long)next);

		return 0.0f;
	}
}
