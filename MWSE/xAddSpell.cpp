#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "mwAdapter.h"
#include "VirtualMachine.h"
#include "ScriptUtil.h"

namespace mwse {
	class xAddSpell : InstructionInterface_t {
	public:
		xAddSpell();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xAddSpell xAddSpellInstance;

	xAddSpell::xAddSpell() : InstructionInterface_t(OpCode::xAddSpell) {}

	float xAddSpell::execute(VMExecuteInterface& virtualMachine) {
		// Get parameter.
		mwseString& id = virtualMachine.getString(Stack::getInstance().popLong());

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xAddSpell: Called on invalid reference." << std::endl;
			}
			return 0.0f;
		}

		// Get spell template by the id.
		TES3::BaseObject* spellTemplate = virtualMachine.getTemplate(id.c_str());
		if (spellTemplate == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xAddSpell: No template found with id '" << id << "'." << std::endl;
			}
			return 0.0f;
		}

		// Call the original function.
		TES3::Script* script = virtualMachine.getScript();
		mwscript::AddSpell(script, reference, spellTemplate);

		return 0.0f;
	}
}
