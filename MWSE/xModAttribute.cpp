#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3MobileNPC.h"
#include "TES3Reference.h"

namespace mwse {
	class xModAttribute : InstructionInterface_t {
	public:
		xModAttribute();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xModAttribute xModAttributeInstance;

	xModAttribute::xModAttribute() : mwse::InstructionInterface_t(OpCode::xModAttribute) {}

	float xModAttribute::execute(mwse::VMExecuteInterface& virtualMachine) {
		if (mwse::Stack::getInstance().size() < 2) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xModAttribute: Function called with too few arguments." << std::endl;
			}
			return 0.0f;
		}

		long attributeId = mwse::Stack::getInstance().popLong();
		float modValue = mwse::Stack::getInstance().popFloat();

		// Verify attribute range.
		if (attributeId < TES3::Attribute::FirstAttribute || attributeId > TES3::Attribute::LastAttribute) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xModAttribute: Invalid attribute id: " << attributeId << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		// Get script reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xModAttribute: Called on invalid reference." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		// Make sure we're looking at an NPC or creature.
		TES3::ObjectType::ObjectType type = reference->baseObject->objectType;
		if (type != TES3::ObjectType::NPC && type != TES3::ObjectType::Creature) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xModAttribute: Called on non-NPC, non-creature reference." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		// Get the associated MACP record.
		auto mobileObject = reference->getAttachedMobileActor();
		if (mobileObject == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xModAttribute: Could not find MACP record for reference." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		// Modify current.
		float newValue = mobileObject->attributes[attributeId].current + modValue;
		if (newValue < 0) {
			newValue = 0;
		}
		mobileObject->attributes[attributeId].current = newValue;

		// Modify base.
		newValue = mobileObject->attributes[attributeId].base + modValue;
		if (newValue < 0) {
			newValue = 0;
		}
		mobileObject->attributes[attributeId].base = newValue;

		// Push to indicate success.
		mwse::Stack::getInstance().pushLong(1);

		return 0.0f;
	}
}
