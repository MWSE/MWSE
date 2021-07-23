#include "TES3GameFileLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3GameFile.h"

namespace mwse {
	namespace lua {
		void bindTES3GameFile() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<TES3::GameFile>("tes3gameFile");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["filename"] = sol::readonly_property(&TES3::GameFile::getFilename);
			usertypeDefinition["path"] = sol::readonly_property(&TES3::GameFile::getPath);
			usertypeDefinition["author"] = sol::readonly_property(&TES3::GameFile::getAuthor);
			usertypeDefinition["description"] = sol::readonly_property(&TES3::GameFile::getDescription);
			usertypeDefinition["currentHealth"] = sol::readonly_property(&TES3::GameFile::getCurrentHealth);
			usertypeDefinition["maxHealth"] = sol::readonly_property(&TES3::GameFile::getMaxHealth);
			usertypeDefinition["gameHour"] = sol::readonly_property(&TES3::GameFile::getGameHour);
			usertypeDefinition["highestFormID"] = sol::readonly_property(&TES3::GameFile::highestFormID);
			usertypeDefinition["day"] = sol::readonly_property(&TES3::GameFile::getDay);
			usertypeDefinition["month"] = sol::readonly_property(&TES3::GameFile::getMonth);
			usertypeDefinition["year"] = sol::readonly_property(&TES3::GameFile::getYear);
			usertypeDefinition["cellName"] = sol::readonly_property(&TES3::GameFile::getCellName);
			usertypeDefinition["daysPassed"] = sol::readonly_property(&TES3::GameFile::getDaysPassed);
			usertypeDefinition["playerName"] = sol::readonly_property(&TES3::GameFile::getPlayerName);
			usertypeDefinition["fileSize"] = sol::readonly_property(&TES3::GameFile::getFileSize);
			usertypeDefinition["modifiedTime"] = sol::readonly_property(&TES3::GameFile::getModifiedTime);

			// Access to other objects that need to be packaged.
			usertypeDefinition["masters"] = sol::readonly_property(&TES3::GameFile::getMasters_lua);

			// Function bindings.
			usertypeDefinition["deleteFile"] = &TES3::GameFile::deleteFile;
		}
	}
}
