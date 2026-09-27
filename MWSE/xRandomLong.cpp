#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "RngUtil.h"

namespace mwse {
	class xRandomLong : InstructionInterface_t {
	public:
		xRandomLong();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xRandomLong xRandomLongInstance;

	xRandomLong::xRandomLong() : mwse::InstructionInterface_t(OpCode::xRandomLong) {}

	float xRandomLong::execute(mwse::VMExecuteInterface& virtualMachine) {
		long min = mwse::Stack::getInstance().popLong();
		long max = mwse::Stack::getInstance().popLong();

		mwse::Stack::getInstance().pushLong(mwse::rng::getRandomLong(min, max));

		return 0.0f;
	}
}
