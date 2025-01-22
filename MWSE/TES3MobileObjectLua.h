#pragma once

#include "LuaUtil.h"

#include "TES3Reference.h"
#include "TES3MobileObject.h"

namespace mwse::lua {
	template <typename T>
	void setUserdataForTES3MobileObject(sol::usertype<T>& usertypeDefinition) {
		// Basic property binding.
		usertypeDefinition["objectType"] = sol::readonly_property(&TES3::MobileObject::objectType);
		usertypeDefinition["flags"] = &TES3::MobileObject::actorFlags;
		usertypeDefinition["boundSize2D"] = &TES3::MobileObject::boundSize;
		usertypeDefinition["cellX"] = sol::readonly_property(&TES3::MobileObject::cellX);
		usertypeDefinition["cellY"] = sol::readonly_property(&TES3::MobileObject::cellY);
		usertypeDefinition["collisionData"] = sol::readonly_property(&TES3::MobileObject::getCollisions_lua);
		usertypeDefinition["height"] = &TES3::MobileObject::height;
		usertypeDefinition["inventory"] = sol::readonly_property(&TES3::MobileObject::getInventory);
		usertypeDefinition["movementFlags"] = &TES3::MobileObject::movementFlags;
		usertypeDefinition["playerDistance"] = &TES3::MobileObject::simulationDistance;
		usertypeDefinition["prevMovementFlags"] = &TES3::MobileObject::prevMovementFlags;
		usertypeDefinition["reference"] = sol::readonly_property(&TES3::MobileObject::reference);

		// Vectors we want to handle differently, and allow table aliasing.
		usertypeDefinition["boundSize"] = sol::property(&TES3::MobileObject::getBoundSize, &TES3::MobileObject::setBoundSize);
		usertypeDefinition["impulseVelocity"] = sol::property(&TES3::MobileObject::getImpulseVelocity, &TES3::MobileObject::setImpulseVelocityFromLua);
		usertypeDefinition["position"] = sol::property(&TES3::MobileObject::getPosition, &TES3::MobileObject::setPositionFromLua);
		usertypeDefinition["velocity"] = sol::property(&TES3::MobileObject::getVelocity, &TES3::MobileObject::setVelocityFromLua);

		// Custom property bindings.
		usertypeDefinition["isAffectedByGravity"] = sol::property(&TES3::MobileObject::getAffectedByGravityFlag, &TES3::MobileObject::setAffectedByGravityFlag);
		usertypeDefinition["mobToMobCollision"] = sol::property(&TES3::MobileObject::getMobToMobCollision, &TES3::MobileObject::setMobToMobCollision);
		usertypeDefinition["movementCollision"] = sol::property(&TES3::MobileObject::getMovementCollisionFlag, &TES3::MobileObject::setMovementCollisionFlag);
	}
}
