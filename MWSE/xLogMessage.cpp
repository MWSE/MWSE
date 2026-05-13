#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "Log.h"
#include "StringUtil.h"

namespace mwse {
	// Provides an interface to mwse::log, allowing scripts to write to the default MWSE.log file.
	//
	// MWScript Usage:
	//	<none> xLogMessage sFormat ...
	//
	// MWEdit definition:
	//	Name = XLogMessage
	//	Options = MWSE | AllowGlobal | ShortVar | AllowVar
	//	Return = None
	//	Param1 = Long | String
	//	Param2 = Short | Long | Float | String | Many | Optional
	//	Opcode = 0x3c09
	class xLogMessage : InstructionInterface_t {
	public:
		xLogMessage();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xLogMessage xLogMessageInstance;

	xLogMessage::xLogMessage() : mwse::InstructionInterface_t(OpCode::xLogMessage) {}

	float xLogMessage::execute(mwse::VMExecuteInterface& virtualMachine) {
		mwseString& format = virtualMachine.getString(Stack::getInstance().popLong());

		bool suppressNull = false;
		std::string badCodes;
		std::string result = se::string::interpolate(format, virtualMachine, &suppressNull, &badCodes);

		mwse::log::getLog() << result << std::endl;
		if (!badCodes.empty()) {
			mwse::log::getLog() << "xLogMessage: bad format \"" << badCodes << "\" in \"" << format << "\" generating \"" << result << "\"" << badCodes << std::endl;
		}

		return 0.0f;
	}
}
