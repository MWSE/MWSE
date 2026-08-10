#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "mwAdapter.h"
#include "VirtualMachine.h"
#include "ScriptUtil.h"

#include "TES3DataHandler.h"
#include "TES3WorldController.h"

namespace mwse {
	class xStartScript : InstructionInterface_t {
	public:
		xStartScript();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xStartScript xStartScriptInstance;

	xStartScript::xStartScript() : mwse::InstructionInterface_t(OpCode::xStartScript) {}

	float xStartScript::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameters.
		mwseString& scriptName = virtualMachine.getString(mwse::Stack::getInstance().popLong());

		// Try to get the target script.
		TES3::Script* targetScript = TES3::DataHandler::get()->nonDynamicData->findScriptByName(scriptName.c_str());
		if (targetScript == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xStartScript: No script could be found with name '" << scriptName << "'." << std::endl;
			}
			return 0.0f;
		}

		// Call the original function.
		TES3::Reference* reference = virtualMachine.getReference();
		TES3::Script* script = virtualMachine.getScript();
		if (targetScript) {
			TES3::WorldController::get()->startGlobalScript(targetScript, reference);
		}

		return 0.0f;
	}
}
