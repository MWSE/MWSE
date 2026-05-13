#include "InstructionInterface.h"
#include "Stack.h"

#include "TES3Attachment.h"
#include "TES3Reference.h"

#include "ScriptUtil.h"

#include "MemoryUtil.h"

namespace mwse {
	class xActivate : InstructionInterface_t {
	public:
		xActivate();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xActivate xActivateInstance;

	xActivate::xActivate() : InstructionInterface_t(OpCode::xActivate) {}

	float xActivate::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get potential target.
		long parameter = Stack::getInstance().popLong();

		// Verify that the script is called on a valid reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << __FUNCTION__ << ": Called on invalid reference." << std::endl;
			}
			return 0.0f;
		}

		// Only certain types of objects can actually activate things without crashing.
		auto baseObjectType = reference->baseObject->objectType;
		if (baseObjectType != TES3::ObjectType::NPC && baseObjectType != TES3::ObjectType::Creature) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << __FUNCTION__ << ": Called on non-NPC, non-creature reference.." << std::endl;
			}
			return 0.0f;
		}

		// Determine if the target is a reference.
		TES3::Reference* target = NULL;
		try {
			TES3::Reference* potential = reinterpret_cast<TES3::Reference*>(parameter);
			if (potential && potential->objectType == TES3::ObjectType::Reference) {
				target = potential;
			}
		}
		catch (std::exception&) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << __FUNCTION__ << ": Target reference could not be resolved." << std::endl;
			}
			return 0.0f;
		}

		// Verify that the target reference was found.
		if (target == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << __FUNCTION__ << ": Target reference is invalid." << std::endl;
			}
			return 0.0f;
		}

		// Set up the attachment node.
		auto activator = static_cast<TES3::ActionAttachment*>(target->getAttachment(TES3::AttachmentType::Action));
		if (activator == NULL) {
			activator = se::memory::malloc<TES3::ActionAttachment>();
			activator->type = TES3::AttachmentType::Action;
			activator->next = NULL;
			activator->reference = NULL;
			activator->flags = 1;

			target->insertAttachment(activator);
		}

		// Ensure that our flags and reference is accurate.
		activator->flags = 2;
		activator->reference = reference;

		// Call the original activate function.
		TES3::Script* script = virtualMachine.getScript();
		mwscript::Activate(script, target);
		Stack::getInstance().pushLong(true);

		return 0.0f;
	}
}
