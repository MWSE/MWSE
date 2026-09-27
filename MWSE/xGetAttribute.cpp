#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse {
	class xGetAttribute : InstructionInterface_t {
	public:
		xGetAttribute();
		float execute(VMExecuteInterface& virtualMachine) override;
	private:
		const float INVALID_VALUE = -1.0f;
	};

	static xGetAttribute xGetAttributeInstance;

	xGetAttribute::xGetAttribute() : mwse::InstructionInterface_t(OpCode::xGetAttribute) {}

	float xGetAttribute::execute(mwse::VMExecuteInterface& virtualMachine) {
		if (mwse::Stack::getInstance().size() < 1) {
			mwse::log::getLog() << "xGetAttribute: Function called with too few arguments." << std::endl;
			return 0.0f;
		}

		// Get attribute index as parameter.
		long attributeId = mwse::Stack::getInstance().popLong();
		if (attributeId < TES3::Attribute::FirstAttribute || attributeId > TES3::Attribute::LastAttribute) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetAttribute: Invalid attribute id: " << attributeId << std::endl;
			}
			mwse::Stack::getInstance().pushFloat(INVALID_VALUE);
			return 0.0f;

		}

		// Get the associated MACP record.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetAttribute: No reference provided." << std::endl;
			}
			mwse::Stack::getInstance().pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		TES3::MobileActor* mobileObject = reference->getAttachedMobileActor();
		if (mobileObject == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetAttribute: Could not find MACP record for reference." << std::endl;
			}
			mwse::Stack::getInstance().pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		// Push the current value of that attribute.
		mwse::Stack::getInstance().pushFloat(mobileObject->attributes[attributeId].current);

		return 0.0f;
	}
}
