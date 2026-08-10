#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "mwAdapter.h"
#include "VirtualMachine.h"
#include "ScriptUtil.h"

namespace mwse {
	class xSetLevel : InstructionInterface_t {
	public:
		xSetLevel();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xSetLevel xSetLevelInstance;

	xSetLevel::xSetLevel() : mwse::InstructionInterface_t(OpCode::xSetLevel) {}

	float xSetLevel::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameters.
		short level = mwse::Stack::getInstance().popShort();

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetLevel: Called on invalid reference." << std::endl;
			}
			return 0.0f;
		}

		// Call the original function.
		TES3::Script* script = virtualMachine.getScript();
		mwse::mwscript::SetLevel(script, reference, level);

		return 0.0f;
	}
}
