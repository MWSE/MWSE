#pragma once

#include "LuaUtil.h"

#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		template <typename T>
		void setUserdataForTES3MobileObject(sol::usertype<T>& usertypeDefinition) {
			// Basic property binding.
			usertypeDefinition["objectType"] = sol::readonly_property(&TES3::MobileObject::objectType);
			usertypeDefinition["flags"] = sol::readonly_property(&TES3::MobileObject::actorFlags);
			usertypeDefinition["cellX"] = sol::readonly_property(&TES3::MobileObject::cellX);
			usertypeDefinition["cellY"] = sol::readonly_property(&TES3::MobileObject::cellY);
			usertypeDefinition["collisionData"] = sol::readonly_property(&TES3::MobileObject::getCollisions_lua);
			usertypeDefinition["height"] = sol::readonly_property(&TES3::MobileObject::height);
			usertypeDefinition["inventory"] = sol::readonly_property(&TES3::MobileObject::getInventory);
			usertypeDefinition["movementFlags"] = sol::readonly_property(&TES3::MobileObject::movementFlags);
			usertypeDefinition["playerDistance"] = sol::readonly_property(&TES3::MobileObject::simulationDistance);
			usertypeDefinition["prevMovementFlags"] = sol::readonly_property(&TES3::MobileObject::prevMovementFlags);
			usertypeDefinition["reference"] = sol::readonly_property(&TES3::MobileObject::reference);

			// Vectors we want to handle differently, and allow table aliasing.
			usertypeDefinition["boundSize"] = sol::readonly_property(&TES3::MobileObject::getBoundSize);
			usertypeDefinition["impulseVelocity"] = sol::property(&TES3::MobileObject::getImpulseVelocity, &TES3::MobileObject::setImpulseVelocityFromLua);
			usertypeDefinition["position"] = sol::property(&TES3::MobileObject::getPosition, &TES3::MobileObject::setPositionFromLua);
			usertypeDefinition["velocity"] = sol::property(&TES3::MobileObject::getVelocity, &TES3::MobileObject::setVelocityFromLua);

			// Custom property bindings.
			usertypeDefinition["movementCollision"] = sol::property(&TES3::MobileObject::getMovementCollisionFlag, &TES3::MobileObject::setMovementCollisionFlag);
		}
	}
}
