#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3Reference.h"

namespace mwse {
	class xGetLockLevel : InstructionInterface_t {
	public:
		xGetLockLevel();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xGetLockLevel xGetLockLevelInstance;

	xGetLockLevel::xGetLockLevel() : InstructionInterface_t(OpCode::xGetLockLevel) {}

	float xGetLockLevel::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		short lockLevel = -1;

		// Get reference to what we're finding the lock level of.
		TES3::Reference* reference = getReference(virtualMachine, __FUNCTION__);
		if (reference == nullptr) {
			stack.pushLong(0);
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

		stack.pushShort(lockLevel);

		return 0.0f;
	}
}
