#include "LuaMobileProjectileActorCollisionEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3MobileProjectile.h"
#include "TES3Reference.h"
#include "TES3Weapon.h"

namespace mwse {
	namespace lua {
		namespace event {
			MobileProjectileActorCollisionEvent::MobileProjectileActorCollisionEvent(TES3::MobileProjectile* projectile, TES3::Reference* targetReference) :
				ObjectFilteredEvent("projectileHitActor", projectile->firingActor->reference),
				m_Projectile(projectile),
				m_TargetReference(targetReference)
			{

			}

			sol::table MobileProjectileActorCollisionEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["mobile"] = makeLuaObject(m_Projectile);
				eventData["target"] = makeLuaObject(m_TargetReference);

				// Give a shorthand to the firing reference.
				if (m_Projectile->firingActor && m_Projectile->firingActor->reference) {
					eventData["firingReference"] = makeLuaObject(m_Projectile->firingActor->reference);
				}

				// Also give a shorthand to the firing weapon.
				if (m_Projectile->firingWeapon) {
					eventData["firingWeapon"] = makeLuaObject(m_Projectile->firingWeapon);
				}


				return eventData;
			}
		}
	}
}
