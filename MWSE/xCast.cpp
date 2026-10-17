#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "mwAdapter.h"
#include "VirtualMachine.h"
#include "ScriptUtil.h"

#include "TES3DataHandler.h"

namespace mwse {
	class xCast : InstructionInterface_t {
	public:
		xCast();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xCast xCastInstance;

	xCast::xCast() : InstructionInterface_t(OpCode::xCast) {}

	float xCast::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get parameters.
		mwseString& spellId = virtualMachine.getString(stack.popLong());
		mwseString& targetId = virtualMachine.getString(stack.popLong());

		// Get reference.
		TES3::Reference* reference = getReference(virtualMachine, __FUNCTION__);
		if (reference == nullptr) {
			return 0.0f;
		}

		// Get spell template by the id.
		TES3::Spell* spell = TES3::DataHandler::get()->nonDynamicData->getSpellById(spellId.c_str());
		if (spell == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xCast: No template found with id '" << spellId << "'." << std::endl;
			}
			return 0.0f;
		}

		// Get the target by id.
		TES3::Reference* target = virtualMachine.getReference(targetId.c_str());
		if (target == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xCast: Could not find valid target by id '" << targetId << "'." << std::endl;
			}
			return 0.0f;
		}

		// This function isn't working yet.
		TES3::Script* script = virtualMachine.getScript();
		log::getLog() << "xCast: Function unimplemented." << std::endl;

		return 0.0f;
	}
}
