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
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xSetProgressLevel xSetProgressLevelInstance;

	xSetProgressLevel::xSetProgressLevel() : InstructionInterface_t(OpCode::xSetProgressLevel) {}

	float xSetProgressLevel::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get parameter.
		long progress = stack.popLong();

		// Get the associated MACP record.
		auto mobileObject = TES3::WorldController::get()->getMobilePlayer();
		if (mobileObject == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xSetProgressLevel: Could not find MACP record for reference." << std::endl;
			}
			stack.pushLong(false);
			return 0.0f;
		}

		// Set progress.
		mobileObject->levelUpProgress = progress;

		// Check for level up.
		tes3::checkForLevelUp(progress);

		// Push success.
		stack.pushLong(true);

		return 0.0f;
	}
}
