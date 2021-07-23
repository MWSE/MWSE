#include "TES3MobileObject.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "LuaObjectInvalidatedEvent.h"
#include "LuaMobileObjectCollisionEvent.h"

#include "TES3MobileCreature.h"
#include "TES3MobileNPC.h"
#include "TES3MobilePlayer.h"
#include "TES3MobileProjectile.h"
#include "TES3MobileSpellProjectile.h"
#include "TES3Reference.h"

#include "Log.h"

namespace TES3 {

	Vector3 MobileObject::Collision::getNormal() const {
		return TES3::Vector3(quantizedNormal[0] * QUANTIZER, quantizedNormal[1] * QUANTIZER, quantizedNormal[2] * QUANTIZER);
	}

	const auto TES3_MobileObject_Collision_clone = reinterpret_cast<void(__thiscall*)(MobileObject::Collision*, MobileObject::Collision*)>(0x405450);
#pragma optimize( "", off )
	void MobileObject::Collision::clone(Collision* from) {
		// Call the original function.
		TES3_MobileObject_Collision_clone(this, from);

		// Check to see if something was borked.
		__try {
			if (colliderRef) {
				colliderRef->baseObject->objectType;
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			mwse::log::getLog() << "WARNING: Caught invalid collision reference. Setting collision to invalid." << std::endl;
			valid = false;
			from->valid = false;
			colliderRef = nullptr;
			from->colliderRef = nullptr;
		}
	}
#pragma optimize( "", on )

#define TES3_vTable_MobileCreature 0x74AFA4
#define TES3_vTable_MobileNPC 0x74AE6C
#define TES3_vTable_MobilePlayer 0x74B174
#define TES3_vTable_MobileProjectile 0x74B2B4
#define TES3_vTable_SpellProjectile 0x74B360

	const auto TES3_MobileObject_onActorCollision = reinterpret_cast<bool(__thiscall*)(MobileObject*, int)>(0x5615A0);
	bool MobileObject::onActorCollision(int collisionIndex) {
		// Grab the hit reference now, it won't be available after calling the main function.
		TES3::Reference* hitReference = this->arrayCollisionResults[collisionIndex].colliderRef;

		// Call the original function. We can't invoke the vtable here because we overwrite it.
		bool result = TES3_MobileObject_onActorCollision(this, collisionIndex);

		// Fire off our hit event.
		if (mwse::lua::event::MobileObjectCollisionEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::MobileObjectCollisionEvent(this, hitReference));
		}

		return result;
	}

	const auto TES3_MobileObject_onObjectCollision = reinterpret_cast<bool(__thiscall*)(MobileObject*, int, bool)>(0x5615C0);
	bool MobileObject::onObjectCollision(int collisionIndex, bool flag) {
		// Grab the hit reference now, it won't be available after calling the main function.
		TES3::Reference* hitReference = this->arrayCollisionResults[collisionIndex].colliderRef;

		// Call the original function. We can't invoke the vtable here because we overwrite it.
		bool result = TES3_MobileObject_onObjectCollision(this, collisionIndex, flag);

		// Fire off our hit event.
		if (mwse::lua::event::MobileObjectCollisionEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::MobileObjectCollisionEvent(this, hitReference));
		}

		return result;
	}

	const auto TES3_MobileObject_onTerrainCollision = reinterpret_cast<bool(__thiscall*)(MobileObject*, int)>(0x5615E0);
	bool MobileObject::onTerrainCollision(int collisionIndex) {
		// Grab the hit reference now, it won't be available after calling the main function.
		TES3::Reference* hitReference = this->arrayCollisionResults[collisionIndex].colliderRef;

		// Call the original function. We can't invoke the vtable here because we overwrite it.
		bool result = TES3_MobileObject_onTerrainCollision(this, collisionIndex);

		// Fire off our hit event.
		if (mwse::lua::event::MobileObjectCollisionEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::MobileObjectCollisionEvent(this, hitReference));
		}

		return result;
	}

	const auto TES3_MobileObject_onWaterCollision = reinterpret_cast<bool(__thiscall*)(MobileObject*, int)>(0x5615E0);
	bool MobileObject::onWaterCollision(int collisionIndex) {
		// Grab the hit reference now, it won't be available after calling the main function.
		TES3::Reference* hitReference = this->arrayCollisionResults[collisionIndex].colliderRef;

		// Call the original function. We can't invoke the vtable here because we overwrite it.
		bool result = TES3_MobileObject_onWaterCollision(this, collisionIndex);

		// Fire off our hit event.
		if (mwse::lua::event::MobileObjectCollisionEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::MobileObjectCollisionEvent(this, hitReference));
		}

		return result;
	}

	const auto TES3_MobileObject_onActivatorCollision = reinterpret_cast<bool(__thiscall*)(MobileObject*, int)>(0x561600);
	bool MobileObject::onActivatorCollision(int collisionIndex) {
		// Grab the hit reference now, it won't be available after calling the main function.
		TES3::Reference* hitReference = this->arrayCollisionResults[collisionIndex].colliderRef;

		// Call the original function. We can't invoke the vtable here because we overwrite it.
		bool result = TES3_MobileObject_onActivatorCollision(this, collisionIndex);

		// Fire off our hit event.
		if (mwse::lua::event::MobileObjectCollisionEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::MobileObjectCollisionEvent(this, hitReference));
		}

		return result;
	}

	bool MobileObject::isActor() const {
		return vTable.mobileObject->isActor(this);
	}

	void MobileObject::enterLeaveSimulation(bool entering) {
		vTable.mobileObject->enterLeaveSimulation(this, entering);
	}

	const auto TES3_MobileObject_enterLeaveSimulationByDistance = reinterpret_cast<void(__thiscall *)(MobileObject*)>(0x55FFC0);
	void MobileObject::enterLeaveSimulationByDistance() {
		TES3_MobileObject_enterLeaveSimulationByDistance(this);
	}

	const auto TES3_MobileObject_getInventory = reinterpret_cast<IteratedList<ItemStack*> * (__thiscall*)(const MobileObject*)>(0x521620);
	IteratedList<ItemStack*>* MobileObject::getInventory() const {
		return TES3_MobileObject_getInventory(this);
	}

	const auto TES3_MobileObject_getBasePositionIsUnderwater = reinterpret_cast<bool(__thiscall*)(const MobileObject*)>(0x529540);
	bool MobileObject::getBasePositionIsUnderwater() const {
		return TES3_MobileObject_getBasePositionIsUnderwater(this);
	}

	Vector3 MobileObject::getBoundSize() const {
		return Vector3(boundSize.x, boundSize.y, height);
	}

	Vector3* MobileObject::getImpulseVelocity() {
		return &impulseVelocity;
	}

	void MobileObject::setImpulseVelocityFromLua(sol::stack_object value) {
		// Use our util class to support vectors or a table.
		mwse::lua::setVectorFromLua(impulseVelocity, value);
	}

	Vector3* MobileObject::getPosition() {
		// Delegate to reference.
		return &reference->position;
	}

	void MobileObject::setPositionFromLua(sol::stack_object value) {
		// Delegate to reference.
		reference->setPositionFromLua(value);
	}

	Vector3* MobileObject::getVelocity() {
		return &velocity;
	}

	void MobileObject::setVelocityFromLua(sol::stack_object value) {
		// Use our util class to support vectors or a table.
		mwse::lua::setVectorFromLua(velocity, value);
	}

	bool MobileObject::getMovementCollisionFlag() const {
		return bool(actorFlags & MobileActorFlag::CollisionActive);
	}

	void MobileObject::setMovementCollisionFlag(bool collide) {
		if (collide) {
			actorFlags |= MobileActorFlag::CollisionActive;
		}
		else {
			actorFlags &= ~MobileActorFlag::CollisionActive;
		}
	}

	sol::table MobileObject::getCollisions_lua(sol::this_state ts) const {
		sol::state_view state = ts;
		sol::table results = state.create_table();

		for (int i = 0; i < countCollisionResults; i++) {
			Collision* collision = &arrayCollisionResults[i];
			if (collision->valid && collision->collisionType != Collision::CollisionType::None) {
				results.add(collision);
			}
		}

		return results;
	}

	static std::unordered_map<const MobileObject*, sol::object> mobileObjectCache;
	static std::mutex mobileObjectCacheMutex;

	sol::object MobileObject::getOrCreateLuaObject(lua_State* L) const {
		if (this == nullptr) {
			return sol::nil;
		}

		mobileObjectCacheMutex.lock();

		auto cacheHit = mobileObjectCache.find(this);
		if (cacheHit != mobileObjectCache.end()) {
			auto result = cacheHit->second;
			mobileObjectCacheMutex.unlock();
			return result;
		}

		// Make sure we're looking at the main state.
		L = sol::main_thread(L);

		sol::object ref = sol::nil;
		switch ((unsigned int)vTable.mobileObject) {
		case TES3_vTable_MobileCreature:
			ref = sol::make_object(L, static_cast<const TES3::MobileCreature*>(this));
			break;
		case TES3_vTable_MobileNPC:
			ref = sol::make_object_userdata(L, static_cast<const TES3::MobileNPC*>(this));
			break;
		case TES3_vTable_MobilePlayer:
			ref = sol::make_object(L, static_cast<const TES3::MobilePlayer*>(this));
			break;
		case TES3_vTable_MobileProjectile:
			ref = sol::make_object_userdata(L, static_cast<const TES3::MobileProjectile*>(this));
			break;
		case TES3_vTable_SpellProjectile:
			ref = sol::make_object(L, static_cast<const TES3::MobileSpellProjectile*>(this));
			break;
		}

		mobileObjectCache[this] = ref;
		mobileObjectCacheMutex.unlock();

		return ref;
	}

	void MobileObject::clearCachedLuaObject(const MobileObject* object) {
		if (!mobileObjectCache.empty()) {
			mobileObjectCacheMutex.lock();

			// Clear any events that make use of this object.
			auto it = mobileObjectCache.find(object);
			if (it != mobileObjectCache.end()) {
				// Let people know that this object is invalidated.
				mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::ObjectInvalidatedEvent(it->second));

				// Clear any events that make use of this object.
				mwse::lua::event::clearObjectFilter(it->second);

				// Remove it from the cache.
				mobileObjectCache.erase(it);
			}

			mobileObjectCacheMutex.unlock();
		}
	}

	void MobileObject::clearCachedLuaObjects() {
		mobileObjectCacheMutex.lock();
		mobileObjectCache.clear();
		mobileObjectCacheMutex.unlock();
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3_MOBILEOBJECT(TES3::MobileObject)
