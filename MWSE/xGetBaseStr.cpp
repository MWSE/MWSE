#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3MobileNPC.h"
#include "TES3Reference.h"

namespace mwse {
	class xGetBaseStr : InstructionInterface_t {
	public:
		xGetBaseStr();
		virtual float execute(VMExecuteInterface& virtualMachine);
	private:
		const float INVALID_VALUE = -1.0f;
	};

	static xGetBaseStr xGetBaseStrInstance;

	xGetBaseStr::xGetBaseStr() : mwse::InstructionInterface_t(OpCode::xGetBaseStr) {}

	float xGetBaseStr::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get the associated MACP record.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetBaseStr: No reference provided." << std::endl;
			}
			mwse::Stack::getInstance().pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		auto mobileObject = reference->getAttachedMobileActor();
		if (mobileObject == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetBaseStr: Could not find MACP record for reference." << std::endl;
			}
			mwse::Stack::getInstance().pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		// Push the current value of that attribute.
		mwse::Stack::getInstance().pushFloat(mobileObject->attributes[TES3::Attribute::Strength].base);

		return 0.0f;
	}
}
