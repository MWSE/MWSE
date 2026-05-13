#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "ScriptUtil.h"
#include "StringUtil.h"
#include "TES3Script.h"


namespace mwse {
	class xMessageFix : InstructionInterface_t {
	public:
		xMessageFix();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xMessageFix xMessageFixInstance;

	xMessageFix::xMessageFix() : mwse::InstructionInterface_t(OpCode::xMessageFix) {}

	// This function changes the script at runtime.
	float xMessageFix::execute(mwse::VMExecuteInterface& virtualMachine) {
		unsigned short mboxhdr[2] = {};

		// We want to modify the real script SCDT info. We'll keep track of where we're reading/writing with a read/write pointer,
		// starting at the current instruction pointer in the machine code.
		auto scriptRWP = reinterpret_cast<unsigned char*>(virtualMachine.getScript()->machineCode);
		scriptRWP += +mwse::mwscript::getInstructionPointer();

		// Must be followed by MessageBox opcode and length.
		mboxhdr[0] = reinterpret_cast<unsigned short*>(scriptRWP)[0];
		mboxhdr[1] = reinterpret_cast<unsigned short*>(scriptRWP)[1];
		scriptRWP += sizeof(mboxhdr);

		// This function is invalid if the next call isn't to MessageBox.
		if (mboxhdr[0] != OpCode::MessageBox) {
			mwse::log::getLog() << "xMessageFix: This function must immediately be followed by a MessageBox call." << std::endl;
			return 0.0f;
		}

		// Get the parameter matching the main message string.
		mwseString& format = virtualMachine.getString(mwse::Stack::getInstance().popLong());

		// The first string is the message text. Following parameters might be based on the format there.
		// We can skip the substitution if the string is empty or nonexistant.
		if (format != 0 && !format.empty()) {
			bool suppressNull = false;
			std::string badCodes;
			auto newString = se::string::interpolate(format, virtualMachine, &suppressNull, &badCodes);
			if (badCodes != "") {
				mwse::log::getLog() << "xMessageFix: Bad format \"" << badCodes << "\" in \"" << format << "\" generating \"" << newString << "\"." << std::endl;
			}

			// Make sure that we can fit the new string into the buffer. Copy what we can into the string here.
			size_t len = 0;
			if (mboxhdr[1] < newString.length()) {
				len = mboxhdr[1];
			}
			else {
				len = newString.length() + 1;
			}
			if (suppressNull) {
				len--;
			}
			strncpy(reinterpret_cast<char*>(scriptRWP), newString.c_str(), len);
		}
		scriptRWP += mboxhdr[1];

		// Find the null after the orignal message string.
		// The "%.0f" style formatting is not allowed for the MessageBox, xMessageFix handles it here.
		if (*scriptRWP != 0) {
			mwse::log::getLog() << "xMessageFix: The \"%.0f\" style formatting is not allowed for the MessageBox. Aborting." << std::endl;
			return false;
		}
		scriptRWP += 1;

		// Next is the number of buttons to replace.
		auto buttonCount = *scriptRWP;
		scriptRWP += 1;

		// For each button, replace it with string from the stack.
		for (auto buttonIndex = 0; buttonIndex < buttonCount; ++buttonIndex) {
			// Find the amount of space available.
			auto buttonLength = *scriptRWP;
			scriptRWP += 1;

			// Get the replacement string.
			mwseString& newButtonText = virtualMachine.getString(mwse::Stack::getInstance().popLong());

			// We can skip the substitution if the string is empty or nonexistant.
			if (!newButtonText.empty()) {
				auto suppressNull = false;
				std::string badCodes;
				std::string newString = se::string::interpolate(newButtonText, virtualMachine, &suppressNull, &badCodes);
				if (badCodes != "") {
					mwse::log::getLog() << "xMessageFix: Bad format \"" << badCodes << "\" in \"" << format << "\" generating \"" << newString << "\"." << std::endl;
				}

				// Make sure that we can fit the new string into the buffer. Copy what we can into the string here.
				size_t len = 0;
				if (buttonLength < newString.length()) {
					len = buttonLength - 1;
				}
				else {
					len = newString.length() + 1;
				}
				if (suppressNull) {
					len--;
				}
				strncpy(reinterpret_cast<char*>(scriptRWP), newString.c_str(), len);
			}

			// Advance our script pointer by the string length.
			scriptRWP += buttonLength;
		}

		return 0.0f;
	}
}
