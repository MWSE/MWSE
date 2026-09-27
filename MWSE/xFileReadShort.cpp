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

		// Gather values into a temporary list, so they aren't in reverse order.
		long valuesRead = 0;
		std::list<short> values;
		for (long i = 0; i < count; ++i) {
			try {
				short value = FileSystem::getInstance().readValue<short>(fileName);
				values.push_front(value);
				valuesRead++;
			}
			catch (std::exception&) {
				values.push_front(0);
			}
		}

		// Copy values from the temporary vector to the stack.
		while (!values.empty()) {
			stack.pushShort(values.front());
			values.pop_front();
		}
		stack.pushLong(valuesRead);

		return 0.0f;
	}
}
