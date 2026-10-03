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
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xStartCombat xStartCombatInstance;

	xStartCombat::xStartCombat() : InstructionInterface_t(OpCode::xStartCombat) {}

	float xStartCombat::execute(VMExecuteInterface& virtualMachine) {
		// Get parameters.
		TES3::Reference* target = Stack::getInstance().popPointer<TES3::Reference*>();

		// Get reference.
		TES3::Reference* reference = getReference(virtualMachine, __FUNCTION__);
		if (reference == nullptr) {
			return 0.0f;
		}

		// Call the original function.
		TES3::Script* script = virtualMachine.getScript();
		mwscript::StartCombat(script, reference, target);

		return 0.0f;
	}
}
