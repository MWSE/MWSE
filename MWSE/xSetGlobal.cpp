#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3DataHandler.h"
#include "TES3GlobalVariable.h"


namespace mwse {
	class xSetGlobal : InstructionInterface_t {
	public:
		xSetGlobal();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xSetGlobal xSetGlobalInstance;

	xSetGlobal::xSetGlobal() : InstructionInterface_t(OpCode::xSetGlobal) {}

	float xSetGlobal::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		mwseString& variable = virtualMachine.getString(stack.popLong());
		float value = stack.popFloat();

		TES3::GlobalVariable* global = TES3::DataHandler::get()->nonDynamicData->findGlobalVariable(variable.c_str());
		if (global == nullptr) {
			log::getLog() << "xSetGlobal: No global could be found with id '" << variable << "'." << std::endl;
			stack.pushLong(false);
			return 0.0f;
		}

		global->value = value;
		stack.pushLong(true);

		return 0.0f;
	}
}
