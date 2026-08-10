#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "mwAdapter.h"
#include "VirtualMachine.h"
#include "ScriptUtil.h"

namespace mwse {
	class xPosition : InstructionInterface_t {
	public:
		xPosition();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xPosition xPositionInstance;

	xPosition::xPosition() : mwse::InstructionInterface_t(OpCode::xPosition) {}

	float xPosition::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameters.
		float x = mwse::Stack::getInstance().popFloat();
		float y = mwse::Stack::getInstance().popFloat();
		float z = mwse::Stack::getInstance().popFloat();
		float rotation = mwse::Stack::getInstance().popFloat();

		// Get other context information for original opcode.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xPosition: Called on invalid reference." << std::endl;
			}
			return 0.0f;
		}

		// Call the original function.
		TES3::Script* script = virtualMachine.getScript();
		mwse::mwscript::Position(script, reference, x, y, z, rotation);

		return 0.0f;
	}
}
