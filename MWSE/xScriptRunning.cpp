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
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xScriptRunning xScriptRunningInstance;

	xScriptRunning::xScriptRunning() : mwse::InstructionInterface_t(OpCode::xScriptRunning) {}

	float xScriptRunning::execute(mwse::VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get parameters.
		mwseString& scriptName = virtualMachine.getString(stack.popLong());

		// Try to get the target script.
		TES3::Script* targetScript = TES3::DataHandler::get()->nonDynamicData->findScriptByName(scriptName.c_str());
		if (targetScript == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xScriptRunning: No script could be found with name '" << scriptName << "'." << std::endl;
			}
			stack.pushLong(false);
			return 0.0f;
		}

		// Call the original function.
		TES3::Script* script = virtualMachine.getScript();
		if (script) {
			bool isRunning = TES3::WorldController::get()->isGlobalScriptRunning(targetScript);
			stack.pushLong(isRunning);
		}
		else {
			stack.pushLong(false);
		}

		return 0.0f;
	}
}
