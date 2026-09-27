#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3DataHandler.h"
#include "TES3Cell.h"

namespace mwse {
	class xPCCellID : InstructionInterface_t {
	public:
		xPCCellID();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xPCCellID xPCCellIDInstance;

	xPCCellID::xPCCellID() : InstructionInterface_t(OpCode::xPCCellID) {}

	float xPCCellID::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		TES3::DataHandler* masterCell = TES3::DataHandler::get();
		constexpr auto defaultCellName = "Wilderness";
		if (masterCell == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xPCCellID: Cell master could not be found." << std::endl;
			}
			stack.pushString(defaultCellName);
			return 0.0f;
		}

		// Determine the PC cell -- either the interior cell if we have one, or the center cell.
		TES3::Cell* cell = masterCell->currentInteriorCell;
		if (cell == nullptr) {
			cell = masterCell->exteriorCellData[TES3::CellGrid::Center]->cell;
		}

		// If the cell has a name, use it. If not we want to use the literal
		// "Wilderness" so that PositionCell behaves properly.
		if (cell->name) {
			stack.pushString(cell->name);
		}
		else {
			stack.pushString(defaultCellName);
		}

		return 0.0f;
	}
}
