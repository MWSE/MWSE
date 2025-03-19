#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

#include "LuaManager.h"

#include "TES3Script.h"

namespace mwse {
	class xLuaRunScript : InstructionInterface_t {
	public:
		xLuaRunScript();
		virtual float execute(VMExecuteInterface& virtualMachine);

	private:
		std::unordered_map<int, sol::table> cachedScripts;
	};

	static xLuaRunScript xLuaRunScriptInstance;

	xLuaRunScript::xLuaRunScript() : mwse::InstructionInterface_t(OpCode::xLuaRunScript) {}

	float xLuaRunScript::execute(mwse::VMExecuteInterface& virtualMachine) {
		lua::LuaManager& manager = lua::LuaManager::getInstance();
		auto stateHandle = manager.getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		// Get parameters.
		auto scriptNameKey = mwse::Stack::getInstance().popLong();
		mwseString& scriptName = virtualMachine.getString(scriptNameKey);

		// Update the LuaManager to reference our current context.
		manager.setCurrentReference(virtualMachine.getReference());
		manager.setCurrentScript(virtualMachine.getScript());

		// Does this script exist in our storage?
		sol::table cachedModule = sol::nil;
		auto cacheHit = cachedScripts.find(scriptNameKey);
		if (cacheHit != cachedScripts.end()) {
			cachedModule = cacheHit->second;
		}
		else {
			sol::protected_function_result result = state.safe_script_file(".\\Data Files\\MWSE\\mods\\" + scriptName + ".lua");
			if (result.valid()) {
				sol::object r = result;
				if (r.is<sol::table>()) {
					cachedScripts[scriptNameKey] = result;
					cachedModule = result;
				}
				else {
					manager.setCurrentReference(nullptr);
					manager.setCurrentScript(nullptr);
					return 0.0f;
				}
			}
			else {
				sol::error error = result;
				log::getLog() << "Lua error encountered for xLuaRunScript call of '" << scriptName << "' from script '" << virtualMachine.getScript()->header.name << "':" << std::endl << error.what() << std::endl;

				manager.setCurrentReference(nullptr);
				manager.setCurrentScript(nullptr);

				// Clear the stack, since we can't trust what the script did or did not do.
				mwse::Stack::getInstance().clear();
				return 0.0f;
			}
		}

		// Run the script.
		if (cachedModule != sol::nil) {
			sol::optional<sol::protected_function> execute = cachedModule["execute"];
			if (execute) {
				sol::protected_function_result result = execute.value().call();
				if (!result.valid()) {
					sol::error error = result;
					log::getLog() << "Lua error encountered for xLuaRunScript call of '" << scriptName << "' from script '" << virtualMachine.getScript()->header.name << "':" << std::endl << error.what() << std::endl;

					// Clear the stack, since we can't trust what the script did or did not do.
					mwse::Stack::getInstance().clear();
				}
			}
			else {
				log::getLog() << "No execute function found for xLuaRunScript call of '" << scriptName << "' from script '" << virtualMachine.getScript()->header.name << "'." << std::endl;

				// Clear the stack, since we can't trust what the script did or did not do.
				mwse::Stack::getInstance().clear();
			}
		}

		manager.setCurrentReference(nullptr);
		manager.setCurrentScript(nullptr);

		return 0.0f;
	}
}
