#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3DataHandler.h"
#include "TES3GameSetting.h"

namespace mwse {
	class xGetGSString : InstructionInterface_t {
	public:
		xGetGSString();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xGetGSString xGetGSStringInstance;

	xGetGSString::xGetGSString() : InstructionInterface_t(OpCode::xGetGSString) {}

	float xGetGSString::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		long gmstId = stack.popLong();

		if (gmstId < TES3::GMST::FirstGMST || gmstId > TES3::GMST::LastGMST) {
			log::getLog() << "xGetGSString: Invalid GMST id." << std::endl;
			stack.pushLong(NULL);
			return 0.0f;
		}

		// Get the string. No real sanity checks here...
		char* value = TES3::DataHandler::get()->nonDynamicData->GMSTs[gmstId]->value.asString;

		stack.pushString(value);

		return 0.0f;
	}
}
