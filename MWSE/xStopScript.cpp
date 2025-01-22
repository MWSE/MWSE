#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "mwAdapter.h"
#include "VirtualMachine.h"
#include "ScriptUtil.h"

#include "TES3DataHandler.h"
#include "TES3Script.h"
#include "TES3WorldController.h"

namespace mwse {
	class xStopScript : InstructionInterface_t {
	public:
		xStopScript();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xStopScript xStopScriptInstance;

	xStopScript::xStopScript() : mwse::InstructionInterface_t(OpCode::xStopScript) {}

	float xStopScript::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameter: script name. We allow a value of 0 to target the current script.
		long scriptNameId = mwse::Stack::getInstance().popLong();
		const char* scriptName = NULL;
		if (scriptNameId == 0) {
			scriptName = virtualMachine.getScript()->header.name;
		}
		else {
			scriptName = virtualMachine.getString(scriptNameId).c_str();
		}

		// Try to get the target script.
		TES3::Script* targetScript = TES3::DataHandler::get()->nonDynamicData->findScriptByName(scriptName);
		if (targetScript == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xStopScript: No script could be found with name '" << scriptName << "'." << std::endl;
			}
			return 0.0f;
		}

		// Call the original function.
		TES3::Script* script = virtualMachine.getScript();
		if (script) {
			TES3::WorldController::get()->stopGlobalScript(targetScript);
		}

		return 0.0f;
	}
}
