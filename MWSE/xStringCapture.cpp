#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "Log.h"
#include "StringUtil.h"

namespace mwse {
	class xStringCapture : InstructionInterface_t {
	public:
		xStringCapture();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xStringCapture xStringCaptureInstance;

	xStringCapture::xStringCapture() : InstructionInterface_t(OpCode::xStringCapture) {}

	float xStringCapture::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get parameters from stack.
		mwseString& string = virtualMachine.getString(stack.popLong());
		mwseString& pattern = virtualMachine.getString(stack.popLong());
		long desiredMatches = stack.popLong();

		// Go and try to get all of our matches, to a limit of the count given to us as our 3rd parameter.
		long matchesReturned = 0;
		try {
			boost::regex regex_pattern(pattern);
			boost::smatch regex_matches;
			if (boost::regex_search(string, regex_matches, regex_pattern)) {
				// The capture groups begin at index 1, so start there.
				for (size_t i = 1; i < regex_matches.size(); ++i) {
					// Bail out if we're past our desired number of matches.
					if (matchesReturned >= desiredMatches) {
						break;
					}

					// Bring the match into string storage and push it back to mwscript.
					mwseString& match = se::string::store::getOrCreate(regex_matches[i].str());
					stack.pushString(match);
					matchesReturned++;
				}
			}
		}
		catch (boost::regex_error& e) {
			log::getLog() << "xStringCapture: A regex exception has occurred. " << e.what() << std::endl;
		}

		// If we didn't get enough matches, pad it out with zeros.
		while (matchesReturned < desiredMatches) {
			stack.pushLong(0);
			matchesReturned++;
		}

		return 0.0f;
	}
}
