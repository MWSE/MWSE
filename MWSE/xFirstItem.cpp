#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3DataHandler.h"
#include "TES3Cell.h"
#include "TES3Reference.h"

namespace mwse {
	class xFirstItem : InstructionInterface_t {
	public:
		xFirstItem();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xFirstItem xFirstItemInstance;

	xFirstItem::xFirstItem() : InstructionInterface_t(OpCode::xFirstItem) {}

	float xFirstItem::execute(VMExecuteInterface& virtualMachine) {
		// Clear elements in our stored exterior ref list.
		tes3::clearExteriorRefs();

		TES3::Reference* reference = nullptr;
		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler->currentInteriorCell != nullptr) {
			reference = static_cast<TES3::Reference*>(dataHandler->currentInteriorCell->temporaryRefs.head->skipDeletedObjects());
		}
		else {
			auto cellPointer = dataHandler->exteriorCellData[TES3::CellGrid::Center];
			if (cellPointer->isFullyLoaded()) {
				// Get the start of the list for the center cell. We'll check that it's valid later.
				reference = static_cast<TES3::Reference*>(cellPointer->cell->temporaryRefs.head->skipDeletedObjects());
				int exteriorCount = 0;
				for (int i = 0; i < 9; ++i) {
					if (i == TES3::CellGrid::Center) {
						continue;
					}

					cellPointer = dataHandler->exteriorCellData[i];
					if (cellPointer->isFullyLoaded()) {
						TES3::Reference* tempReference = static_cast<TES3::Reference*>(cellPointer->cell->temporaryRefs.head->skipDeletedObjects());
						if (tempReference != nullptr) {
							tes3::exteriorRefs[exteriorCount] = tempReference;
							exteriorCount++;
						}
					}
				}

				// Make sure that we end our list with a nullptr, so we know we're done.
				tes3::exteriorRefs[exteriorCount] = nullptr;

				// Make sure the reference in the center cell is valid.
				// If not, use the reference from another exterior cell.
				if (reference == nullptr && exteriorCount > 0) {
					exteriorCount--;
					reference = tes3::exteriorRefs[exteriorCount];
					tes3::exteriorRefs[exteriorCount] = nullptr;
				}
			}
		}

		Stack::getInstance().pushLong((long)reference);

		return 0.0f;
	}
}
