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
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xAddSpell xAddSpellInstance;

	xAddSpell::xAddSpell() : mwse::InstructionInterface_t(OpCode::xAddSpell) {}

	float xAddSpell::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameter.
		mwseString& id = virtualMachine.getString(mwse::Stack::getInstance().popLong());

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xAddSpell: Called on invalid reference." << std::endl;
			}
			return 0.0f;
		}

		// Get spell template by the id.
		TES3::BaseObject* spellTemplate = virtualMachine.getTemplate(id.c_str());
		if (spellTemplate == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xAddSpell: No template found with id '" << id << "'." << std::endl;
			}
			return 0.0f;
		}

		// Call the original function.
		TES3::Script* script = virtualMachine.getScript();
		mwse::mwscript::AddSpell(script, reference, spellTemplate);

		return 0.0f;
	}
}
