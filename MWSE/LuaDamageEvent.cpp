#include "LuaDamageEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MagicEffectInstance.h"
#include "TES3MobileActor.h"
#include "TES3MobileProjectile.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	DamageEvent::DamageEvent(TES3::MobileActor* mobileActor, float damage) :
		ObjectFilteredEvent("damage", mobileActor->reference),
		m_MobileActor(mobileActor),
		m_Damage(damage)
	{

	}

	sol::table DamageEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		if (m_MobileActor) {
			eventData["mobile"] = m_MobileActor;
			eventData["reference"] = m_MobileActor->reference;
		}

		if (m_Attacker) {
			eventData["attacker"] = m_Attacker;
			eventData["attackerReference"] = m_Attacker->reference;
		}

		if (m_Projectile) {
			eventData["projectile"] = m_Projectile;
		}

		auto magicSourceInstance = m_MagicSourceInstance;
		if (m_ActiveMagicEffect) {
			eventData["activeMagicEffect"] = TES3::ActiveMagicEffectLua(*m_ActiveMagicEffect, m_MobileActor);
			magicSourceInstance = m_ActiveMagicEffect->getInstance();
		}

		if (magicSourceInstance) {
			eventData["magicSourceInstance"] = magicSourceInstance;
			eventData["magicEffectIndex"] = m_MagicEffectIndex;

			// Get the specific effect on the source.
			TES3::Effect* effects = magicSourceInstance->sourceCombo.getSourceEffects();
			if (effects) {
				eventData["magicEffect"] = effects[m_MagicEffectIndex];
			}

			// Get the attacker as the caster of the spell.
			if (!m_Attacker) {
				eventData["attackerReference"] = magicSourceInstance->caster;
				if (magicSourceInstance->caster) {
					eventData["attacker"] = magicSourceInstance->caster->getAttachedMobileActor();
				}
			}
		}

		if (m_MagicEffectInstance) {
			eventData["magicEffectInstance"] = m_MagicEffectInstance;
		}

		if (m_Source) {
			eventData["source"] = m_Source;
		}

		eventData["damage"] = m_Damage;

		return eventData;
	}
	TES3::MobileActor* DamageEvent::m_Attacker = nullptr;
	TES3::MobileProjectile* DamageEvent::m_Projectile = nullptr;
	TES3::MagicSourceInstance* DamageEvent::m_MagicSourceInstance = nullptr;
	TES3::MagicEffectInstance* DamageEvent::m_MagicEffectInstance = nullptr;
	int DamageEvent::m_MagicEffectIndex = -1;
	TES3::ActiveMagicEffect* DamageEvent::m_ActiveMagicEffect = nullptr;
	const char* DamageEvent::m_Source = nullptr;
}
