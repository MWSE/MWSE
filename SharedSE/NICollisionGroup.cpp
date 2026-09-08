#include "NICollisionGroup.h"

#include "NINode.h"
#include "NIRTTI.h"

#include "ExceptionUtil.h"

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
#include "MWSEConfig.h"
#endif

namespace NI {
	// Actors attach their collision volume to the scene node root only
	// (MobileActor::setupCollision), so the per-frame subtree walk is wasted on
	// them. Entries leave with their collider (removeCollider, removeAll).
	struct ColliderInfo {
		bool rootOnly = false;
		bool scanned = false;
		unsigned int scannedAtWalk = 0;
	};
	static std::unordered_map<const AVObject*, ColliderInfo> colliderInfos;
	static unsigned int walkCounter = 0;
	constexpr auto rescanIntervalWalks = 64u;

	static bool subtreeHasCollisionVolumeBelow(const AVObject* object) {
		if (!object->isInstanceOfType(RTTIStaticPtr::NiNode)) {
			return false;
		}
		for (const auto& child : static_cast<const Node*>(object)->children) {
			if (child && (child->modelABV || subtreeHasCollisionVolumeBelow(child))) {
				return true;
			}
		}
		return false;
	}

	static void forgetCollider(const AVObject* collider) {
		colliderInfos.erase(collider);
	}

	bool CollisionGroup::containsCollider(AVObject* obj) {
#if defined(SE_NI_COLLISIONGROUP_FNADDR_CONTAINSCOLLIDER) && SE_NI_COLLISIONGROUP_FNADDR_CONTAINSCOLLIDER > 0
		const auto NI_CollisionGroup_containsCollider = reinterpret_cast<bool(__thiscall*)(CollisionGroup*, AVObject*)>(SE_NI_COLLISIONGROUP_FNADDR_CONTAINSCOLLIDER);
		return NI_CollisionGroup_containsCollider(this, obj);
#else
		throw not_implemented_exception();
#endif
	}

	void CollisionGroup::addCollider(AVObject* obj) {
#if defined(SE_NI_COLLISIONGROUP_FNADDR_ADDCOLLIDER) && SE_NI_COLLISIONGROUP_FNADDR_ADDCOLLIDER > 0
		const auto NI_CollisionGroup_addCollider = reinterpret_cast<void(__thiscall*)(CollisionGroup*, AVObject*, int, int)>(SE_NI_COLLISIONGROUP_FNADDR_ADDCOLLIDER);
		NI_CollisionGroup_addCollider(this, obj, -1, 1);
#else
		throw not_implemented_exception();
#endif
	}

	void CollisionGroup::removeCollider(AVObject* obj) {
		forgetCollider(obj);
#if defined(SE_NI_COLLISIONGROUP_FNADDR_REMOVECOLLIDER) && SE_NI_COLLISIONGROUP_FNADDR_REMOVECOLLIDER > 0
		const auto NI_CollisionGroup_removeCollider = reinterpret_cast<void(__thiscall*)(CollisionGroup*, AVObject*)>(SE_NI_COLLISIONGROUP_FNADDR_REMOVECOLLIDER);
		NI_CollisionGroup_removeCollider(this, obj);
#else
		throw not_implemented_exception();
#endif
	}

	void CollisionGroup::removeCollidee(AVObject* obj) {
#if defined(SE_NI_COLLISIONGROUP_FNADDR_REMOVECOLLIDEE) && SE_NI_COLLISIONGROUP_FNADDR_REMOVECOLLIDEE > 0
		const auto NI_CollisionGroup_removeCollidee = reinterpret_cast<void(__thiscall*)(CollisionGroup*, AVObject*)>(SE_NI_COLLISIONGROUP_FNADDR_REMOVECOLLIDEE);
		NI_CollisionGroup_removeCollidee(this, obj);
#else
		throw not_implemented_exception();
#endif
	}

	void CollisionGroup::removeAll() {
		for (const auto& record : colliders) {
			if (record) {
				forgetCollider(record->sgObject);
			}
		}
#if defined(SE_NI_COLLISIONGROUP_FNADDR_REMOVEALL) && SE_NI_COLLISIONGROUP_FNADDR_REMOVEALL > 0
		const auto NI_CollisionGroup_removeAll = reinterpret_cast<void(__thiscall*)(CollisionGroup*)>(SE_NI_COLLISIONGROUP_FNADDR_REMOVEALL);
		NI_CollisionGroup_removeAll(this);
#else
		throw not_implemented_exception();
#endif
	}

	void CollisionGroup::updateWorldData() {
#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		if (mwse::Configuration::UsePhysicsOptimizations) {
			++walkCounter;
			for (const auto& record : colliders) {
				if (!record || !record->sgObject) {
					continue;
				}
				const auto collider = record->sgObject;
				auto& info = colliderInfos[collider];
				if (!info.scanned || walkCounter - info.scannedAtWalk >= rescanIntervalWalks) {
					info.rootOnly = !subtreeHasCollisionVolumeBelow(collider);
					info.scanned = true;
					info.scannedAtWalk = walkCounter;
				}
				if (info.rootOnly) {
					collider->updateWorldCollisionVolume();
				}
				else {
					collider->updateCollisionData();
				}
			}
			return;
		}

		// Clear so Node::updateCollisionData falls back to the full walk.
		colliderInfos.clear();
#endif

#if defined(SE_NI_COLLISIONGROUP_FNADDR_UPDATEWORLDDATA) && SE_NI_COLLISIONGROUP_FNADDR_UPDATEWORLDDATA > 0
		const auto NI_CollisionGroup_updateWorldData = reinterpret_cast<void(__thiscall*)(CollisionGroup*)>(SE_NI_COLLISIONGROUP_FNADDR_UPDATEWORLDDATA);
		NI_CollisionGroup_updateWorldData(this);
#else
		throw not_implemented_exception();
#endif
	}

	bool CollisionGroup::isRootOnlyCollider(const AVObject* collider) {
		const auto it = colliderInfos.find(collider);
		return it != colliderInfos.end() && it->second.rootOnly;
	}
}
