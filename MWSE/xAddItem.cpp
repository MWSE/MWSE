#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "mwAdapter.h"
#include "VirtualMachine.h"
#include "ScriptUtil.h"

namespace mwse {
	class xAddItem : InstructionInterface_t {
	public:
		xAddItem();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xAddItem xAddItemInstance;

	xAddItem::xAddItem() : mwse::InstructionInterface_t(OpCode::xAddItem) {}

	float xAddItem::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameters.
		mwseString& id = virtualMachine.getString(mwse::Stack::getInstance().popLong());
		long count = mwse::Stack::getInstance().popLong();

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xAddItem: Called on invalid reference." << std::endl;
			}
			return 0.0f;
		}

		// Get spell template by the id.
		TES3::BaseObject* itemTemplate = virtualMachine.getTemplate(id.c_str());
		if (itemTemplate == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xAddItem: No template found with id '" << id << "'." << std::endl;
			}
			return 0.0f;
		}

		// Call the original function.
		TES3::Script* script = virtualMachine.getScript();
		mwse::mwscript::AddItem(script, reference, itemTemplate, count);

		return 0.0f;
	}
}
