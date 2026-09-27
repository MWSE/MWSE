#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3GlobalVariable.h"
#include "TES3DataHandler.h"


namespace mwse {
	class xGetGlobal : InstructionInterface_t {
	public:
		xGetGlobal();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xGetGlobal xGetGlobalInstance;

	xGetGlobal::xGetGlobal() : mwse::InstructionInterface_t(OpCode::xGetGlobal) {}

	float xGetGlobal::execute(mwse::VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		mwseString& variable = virtualMachine.getString(stack.popLong());

		float value = 0.0f;

		// Get global.
		const TES3::GlobalVariable* global = TES3::DataHandler::get()->nonDynamicData->findGlobalVariable(variable.c_str());
		if (global == nullptr) {
			mwse::log::getLog() << "xGetGlobal: Global '" << variable << "' could not be found." << std::endl;
			stack.pushFloat(0.0f);
			stack.pushLong(false);
			return 0.0f;
		}

		// Push value if found.
		stack.pushFloat(global->value);
		stack.pushLong(true);

		return 0.0f;
	}
}
