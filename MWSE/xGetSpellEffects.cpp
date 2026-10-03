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
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xGetSpellEffects xGetSpellEffectsInstance;

	xGetSpellEffects::xGetSpellEffects() : InstructionInterface_t(OpCode::xGetSpellEffects) {}

	float xGetSpellEffects::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get parameter.
		mwseString& id = virtualMachine.getString(stack.popLong());

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetSpellEffects: Called on invalid reference." << std::endl;
			}
			stack.pushLong(false);
			return 0.0f;
		}

		// Get spell template by the id.
		TES3::BaseObject* spellTemplate = virtualMachine.getTemplate(id.c_str());
		if (spellTemplate == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetSpellEffects: No template found with id '" << id << "'." << std::endl;
			}
			stack.pushLong(false);
			return 0.0f;
		}

		// Call the original function.
		TES3::Script* script = virtualMachine.getScript();
		bool result = mwscript::GetSpellEffects(script, reference, spellTemplate);

		stack.pushLong(result);

		return 0.0f;
	}
}
