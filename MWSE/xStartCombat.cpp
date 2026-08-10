#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "mwAdapter.h"
#include "VirtualMachine.h"
#include "ScriptUtil.h"

namespace mwse {
	class xStartCombat : InstructionInterface_t {
	public:
		xStartCombat();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xStartCombat xStartCombatInstance;

	xStartCombat::xStartCombat() : mwse::InstructionInterface_t(OpCode::xStartCombat) {}

	float xStartCombat::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameters.
		TES3::Reference* target = reinterpret_cast<TES3::Reference*>(mwse::Stack::getInstance().popLong());

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xStartCombat: Called on invalid reference." << std::endl;
			}
			return 0.0f;
		}

		// Call the original function.
		TES3::Script* script = virtualMachine.getScript();
		mwse::mwscript::StartCombat(script, reference, target);

		return 0.0f;
	}
}
