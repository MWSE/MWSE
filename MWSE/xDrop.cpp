#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "mwAdapter.h"
#include "VirtualMachine.h"
#include "ScriptUtil.h"

namespace mwse {
	class xDrop : InstructionInterface_t {
	public:
		xDrop();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xDrop xDropInstance;

	xDrop::xDrop() : InstructionInterface_t(OpCode::xDrop) {}

	float xDrop::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get parameters.
		mwseString& id = virtualMachine.getString(stack.popLong());
		long count = stack.popLong();

		// Get reference.
		TES3::Reference* reference = getReference(virtualMachine, __FUNCTION__);
		if (reference == nullptr) {
			return 0.0f;
		}

		// Get spell template by the id.
		TES3::BaseObject* itemTemplate = virtualMachine.getTemplate(id.c_str());
		if (itemTemplate == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xAddItem: No template found with id '" << id << "'." << std::endl;
			}
			return 0.0f;
		}

		// Call the original function.
		TES3::Script* script = virtualMachine.getScript();
		mwscript::Drop(script, reference, itemTemplate, count);

		return 0.0f;
	}
}
