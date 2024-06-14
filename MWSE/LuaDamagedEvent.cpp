#include "LuaDamagedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MagicEffectInstance.h"
#include "TES3MobileActor.h"
#include "TES3MobileProjectile.h"
#include "TES3Reference.h"

#include "LuaDamageEvent.h"

namespace mwse::lua::event {
	DamagedEvent::DamagedEvent(TES3::MobileActor* mobileActor, float damage, bool killingBlow) :
		ObjectFilteredEvent("damaged", mobileActor->reference),
		m_MobileActor(mobileActor),
		m_Damage(damage),
		m_KillingBlow(killingBlow)
	{

	}

	sol::table DamagedEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		if (m_MobileActor) {
			eventData["mobile"] = m_MobileActor;
			eventData["reference"] = m_MobileActor->reference;
		}

		if (DamageEvent::m_Attacker) {
			eventData["attacker"] = DamageEvent::m_Attacker;
			eventData["attackerReference"] = DamageEvent::m_Attacker->reference;
		}

		if (DamageEvent::m_Projectile) {
			eventData["projectile"] = DamageEvent::m_Projectile;
		}

		auto magicSourceInstance = DamageEvent::m_MagicSourceInstance;
		if (DamageEvent::m_ActiveMagicEffect) {
			eventData["activeMagicEffect"] = DamageEvent::m_ActiveMagicEffect;
			magicSourceInstance = DamageEvent::m_ActiveMagicEffect->getInstance();
		}

		if (magicSourceInstance) {
			eventData["magicSourceInstance"] = magicSourceInstance;
			eventData["magicEffectIndex"] = DamageEvent::m_MagicEffectIndex;

			// Get the specific effect on the source.
			TES3::Effect* effects = magicSourceInstance->sourceCombo.getSourceEffects();
			if (effects) {
				eventData["magicEffect"] = effects[DamageEvent::m_MagicEffectIndex];
			}

			// Get the attacker as the caster of the spell.
			if (!DamageEvent::m_Attacker) {
				eventData["attackerReference"] = magicSourceInstance->caster;
				if (magicSourceInstance->caster) {
					eventData["attacker"] = magicSourceInstance->caster->getAttachedMobileActor();
				}
			}
		}

		if (DamageEvent::m_MagicEffectInstance) {
			eventData["magicEffectInstance"] = DamageEvent::m_MagicEffectInstance;
		}

		if (DamageEvent::m_Source) {
			eventData["source"] = DamageEvent::m_Source;
		}

		eventData["damage"] = m_Damage;
		eventData["killingBlow"] = m_KillingBlow;

		return eventData;
	}
}
