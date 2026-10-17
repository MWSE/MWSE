#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "FileUtil.h"

namespace mwse {
	class xFileReadShort : InstructionInterface_t {
	public:
		xFileReadShort();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xFileReadShort xFileReadShortInstance;

	xFileReadShort::xFileReadShort() : InstructionInterface_t(OpCode::xFileReadShort) {}

	float xFileReadShort::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		if (stack.size() < 2) {
			log::getLog() << "xFileReadShort: Function called with too few arguments." << std::endl;
			return 0.0f;
		}

		// Get arguments from the stack.
		mwseString& fileName = virtualMachine.getString(stack.popLong());
		long count = stack.popLong();
		long valuesRead = 0;

		if (count <= 0) {
			log::getLog() << "xFileReadShort: Provided 'count' needs to be a number larger than 0." << std::endl;
			stack.pushLong(valuesRead);
			return 0.0f;
		}

		// Gather values into a temporary vector so they can be pushed onto the stack in reverse order.
		std::vector<short> values(count);
		for (long i = 0; i < count; ++i) {
			try {
				short value = FileSystem::getInstance().readValue<short>(fileName);
				values.push_back(value);
				valuesRead++;
			}
			catch (std::exception&) {
				values.push_back(0);
			}
		}

		// Copy values from the temporary vector to the stack.
		for (const auto& v : std::views::reverse(values)) {
			stack.pushShort(v);
		}
		stack.pushLong(valuesRead);

		return 0.0f;
	}
}
