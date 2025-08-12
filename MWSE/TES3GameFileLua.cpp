#include "TES3GameFileLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3GameFile.h"

namespace mwse::lua {
	void bindTES3GameFile() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::GameFile>("tes3gameFile");
		usertypeDefinition["new"] = sol::no_constructor;

		// Basic property binding.
		usertypeDefinition["author"] = sol::readonly_property(&TES3::GameFile::getAuthor);
		usertypeDefinition["cellName"] = sol::readonly_property(&TES3::GameFile::getCellName);
		usertypeDefinition["currentHealth"] = sol::readonly_property(&TES3::GameFile::getCurrentHealth);
		usertypeDefinition["day"] = sol::readonly_property(&TES3::GameFile::getDay);
		usertypeDefinition["daysPassed"] = sol::readonly_property(&TES3::GameFile::getDaysPassed);
		usertypeDefinition["description"] = sol::readonly_property(&TES3::GameFile::getDescription);
		usertypeDefinition["filename"] = sol::readonly_property(&TES3::GameFile::getFilename);
		usertypeDefinition["fileSize"] = sol::readonly_property(&TES3::GameFile::getFileSize);
		usertypeDefinition["gameHour"] = sol::readonly_property(&TES3::GameFile::getGameHour);
		usertypeDefinition["highestFormID"] = sol::readonly_property(&TES3::GameFile::highestFormID);
		usertypeDefinition["masterFileSizes"] = sol::readonly_property(&TES3::GameFile::masterFileSizes);
		usertypeDefinition["masterNames"] = sol::readonly_property(&TES3::GameFile::masterNames);
		usertypeDefinition["masters"] = sol::readonly_property(&TES3::GameFile::getMasters);
		usertypeDefinition["maxHealth"] = sol::readonly_property(&TES3::GameFile::getMaxHealth);
		usertypeDefinition["modifiedTime"] = sol::readonly_property(&TES3::GameFile::getModifiedTime);
		usertypeDefinition["month"] = sol::readonly_property(&TES3::GameFile::getMonth);
		usertypeDefinition["path"] = sol::readonly_property(&TES3::GameFile::getPath);
		usertypeDefinition["playerName"] = sol::readonly_property(&TES3::GameFile::getPlayerName);
		usertypeDefinition["toLoad"] = sol::property(&TES3::GameFile::getToLoad, &TES3::GameFile::setToLoad);
		usertypeDefinition["year"] = sol::readonly_property(&TES3::GameFile::getYear);

		// Function bindings.
		usertypeDefinition["deleteFile"] = &TES3::GameFile::deleteFile;
	}
}
