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
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xGetGlobal xGetGlobalInstance;

	xGetGlobal::xGetGlobal() : mwse::InstructionInterface_t(OpCode::xGetGlobal) {}

	float xGetGlobal::execute(mwse::VMExecuteInterface& virtualMachine) {
		mwseString& variable = virtualMachine.getString(Stack::getInstance().popLong());

		float value = 0.0f;

		// Get global.
		const TES3::GlobalVariable* global = TES3::DataHandler::get()->nonDynamicData->findGlobalVariable(variable.c_str());
		if (global == NULL) {
			mwse::log::getLog() << "xGetGlobal: Global '" << variable << "' could not be found." << std::endl;
			mwse::Stack::getInstance().pushFloat(0.0f);
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Push value if found.
		mwse::Stack::getInstance().pushFloat(global->value);
		mwse::Stack::getInstance().pushLong(true);

		return 0.0f;
	}
}
