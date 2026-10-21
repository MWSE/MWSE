#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "mwAdapter.h"
#include "VirtualMachine.h"
#include "ScriptUtil.h"

namespace mwse {
	class xRemoveSpell : InstructionInterface_t {
	public:
		xRemoveSpell();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xRemoveSpell xRemoveSpellInstance;

	xRemoveSpell::xRemoveSpell() : InstructionInterface_t(OpCode::xRemoveSpell) {}

	float xRemoveSpell::execute(VMExecuteInterface& virtualMachine) {
		// Get parameters.
		mwseString& id = virtualMachine.getString(Stack::getInstance().popLong());

		// Get reference.
		TES3::Reference* reference = getReference(virtualMachine, __FUNCTION__);
		if (reference == nullptr) {
			return 0.0f;
		}

		// Get spell template by the id.
		TES3::BaseObject* spellTemplate = virtualMachine.getTemplate(id.c_str());
		if (spellTemplate == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xRemoveSpell: No template found with id '" << id << "'." << std::endl;
			}
			return 0.0f;
		}

		// Call the original function.
		TES3::Script* script = virtualMachine.getScript();
		mwscript::RemoveSpell(script, reference, spellTemplate);

		return 0.0f;
	}
}
