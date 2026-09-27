#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3DataHandler.h"
#include "TES3MagicEffect.h"
#include "TES3MagicEffectController.h"

namespace mwse {
	class xGetBaseEffectInfo : InstructionInterface_t {
	public:
		xGetBaseEffectInfo();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xGetBaseEffectInfo xGetBaseEffectInfoInstance;

	xGetBaseEffectInfo::xGetBaseEffectInfo() : mwse::InstructionInterface_t(OpCode::xGetBaseEffectInfo) {}

	float xGetBaseEffectInfo::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameters.
		long id = Stack::getInstance().popLong();

		// Validate id.
		if (id < TES3::EffectID::FirstEffect || id > TES3::EffectID::LastEffect) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetBaseEffectInfo: Effect ID out of range." << std::endl;
			}
			Stack::getInstance().pushLong(0);
			Stack::getInstance().pushFloat(0.0f);
			Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		// Get the effect.
		auto nonDynamicData = TES3::DataHandler::get()->nonDynamicData;
		TES3::MagicEffect* effect = nonDynamicData->getMagicEffect(id);

		// Flags are a unique case. There is other data associated with flags that we want
		// to expose, so we will return it here.
		// TODO: Programmatically allow setting/getting these normally hard-coded values.
		Stack::getInstance().pushLong(effect->flags | nonDynamicData->magicEffects->getEffectFlags(id));

		// Push other results.
		Stack::getInstance().pushFloat(effect->baseMagickaCost);
		Stack::getInstance().pushLong(effect->school);

		return 0.0f;
	}
}
