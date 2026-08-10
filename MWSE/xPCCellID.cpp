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
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xPCCellID xPCCellIDInstance;

	xPCCellID::xPCCellID() : mwse::InstructionInterface_t(OpCode::xPCCellID) {}

	float xPCCellID::execute(mwse::VMExecuteInterface& virtualMachine) {
		TES3::DataHandler* masterCell = TES3::DataHandler::get();
		if (masterCell == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xPCCellID: Cell master could not be found." << std::endl;
			}
			mwse::Stack::getInstance().pushString("Wilderness");
			return 0.0f;
		}

		// Determine the PC cell -- either the interior cell if we have one, or the center cell.
		TES3::Cell* cell = masterCell->currentInteriorCell;
		if (cell == NULL) {
			cell = masterCell->exteriorCellData[TES3::CellGrid::Center]->cell;
		}

		// If the cell has a name, use it. If not we want to use the literal
		// "Wilderness" so that PositionCell behaves properly.
		if (cell->name) {
			mwse::Stack::getInstance().pushString(cell->name);
		}
		else {
			mwse::Stack::getInstance().pushString("Wilderness");
		}

		return 0.0f;
	}
}
