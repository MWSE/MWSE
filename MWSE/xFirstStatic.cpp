#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3DataHandler.h"
#include "TES3Cell.h"

namespace mwse {
	class xFirstStatic : InstructionInterface_t {
	public:
		xFirstStatic();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xFirstStatic xFirstStaticInstance;

	xFirstStatic::xFirstStatic() : mwse::InstructionInterface_t(OpCode::xFirstStatic) {}

	float xFirstStatic::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Clear elements in our stored exterior ref list.
		mwse::tes3::clearExteriorRefs();

		TES3::Reference* reference = NULL;
		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler->currentInteriorCell != NULL) {
			reference = static_cast<TES3::Reference*>(dataHandler->currentInteriorCell->persistentRefs.head->skipDeletedObjects());
		}
		else {
			auto cellPointer = dataHandler->exteriorCellData[TES3::CellGrid::Center];
			if (cellPointer->loadingFlags >= 1) {
				// Get the start of the list for the center cell. We'll check that it's valid later.
				reference = static_cast<TES3::Reference*>(cellPointer->cell->persistentRefs.head->skipDeletedObjects());
				int exteriorCount = 0;
				for (int i = 0; i < 9; ++i) {
					if (i == TES3::CellGrid::Center) {
						continue;
					}

					cellPointer = dataHandler->exteriorCellData[i];
					if (cellPointer->loadingFlags >= 1) {
						TES3::Reference* tempReference = static_cast<TES3::Reference*>(cellPointer->cell->persistentRefs.head->skipDeletedObjects());
						if (tempReference != NULL) {
							mwse::tes3::exteriorRefs[exteriorCount] = tempReference;
							exteriorCount++;
						}
					}
					else {
						mwse::log::getLog() << "xFirstStatic: Exterior flags is " << cellPointer->loadingFlags << ". Skipping exterior " << i << "." << std::endl;
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
