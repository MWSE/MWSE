#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "mwAdapter.h"
#include "VirtualMachine.h"
#include "ScriptUtil.h"

namespace mwse {
	class xGetItemCount : InstructionInterface_t {
	public:
		xGetItemCount();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xGetItemCount xGetItemCountInstance;

	xGetItemCount::xGetItemCount() : mwse::InstructionInterface_t(OpCode::xGetItemCount) {}

	float xGetItemCount::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameter.
		mwseString& id = virtualMachine.getString(mwse::Stack::getInstance().popLong());

		// Get who we're getting the item count of. mwscript's GetItemCount validates the
		// object type for us, we don't need to.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetItemCount: No reference found for function call." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		// Get template for the item we want to get the count of.
		TES3::BaseObject* itemTemplate = virtualMachine.getTemplate(id.c_str());
		if (itemTemplate == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetItemCount: No template found with id " << id << "." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		// Call the original function.
		TES3::Script* script = virtualMachine.getScript();
		long result = mwse::mwscript::GetItemCount(script, reference, itemTemplate);
		mwse::Stack::getInstance().pushLong(result);

		return 0.0f;
	}
}
