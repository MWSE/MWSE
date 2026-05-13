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
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xSetGSString xSetGSStringInstance;

	xSetGSString::xSetGSString() : mwse::InstructionInterface_t(OpCode::xSetGSString) {}

	float xSetGSString::execute(mwse::VMExecuteInterface& virtualMachine) {
		long gmstId = Stack::getInstance().popLong();
		mwseString& newString = virtualMachine.getString(Stack::getInstance().popLong());

		if (gmstId < 0) {
			mwse::log::getLog() << "xSetGSString: Invalid GMST id." << std::endl;
			mwse::Stack::getInstance().pushLong(false);
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

		mwse::Stack::getInstance().pushLong(true);
		return 0.0f;
	}
}
