#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "mwAdapter.h"
#include "VirtualMachine.h"
#include "ScriptUtil.h"


namespace mwse {
	class xEquip : InstructionInterface_t {
	public:
		xEquip();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xEquip xEquipInstance;

	xEquip::xEquip() : InstructionInterface_t(OpCode::xEquip) {}

	float xEquip::execute(VMExecuteInterface& virtualMachine) {
		// Get parameters.
		mwseString& id = virtualMachine.getString(Stack::getInstance().popLong());

		// Get reference.
		TES3::Reference* reference = getReference(virtualMachine, __FUNCTION__);
		if (reference == nullptr) {
			return 0.0f;
		}

		// Get spell template by the id.
		TES3::BaseObject* itemTemplate = virtualMachine.getTemplate(id.c_str());
		if (itemTemplate == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xAddSpell: No template found with id '" << id << "'." << std::endl;
			}
			return 0.0f;
		}

		// Call the original function.
		TES3::Script* script = virtualMachine.getScript();
		mwscript::Equip(script, reference, itemTemplate);

		return 0.0f;
	}
}
