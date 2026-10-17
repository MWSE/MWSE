#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "FileUtil.h"

namespace mwse {
	class xFileReadLong : InstructionInterface_t {
	public:
		xFileReadLong();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xFileReadLong xFileReadLongInstance;

	xFileReadLong::xFileReadLong() : InstructionInterface_t(OpCode::xFileReadLong) {}

	float xFileReadLong::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		if (stack.size() < 2) {
			log::getLog() << "xFileReadLong: Function called with too few arguments." << std::endl;
			return 0.0f;
		}

		// Get arguments from the stack.
		mwseString& fileName = virtualMachine.getString(stack.popLong());
		long count = stack.popLong();
		long valuesRead = 0;

		if (count <= 0) {
			log::getLog() << "xFileReadLong: Provided 'count' needs to be a number larger than 0." << std::endl;
			stack.pushLong(valuesRead);
			return 0.0f;
		}

		// Gather values into a temporary vector so they can be pushed onto the stack in reverse order.
		std::vector<long> values(count);
		for (long i = 0; i < count; ++i) {
			try {
				long value = FileSystem::getInstance().readValue<long>(fileName);
				values.push_back(value);
				valuesRead++;
			}
			catch (std::exception&) {
				values.push_back(0);
			}
		}

		// Copy values from the temporary vector to the stack.
		for (const auto& v : std::views::reverse(values)) {
			stack.pushLong(v);
		}
		stack.pushLong(valuesRead);

		return 0.0f;
	}
}
