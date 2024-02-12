#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3Alchemy.h"
#include "TES3DataHandler.h"

namespace mwse {
	class xGetAlchemyInfo : InstructionInterface_t {
	public:
		xGetAlchemyInfo();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xGetAlchemyInfo xGetAlchemyInfoInstance;

	xGetAlchemyInfo::xGetAlchemyInfo() : mwse::InstructionInterface_t(OpCode::xGetAlchemyInfo) {}

	float xGetAlchemyInfo::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameters.
		mwseString& id = virtualMachine.getString(Stack::getInstance().popLong());

		// Get the record by its id.
		const auto record = TES3::DataHandler::get()->nonDynamicData->resolveObjectByType<TES3::Alchemy>(id);
		if (record == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetAlchemyInfo: No record found by id '" << id << "'." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}
		else if (record->objectType != TES3::ObjectType::Alchemy) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetAlchemyInfo: Found record by id '" << id << "' of invalid type " << record->objectType << "." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		long flags = record->flags;
		long effectCount = record->getActiveEffectCount();

		mwse::Stack::getInstance().pushLong(flags);
		mwse::Stack::getInstance().pushLong(effectCount);

		return 0.0f;
	}
}
