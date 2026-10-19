#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "MemoryUtil.h"

#include "TES3DataHandler.h"
#include "TES3GameSetting.h"

namespace mwse {
	class xSetGSString : InstructionInterface_t {
	public:
		xSetGSString();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xSetGSString xSetGSStringInstance;

	xSetGSString::xSetGSString() : InstructionInterface_t(OpCode::xSetGSString) {}

	float xSetGSString::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		long gmstId = stack.popLong();
		mwseString& newString = virtualMachine.getString(stack.popLong());

		if (gmstId < TES3::GMST::FirstGMST || gmstId > TES3::GMST::LastGMST) {
			log::getLog() << "xSetGSString: Invalid GMST id." << std::endl;
			stack.pushLong(false);
			return 0.0f;
		}

		// Get the string we're going to change.
		auto dataHandler = TES3::DataHandler::get();
		char*& oldString = dataHandler->nonDynamicData->GMSTs[gmstId]->value.asString;

		// Reallocate string memory if it is growing in size.
		if (newString.length() > strlen(oldString)) {
			oldString = reinterpret_cast<char*>(se::memory::realloc(oldString, newString.length() + 1));
		}

		// Copy over new value.
		strcpy(oldString, newString.c_str());

		stack.pushLong(true);
		return 0.0f;
	}
}
