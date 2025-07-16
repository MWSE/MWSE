#include "TES3MobManagerLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobManager.h"
#include "TES3MobileProjectile.h"
#include "TES3Reference.h"

namespace mwse::lua {
	void bindTES3MobManager() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Binding for TES3::ProcessManager.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::ProcessManager>("tes3processManager");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["aiDistance"] = &TES3::ProcessManager::aiDistance;

			// Basic function binding.
			usertypeDefinition["checkNearbyEnemiesAllowRest"] = &TES3::ProcessManager::checkNearbyEnemiesAllowRest;
			usertypeDefinition["detectPresence"] = &TES3::ProcessManager::detectPresence;
			usertypeDefinition["detectSneak"] = &TES3::ProcessManager::detectSneak;
			usertypeDefinition["checkAlarmRadius"] = &TES3::ProcessManager::checkAlarmRadius;
			usertypeDefinition["checkPlayerDistance"] = &TES3::ProcessManager::checkPlayerDistance;

			// Functions exposed as properties.
			usertypeDefinition["allMobileActors"] = sol::readonly_property(&TES3::ProcessManager::getAllMobileActors);
			usertypeDefinition["allPlanners"] = sol::readonly_property(&TES3::ProcessManager::getAllPlanners);
			usertypeDefinition["aiDistanceScale"] = sol::property(&TES3::ProcessManager::getAIDistanceScale, &TES3::ProcessManager::setAIDistanceScale);
		}

		// Binding for TES3::ProjectileManager.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::ProjectileManager>("tes3projectileManager");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["projectiles"] = sol::readonly_property(&TES3::ProjectileManager::activeProjectiles);
		}

		// Binding for TES3::MobManager.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::MobManager>("tes3mobManager");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["gravity"] = sol::property(&TES3::MobManager::getGravity, &TES3::MobManager::setGravity);
			usertypeDefinition["mobCollisionGroup"] = sol::readonly_property(&TES3::MobManager::mobCollisionGroup);
			usertypeDefinition["movingProps"] = sol::readonly_property(&TES3::MobManager::listMovingProps);
			usertypeDefinition["processManager"] = sol::readonly_property(&TES3::MobManager::processManager);
			usertypeDefinition["projectileManager"] = sol::readonly_property(&TES3::MobManager::projectileManager);
			usertypeDefinition["terminalVelocity"] = sol::property(&TES3::MobManager::getTerminalVelocity, &TES3::MobManager::setTerminalVelocity);

			// Legacy bindings.
			usertypeDefinition["mobController_0x24"] = sol::readonly_property(&TES3::MobManager::processManager);
			usertypeDefinition["projectileController"] = sol::readonly_property(&TES3::MobManager::projectileManager);
		}
	}
}
