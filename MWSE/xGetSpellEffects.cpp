#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "mwAdapter.h"
#include "VirtualMachine.h"
#include "ScriptUtil.h"

namespace mwse {
	class xGetSpellEffects : InstructionInterface_t {
	public:
		xGetSpellEffects();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xGetSpellEffects xGetSpellEffectsInstance;

	xGetSpellEffects::xGetSpellEffects() : mwse::InstructionInterface_t(OpCode::xGetSpellEffects) {}

	float xGetSpellEffects::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameter.
		mwseString& id = virtualMachine.getString(mwse::Stack::getInstance().popLong());

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetSpellEffects: Called on invalid reference." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Get spell template by the id.
		TES3::BaseObject* spellTemplate = virtualMachine.getTemplate(id.c_str());
		if (spellTemplate == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetSpellEffects: No template found with id '" << id << "'." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Call the original function.
		TES3::Script* script = virtualMachine.getScript();
		bool result = mwse::mwscript::GetSpellEffects(script, reference, spellTemplate);

		mwse::Stack::getInstance().pushLong(result);

		return 0.0f;
	}
}
