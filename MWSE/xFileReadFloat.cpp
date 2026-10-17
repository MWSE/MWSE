#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "FileUtil.h"

namespace mwse {
	class xFileReadFloat : InstructionInterface_t {
	public:
		xFileReadFloat();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xFileReadFloat xFileReadFloatInstance;

	xFileReadFloat::xFileReadFloat() : InstructionInterface_t(OpCode::xFileReadFloat) {}

	float xFileReadFloat::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		if (stack.size() < 2) {
			log::getLog() << "xFileReadFloat: Function called with too few arguments." << std::endl;
			return 0.0f;
		}

		// Get arguments from the stack.
		mwseString& fileName = virtualMachine.getString(stack.popLong());
		long count = stack.popLong();
		long valuesRead = 0;

		if (count <= 0) {
			log::getLog() << "xFileReadFloat: Provided 'count' needs to be a number larger than 0." << std::endl;
			stack.pushLong(valuesRead);
			return 0.0f;
		}

		// Gather values into a temporary vector so they can be pushed onto the stack in reverse order.
		std::vector<float> values(count);
		for (long i = 0; i < count; ++i) {
			try {
				float value = FileSystem::getInstance().readValue<float>(fileName);
				values.push_back(value);
				valuesRead++;
			}
			catch (std::exception&) {
				values.push_back(0.0f);
			}
		}

		// Copy values from the temporary vector to the stack.
		for (const auto& v : std::views::reverse(values)) {
			stack.pushFloat(v);
		}
		stack.pushLong(valuesRead);

		return 0.0f;
	}
}
