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
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xPositionCell xPositionCellInstance;

	xPositionCell::xPositionCell() : mwse::InstructionInterface_t(OpCode::xPositionCell) {}

	float xPositionCell::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameters.
		float x = mwse::Stack::getInstance().popFloat();
		float y = mwse::Stack::getInstance().popFloat();
		float z = mwse::Stack::getInstance().popFloat();
		float rotation = mwse::Stack::getInstance().popFloat();
		mwseString& cell = virtualMachine.getString(mwse::Stack::getInstance().popLong());

		// Get other context information for original opcode.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xPositionCell: Called on invalid reference." << std::endl;
			}
			return 0.0f;
		}

		// Call the original function.
		TES3::Script* script = virtualMachine.getScript();
		mwse::mwscript::PositionCell(script, reference, x, y, z, rotation, cell.c_str());

		return 0.0f;
	}
}
