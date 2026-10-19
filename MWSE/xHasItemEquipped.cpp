#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "mwAdapter.h"
#include "VirtualMachine.h"
#include "ScriptUtil.h"

namespace mwse {
	class xHasItemEquipped : InstructionInterface_t {
	public:
		xHasItemEquipped();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xHasItemEquipped xHasItemEquippedInstance;

	xHasItemEquipped::xHasItemEquipped() : InstructionInterface_t(OpCode::xHasItemEquipped) {}

	float xHasItemEquipped::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get parameters.
		mwseString& id = virtualMachine.getString(stack.popLong());

		// Get script reference.
		TES3::Reference* reference = getReference(virtualMachine, __FUNCTION__);
		if (reference == nullptr) {
			stack.pushLong(false);
			return 0.0f;
		}

		// Get item template.
		TES3::BaseObject* itemTemplate = virtualMachine.getTemplate(id.c_str());
		if (itemTemplate == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xHasItemEquipped: No template found with id '" << id << "'." << std::endl;
			}
			stack.pushLong(false);
			return 0.0f;
		}

		// Call the original function.
		TES3::Script* script = virtualMachine.getScript();
		bool result = mwscript::HasItemEquipped(script, reference, itemTemplate);

		stack.pushLong(result);

		return 0.0f;
	}
}
