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
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xSetLevel xSetLevelInstance;

	xSetLevel::xSetLevel() : InstructionInterface_t(OpCode::xSetLevel) {}

	float xSetLevel::execute(VMExecuteInterface& virtualMachine) {
		// Get parameters.
		short level = Stack::getInstance().popShort();

		// Get reference.
		TES3::Reference* reference = getReference(virtualMachine, __FUNCTION__);
		if (reference == nullptr) {
			return 0.0f;
		}

		// Call the original function.
		TES3::Script* script = virtualMachine.getScript();
		mwscript::SetLevel(script, reference, level);

		return 0.0f;
	}
}
