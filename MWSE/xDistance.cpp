#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Reference.h"

namespace mwse {
	class xDistance : InstructionInterface_t {
	public:
		xDistance();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xDistance xDistanceInstance;

	xDistance::xDistance() : mwse::InstructionInterface_t(OpCode::xDistance) {}

	float xDistance::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get target reference
		TES3::Reference* targetref = reinterpret_cast<TES3::Reference*>(mwse::Stack::getInstance().popLong());
		if (!targetref) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xModProgressSkill: Target reference is invalid." << std::endl;
			}
			mwse::Stack::getInstance().pushFloat(0.0f);
			return 0.0f;
		}

		// Get script reference.
		TES3::Reference* thisref = virtualMachine.getReference();
		if (!thisref) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xModProgressSkill: Script reference is invalid." << std::endl;
			}
			mwse::Stack::getInstance().pushFloat(0.0f);
			return 0.0f;
		}

		float dx = targetref->position.x - thisref->position.x;
		float dy = targetref->position.y - thisref->position.y;
		float dz = targetref->position.z - thisref->position.z;
		float xDistance = std::sqrt(dx * dx + dy * dy + dz * dz);

		mwse::Stack::getInstance().pushFloat(xDistance);

		return 0.0f;
	}
}
