#include "TES3CombatSessionLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3CombatSession.h"
#include "TES3Inventory.h"
#include "TES3MobileActor.h"
#include "TES3Spell.h"

namespace mwse::lua {
	void bindTES3CombatSession() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::CombatSession>("tes3combatSession");
		usertypeDefinition["new"] = sol::no_constructor;

		// Basic property binding.
		usertypeDefinition["alchemyPriority"] = &TES3::CombatSession::alchemyPriority;
		usertypeDefinition["castableSpells"] = sol::readonly_property(&TES3::CombatSession::spells);
		usertypeDefinition["distance"] = &TES3::CombatSession::combatDistance;
		usertypeDefinition["mobile"] = sol::readonly_property(&TES3::CombatSession::parentActor);
		usertypeDefinition["lastUseTimestamp"] = &TES3::CombatSession::mcpAlchemyLastUseTimestamp;
		usertypeDefinition["potionUseFlag"] = &TES3::CombatSession::potionUseFlag;
		usertypeDefinition["selectedAction"] = &TES3::CombatSession::nextAction;
		usertypeDefinition["selectedItem"] = &TES3::CombatSession::selectedItem;
		usertypeDefinition["selectedShield"] = sol::readonly_property(&TES3::CombatSession::selectedShield);
		usertypeDefinition["selectedSpell"] = &TES3::CombatSession::selectedSpell;
		usertypeDefinition["selectedWeapon"] = sol::readonly_property(&TES3::CombatSession::selectedWeapon);
		usertypeDefinition["spellPriority"] = &TES3::CombatSession::spellPriority;

		// Basic function binding.
		usertypeDefinition["selectAlchemyWithEffect"] = &TES3::CombatSession::chooseAlchemyWithEffect;
		usertypeDefinition["changeEquipment"] = &TES3::CombatSession::changeEquipment;

		// Custom lua data access.
		usertypeDefinition["data"] = sol::property(&TES3::CombatSession::getLuaData, &TES3::CombatSession::setLuaData);

		// Deprecated fields.
		usertypeDefinition["selectedAlchemy"] = &TES3::CombatSession::selectedItem;
	}
}
