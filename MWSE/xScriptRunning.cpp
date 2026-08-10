#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "mwAdapter.h"
#include "VirtualMachine.h"
#include "ScriptUtil.h"
#include "TES3WorldController.h"

#include "TES3DataHandler.h"

namespace mwse {
	class xScriptRunning : InstructionInterface_t {
	public:
		xScriptRunning();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xScriptRunning xScriptRunningInstance;

	xScriptRunning::xScriptRunning() : mwse::InstructionInterface_t(OpCode::xScriptRunning) {}

	float xScriptRunning::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameters.
		mwseString& scriptName = virtualMachine.getString(mwse::Stack::getInstance().popLong());

		// Try to get the target script.
		TES3::Script* targetScript = TES3::DataHandler::get()->nonDynamicData->findScriptByName(scriptName.c_str());
		if (targetScript == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xScriptRunning: No script could be found with name '" << scriptName << "'." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Call the original function.
		TES3::Script* script = virtualMachine.getScript();
		if (script) {
			bool isRunning = TES3::WorldController::get()->isGlobalScriptRunning(targetScript);
			mwse::Stack::getInstance().pushLong(isRunning);
		}
		else {
			mwse::Stack::getInstance().pushLong(false);
		}

		return 0.0f;
	}
}
