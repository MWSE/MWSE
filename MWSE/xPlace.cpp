#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "mwAdapter.h"
#include "VirtualMachine.h"
#include "ScriptUtil.h"

namespace mwse {
	class xPlace : InstructionInterface_t {
	public:
		xPlace();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xPlace xPlaceInstance;

	xPlace::xPlace() : mwse::InstructionInterface_t(OpCode::xPlace) {}

	float xPlace::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameters.
		mwseString& id = virtualMachine.getString(mwse::Stack::getInstance().popLong());

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference("player");
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xPlace: Called on invalid reference." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		// Get the template we're supposed to place.
		TES3::BaseObject* templateToPlace = virtualMachine.getTemplate(id.c_str());
		if (templateToPlace == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xPlace: No template found for id '" << id << "'." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		// Call the original function.
		TES3::Script* script = virtualMachine.getScript();
		mwse::mwscript::PlaceAtPC(script, reference, templateToPlace, 1, 256.0f, 1);

		// Push back the reference we created.
		TES3::Reference* createdReference = mwse::mwscript::lastCreatedPlaceAtPCReference;
		mwse::Stack::getInstance().pushLong((long)createdReference);

		return 0.0f;
	}
}
