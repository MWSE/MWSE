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
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xSetGlobal xSetGlobalInstance;

	xSetGlobal::xSetGlobal() : mwse::InstructionInterface_t(OpCode::xSetGlobal) {}

	float xSetGlobal::execute(mwse::VMExecuteInterface& virtualMachine) {
		mwseString& variable = virtualMachine.getString(Stack::getInstance().popLong());
		float value = Stack::getInstance().popFloat();

		TES3::GlobalVariable* global = TES3::DataHandler::get()->nonDynamicData->findGlobalVariable(variable.c_str());
		if (global == NULL) {
			mwse::log::getLog() << "xSetGlobal: No global could be found with id '" << variable << "'." << std::endl;
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		global->value = value;
		mwse::Stack::getInstance().pushLong(true);

		return 0.0f;
	}
}
