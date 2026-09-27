#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3MobilePlayer.h"
#include "TES3WorldController.h"

namespace mwse {
	class xModProgressLevel : InstructionInterface_t {
	public:
		xModProgressLevel();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xModProgressLevel xModProgressLevelInstance;

	xModProgressLevel::xModProgressLevel() : mwse::InstructionInterface_t(OpCode::xModProgressLevel) {}

	float xModProgressLevel::execute(mwse::VMExecuteInterface& virtualMachine) {
		if (mwse::Stack::getInstance().size() < 1) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xModProgressLevel: Function called with too few arguments." << std::endl;
			}
			return 0.0f;
		}

		long modValue = mwse::Stack::getInstance().popLong();

		// Get the MACP record.
		auto mobileObject = TES3::WorldController::get()->getMobilePlayer();

		// Modify value.
		long newValue = mobileObject->levelUpProgress + modValue;
		if (newValue < 0) {
			newValue = 0;
		}
		mobileObject->levelUpProgress = newValue;

		// Push to indicate success.
		mwse::Stack::getInstance().pushLong(true);

		return 0.0f;
	}
}
