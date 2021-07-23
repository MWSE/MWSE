/************************************************************************
	
	xLuaRunScript.cpp - Copyright (c) 2008 The MWSE Project
	https://github.com/MWSE/MWSE/

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

**************************************************************************/

#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

#include "LuaManager.h"

#include "TES3Script.h"

using namespace mwse;

namespace mwse
{
	class xLuaRunScript : mwse::InstructionInterface_t
	{
	public:
		xLuaRunScript();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);

	private:
		std::unordered_map<int, sol::table> cachedScripts;
	};

	static xLuaRunScript xLuaRunScriptInstance;

	xLuaRunScript::xLuaRunScript() : mwse::InstructionInterface_t(OpCode::xLuaRunScript) {}

	void xLuaRunScript::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xLuaRunScript::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		lua::LuaManager& manager = lua::LuaManager::getInstance();
		auto stateHandle = manager.getThreadSafeStateHandle();
		sol::state& state = stateHandle.state;

		// Get parameters.
		long scriptNameKey = mwse::Stack::getInstance().popLong();
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
				log::getLog() << "Lua error encountered for xLuaRunScript call of '" << scriptName << "' from script '" << virtualMachine.getScript()->name << "':" << std::endl << error.what() << std::endl;

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
					log::getLog() << "Lua error encountered for xLuaRunScript call of '" << scriptName << "' from script '" << virtualMachine.getScript()->name << "':" << std::endl << error.what() << std::endl;

					// Clear the stack, since we can't trust what the script did or did not do.
					mwse::Stack::getInstance().clear();
				}
			}
			else {
				log::getLog() << "No execute function found for xLuaRunScript call of '" << scriptName << "' from script '" << virtualMachine.getScript()->name << "'." << std::endl;

				// Clear the stack, since we can't trust what the script did or did not do.
				mwse::Stack::getInstance().clear();
			}
		}

		manager.setCurrentReference(nullptr);
		manager.setCurrentScript(nullptr);

		return 0.0f;
	}
}