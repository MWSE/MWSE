#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3MobilePlayer.h"
#include "TES3WorldController.h"

namespace mwse {
	class xSetProgressLevel : InstructionInterface_t {
	public:
		xSetProgressLevel();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xSetProgressLevel xSetProgressLevelInstance;

	xSetProgressLevel::xSetProgressLevel() : mwse::InstructionInterface_t(OpCode::xSetProgressLevel) {}

	float xSetProgressLevel::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameter.
		long progress = mwse::Stack::getInstance().popLong();

		// Get the associated MACP record.
		auto mobileObject = TES3::WorldController::get()->getMobilePlayer();
		if (mobileObject == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetProgressLevel: Could not find MACP record for reference." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Set progress.
		mobileObject->levelUpProgress = progress;

		// Check for level up.
		tes3::checkForLevelUp(progress);

		// Push success.
		mwse::Stack::getInstance().pushLong(true);

		return 0.0f;
	}
}
