#include "TES3GameFileLua.h"

#include "sol.hpp"
#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3GameFile.h"

namespace mwse {
	namespace lua {
		void bindTES3GameFile() {
			// Get our lua state.
			sol::state& state = LuaManager::getInstance().getState();

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.create_simple_usertype<TES3::GameFile>();
			usertypeDefinition.set("new", sol::no_constructor);

			// Basic property binding.
			usertypeDefinition.set("filename", sol::readonly_property([](TES3::GameFile& self) -> const char* { return self.filename; }));
			usertypeDefinition.set("path", sol::readonly_property([](TES3::GameFile& self) -> const char* { return self.path; }));
			usertypeDefinition.set("author", sol::readonly_property([](TES3::GameFile& self) -> const char* { return self.author; }));;
			usertypeDefinition.set("description", sol::readonly_property([](TES3::GameFile& self) -> const char* { return self.description; }));
			usertypeDefinition.set("currentHealth", sol::readonly_property([](TES3::GameFile& self) { return self.gmdt.currentHealth; }));
			usertypeDefinition.set("maxHealth", sol::readonly_property([](TES3::GameFile& self) { return self.gmdt.currentHealth; }));
			usertypeDefinition.set("gameHour", sol::readonly_property([](TES3::GameFile& self) { return self.gmdt.gameHour; }));
			usertypeDefinition.set("day", sol::readonly_property([](TES3::GameFile& self) { return self.gmdt.day; }));
			usertypeDefinition.set("month", sol::readonly_property([](TES3::GameFile& self) { return self.gmdt.month; }));
			usertypeDefinition.set("year", sol::readonly_property([](TES3::GameFile& self) { return self.gmdt.year; }));
			usertypeDefinition.set("cellName", sol::readonly_property([](TES3::GameFile& self) -> const char* { return self.gmdt.cellName; }));
			usertypeDefinition.set("daysPassed", sol::readonly_property([](TES3::GameFile& self) { return self.gmdt.daysPassed; }));
			usertypeDefinition.set("playerName", sol::readonly_property([](TES3::GameFile& self) -> const char* { return self.gmdt.playerName; }));

			// Access to other objects that need to be packaged.
			usertypeDefinition.set("masters", sol::readonly_property([](TES3::GameFile& self) {
				sol::table t = LuaManager::getInstance().getState().create_table();
				TES3::GameFile* master = self.arrayMasters;
				for (int i = 1; master; ++i, ++master) {
					t[i] = master;
				}
				return t;
			}));

			// Finish up our usertype.
			state.set_usertype("tes3gameFile", usertypeDefinition);
		}
	}
}
