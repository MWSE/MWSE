#include "TES3MobControllerLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobController.h"
#include "TES3MobileProjectile.h"

namespace mwse {
	namespace lua {
		void bindTES3MobController() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Binding for TES3::ProcessManager.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::ProcessManager>("tes3processManager");
				usertypeDefinition["new"] = sol::no_constructor;

				// Basic property binding.
				usertypeDefinition["aiDistance"] = &TES3::ProcessManager::aiDistance;

				// Basic function binding.
				usertypeDefinition["canRest"] = &TES3::ProcessManager::canRest;
				usertypeDefinition["detectPresence"] = &TES3::ProcessManager::detectPresence;
				usertypeDefinition["detectSneak"] = &TES3::ProcessManager::detectSneak;
				usertypeDefinition["checkAlarmRadius"] = &TES3::ProcessManager::checkAlarmRadius;
				usertypeDefinition["checkPlayerDistance"] = &TES3::ProcessManager::checkPlayerDistance;

				// Functions exposed as properties.
				usertypeDefinition["aiDistanceScale"] = sol::property(&TES3::ProcessManager::getAIDistanceScale, &TES3::ProcessManager::setAIDistanceScale);
			}

			// Binding for TES3::ProjectileController.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::ProjectileController>("tes3projectileController");
				usertypeDefinition["new"] = sol::no_constructor;

				// Basic property binding.
				usertypeDefinition["projectiles"] = sol::readonly_property(&TES3::ProjectileController::activeProjectiles);
			}

			// Binding for TES3::MobController.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::MobController>("tes3mobController");
				usertypeDefinition["new"] = sol::no_constructor;

				// Basic property binding.
				usertypeDefinition["processManager"] = sol::readonly_property(&TES3::MobController::processManager);
				usertypeDefinition["projectileController"] = sol::readonly_property(&TES3::MobController::projectileController);

				// Legacy bindings.
				usertypeDefinition["mobController_0x24"] = sol::readonly_property(&TES3::MobController::processManager);
			}
		}
	}
}
