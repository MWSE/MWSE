#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3MobileNPC.h"
#include "TES3Skill.h"
#include "TES3Reference.h"

namespace mwse {
	class xGetBaseAttribute : InstructionInterface_t {
	public:
		xGetBaseAttribute();
		virtual float execute(VMExecuteInterface& virtualMachine);
	private:
		const float INVALID_VALUE = -1.0f;
	};

	static xGetBaseAttribute xGetBaseAttributeInstance;

	xGetBaseAttribute::xGetBaseAttribute() : mwse::InstructionInterface_t(OpCode::xGetBaseAttribute) {}

	float xGetBaseAttribute::execute(mwse::VMExecuteInterface& virtualMachine) {
		if (mwse::Stack::getInstance().size() < 1) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetBaseAttribute: Function called with too few arguments." << std::endl;
			}
			return 0.0f;
		}

		// Get attribute index as parameter.
		long attributeId = mwse::Stack::getInstance().popLong();
		if (attributeId < TES3::Attribute::FirstAttribute || attributeId > TES3::Attribute::LastAttribute) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetBaseAttribute: Invalid attribute id: " << attributeId << std::endl;
			}
			mwse::Stack::getInstance().pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		// Get the associated MACP record.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetBaseAttribute: No reference provided." << std::endl;
			}
			mwse::Stack::getInstance().pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		auto mobileObject = reference->getAttachedMobileActor();
		if (mobileObject == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetBaseAttribute: Could not find MACP record for reference." << std::endl;
			}
			mwse::Stack::getInstance().pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		// Push the base value of that attribute.
		mwse::Stack::getInstance().pushFloat(mobileObject->attributes[attributeId].base);

		return 0.0f;
	}
}
