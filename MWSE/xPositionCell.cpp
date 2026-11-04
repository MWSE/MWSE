#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "mwAdapter.h"
#include "VirtualMachine.h"
#include "ScriptUtil.h"

namespace mwse {
	class xPositionCell : InstructionInterface_t {
	public:
		xPositionCell();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xPositionCell xPositionCellInstance;

	xPositionCell::xPositionCell() : InstructionInterface_t(OpCode::xPositionCell) {}

	float xPositionCell::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get parameters.
		float x = stack.popFloat();
		float y = stack.popFloat();
		float z = stack.popFloat();
		float rotation = stack.popFloat();
		mwseString& cell = virtualMachine.getString(stack.popLong());

		// Get other context information for original opcode.
		TES3::Reference* reference = getReference(virtualMachine, __FUNCTION__);
		if (reference == nullptr) {
			return 0.0f;
		}

		// Call the original function.
		TES3::Script* script = virtualMachine.getScript();
		mwscript::PositionCell(script, reference, x, y, z, rotation, cell.c_str());

		return 0.0f;
	}
}
