#include "LuaWeaponReadiedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	WeaponReadiedEvent::WeaponReadiedEvent(TES3::Reference* reference) :
		ObjectFilteredEvent("weaponReadied", reference),
		m_Reference(reference)
	{

	}

	sol::table WeaponReadiedEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["reference"] = m_Reference;
		eventData["weaponStack"] = m_Reference->getAttachedMobileActor()->readiedWeapon;

		return eventData;
	}
}
