#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3Reference.h"

namespace mwse {
	class xGetQuality : InstructionInterface_t {
	public:
		xGetQuality();
		virtual float execute(VMExecuteInterface& virtualMachine);
	private:
		const float INVALID_VALUE = -1.0f;
	};

	static xGetQuality xGetQualityInstance;

	xGetQuality::xGetQuality() : mwse::InstructionInterface_t(OpCode::xGetQuality) {}

	float xGetQuality::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (!reference) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetQuality: No reference provided." << std::endl;
			}
			mwse::Stack::getInstance().pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		// Get record.
		TES3::Object* object = reference->baseObject;
		if (!object) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetQuality: No base record found." << std::endl;
			}
			mwse::Stack::getInstance().pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		mwse::Stack::getInstance().pushFloat(object->vTable.object->getQuality(object));

		return 0.0f;
	}
}
