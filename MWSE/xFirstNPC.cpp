#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3DataHandler.h"
#include "TES3Cell.h"

namespace mwse {
	class xFirstNPC : InstructionInterface_t {
	public:
		xFirstNPC();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xFirstNPC xFirstNPCInstance;

	xFirstNPC::xFirstNPC() : mwse::InstructionInterface_t(OpCode::xFirstNPC) {}

	float xFirstNPC::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Clear elements in our stored exterior ref list.
		mwse::tes3::clearExteriorRefs();

		TES3::Reference* reference = NULL;
		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler->currentInteriorCell != NULL) {
			reference = static_cast<TES3::Reference*>(dataHandler->currentInteriorCell->actors.head->skipDeletedObjects());
		}
		else {
			auto cellPointer = dataHandler->exteriorCellData[TES3::CellGrid::Center];
			if (cellPointer->isFullyLoaded()) {
				// Get the start of the list for the center cell. We'll check that it's valid later.
				reference = static_cast<TES3::Reference*>(cellPointer->cell->actors.head->skipDeletedObjects());
				int exteriorCount = 0;
				for (int i = 0; i < 9; ++i) {
					if (i == TES3::CellGrid::Center) {
						continue;
					}

					cellPointer = dataHandler->exteriorCellData[i];
					if (cellPointer->isFullyLoaded()) {
						TES3::Reference* tempReference = static_cast<TES3::Reference*>(cellPointer->cell->actors.head->skipDeletedObjects());
						if (tempReference != NULL) {
							mwse::tes3::exteriorRefs[exteriorCount] = tempReference;
							exteriorCount++;
						}
					}
				}

				// Make sure that we end our list with a NULL, so we know we're done.
				mwse::tes3::exteriorRefs[exteriorCount] = NULL;

				// Make sure the reference in the center cell is valid.
				// If not, use the reference from another exterior cell.
				if (reference == NULL && exteriorCount > 0) {
					exteriorCount--;
					reference = mwse::tes3::exteriorRefs[exteriorCount];
					mwse::tes3::exteriorRefs[exteriorCount] = NULL;
				}
			}
		}

		mwse::Stack::getInstance().pushLong((long)reference);

		return 0.0f;
	}
}
