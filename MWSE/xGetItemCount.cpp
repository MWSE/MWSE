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
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xGetItemCount xGetItemCountInstance;

	xGetItemCount::xGetItemCount() : mwse::InstructionInterface_t(OpCode::xGetItemCount) {}

	float xGetItemCount::execute(mwse::VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get parameter.
		mwseString& id = virtualMachine.getString(stack.popLong());

		// Get who we're getting the item count of. mwscript's GetItemCount validates the
		// object type for us, we don't need to.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetItemCount: No reference found for function call." << std::endl;
			}
			stack.pushLong(0);
			return 0.0f;
		}

		// Get template for the item we want to get the count of.
		TES3::BaseObject* itemTemplate = virtualMachine.getTemplate(id.c_str());
		if (itemTemplate == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetItemCount: No template found with id " << id << "." << std::endl;
			}
			stack.pushLong(0);
			return 0.0f;
		}

		// Call the original function.
		TES3::Script* script = virtualMachine.getScript();
		long result = mwse::mwscript::GetItemCount(script, reference, itemTemplate);
		stack.pushLong(result);

		return 0.0f;
	}
}
