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
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xGetAlchemyInfo xGetAlchemyInfoInstance;

	xGetAlchemyInfo::xGetAlchemyInfo() : InstructionInterface_t(OpCode::xGetAlchemyInfo) {}

	float xGetAlchemyInfo::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get parameters.
		mwseString& id = virtualMachine.getString(stack.popLong());

		// Get the record by its id.
		const auto record = TES3::DataHandler::get()->nonDynamicData->resolveObjectByType<TES3::Alchemy>(id);
		if (record == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetAlchemyInfo: No record found by id '" << id << "'." << std::endl;
			}
			stack.pushLong(0);
			stack.pushLong(0);
			return 0.0f;
		}
		else if (record->objectType != TES3::ObjectType::Alchemy) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetAlchemyInfo: Found record by id '" << id << "' of invalid type " << record->objectType << "." << std::endl;
			}
			stack.pushLong(0);
			stack.pushLong(0);
			return 0.0f;
		}

		long flags = record->flags;
		long effectCount = record->getActiveEffectCount();

		stack.pushLong(flags);
		stack.pushLong(effectCount);

		return 0.0f;
	}
}
