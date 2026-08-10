#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3MobilePlayer.h"
#include "TES3WorldController.h"

namespace mwse {
	class xGetProgressLevel : InstructionInterface_t {
	public:
		xGetProgressLevel();
		virtual float execute(VMExecuteInterface& virtualMachine);
	private:
		const long INVALID_VALUE = -1;
	};

	static xGetProgressLevel xGetProgressLevelInstance;

	xGetProgressLevel::xGetProgressLevel() : mwse::InstructionInterface_t(OpCode::xGetProgressLevel) {}

	float xGetProgressLevel::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get the player's associated MACP record.
		auto mobileObject = TES3::WorldController::get()->getMobilePlayer();
		if (mobileObject == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetProgressLevel: Could not find MACP record for reference." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(INVALID_VALUE);
			return 0.0f;
		}
		else if (mobileObject->objectType != TES3::ObjectType::MobilePlayer) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetProgressLevel: Attached mobile object is not for the player." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(INVALID_VALUE);
			return 0.0f;
		}

		mwse::Stack::getInstance().pushLong(mobileObject->levelUpProgress);

		return 0.0f;
	}
}
