#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3Reference.h"

namespace mwse {
	class xGetLockLevel : InstructionInterface_t {
	public:
		xGetLockLevel();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xGetLockLevel xGetLockLevelInstance;

	xGetLockLevel::xGetLockLevel() : mwse::InstructionInterface_t(OpCode::xGetLockLevel) {}

	float xGetLockLevel::execute(mwse::VMExecuteInterface& virtualMachine) {
		short lockLevel = -1;

		// Get reference to what we're finding the lock level of.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetLockLevel: No reference provided." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		TES3::ObjectType::ObjectType type = reference->baseObject->objectType;
		if (type == TES3::ObjectType::Container || type == TES3::ObjectType::Door) {
			auto lockNode = reference->getAttachedLockNode();
			if (lockNode) {
				lockLevel = lockNode->lockLevel;
			}
			else {
				if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
					log::getLog() << "xGetLockLevel: Could not obtain lock node." << std::endl;
				}
			}
		}
		else {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetLockLevel: Called on a non-container, non-door reference." << std::endl;
			}
		}

		Stack::getInstance().pushShort(lockLevel);

		return 0.0f;
	}
}
