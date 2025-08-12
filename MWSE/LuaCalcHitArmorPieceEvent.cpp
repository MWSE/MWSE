#include "LuaCalcHitArmorPieceEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "LuaDamageEvent.h"

#include "TES3MobileNPC.h"
#include "TES3MobileProjectile.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	CalcHitArmorPiece::CalcHitArmorPiece(TES3::MobileNPC* actor, TES3::ArmorSlot::ArmorSlot slot, TES3::ArmorSlot::ArmorSlot fallback) :
		ObjectFilteredEvent("calcArmorPieceHit", actor->reference->baseObject),
		m_Mobile(actor),
		m_Slot(slot),
		m_Fallback(fallback)
	{

	}

	sol::table CalcHitArmorPiece::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["mobile"] = m_Mobile;
		eventData["reference"] = m_Mobile->reference;

		if (m_Slot != TES3::ArmorSlot::Invalid) {
			eventData["slot"] = m_Slot;
		}

		if (m_Fallback != TES3::ArmorSlot::Invalid) {
			eventData["fallback"] = m_Fallback;
		}

		auto attacker = mwse::lua::event::DamageEvent::m_Attacker;
		if (attacker) {
			eventData["attacker"] = attacker->reference;
			eventData["attackerMobile"] = attacker;
		}

		auto projectile = mwse::lua::event::DamageEvent::m_Projectile;
		if (projectile) {
			eventData["projectile"] = projectile;
		}

		return eventData;
	}
}
