#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "FileUtil.h"

namespace mwse {
	class xFileReadFloat : InstructionInterface_t {
	public:
		xFileReadFloat();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xFileReadFloat xFileReadFloatInstance;

	xFileReadFloat::xFileReadFloat() : mwse::InstructionInterface_t(OpCode::xFileReadFloat) {}

	float xFileReadFloat::execute(mwse::VMExecuteInterface& virtualMachine) {
		if (mwse::Stack::getInstance().size() < 2) {
			mwse::log::getLog() << "xFileReadFloat: Function called with too few arguments." << std::endl;
			return 0.0f;
		}

		// Get arguments from the stack.
		mwseString& fileName = virtualMachine.getString(mwse::Stack::getInstance().popLong());
		long count = mwse::Stack::getInstance().popLong();

		// Gather values into a temporary list, so they aren't in reverse order.
		long valuesRead = 0;
		std::list<float> values;
		for (long i = 0; i < count; ++i) {
			try {
				float value = mwse::FileSystem::getInstance().readFloat(fileName);
				values.push_front(value);
				valuesRead++;
			}
			catch (std::exception&) {
				values.push_front(0.0f);
			}
		}

		// Copy values from the temporary vector to the stack.
		while (!values.empty()) {
			mwse::Stack::getInstance().pushFloat(values.front());
			values.pop_front();
		}
		mwse::Stack::getInstance().pushLong(valuesRead);

		return 0.0f;
	}
}
