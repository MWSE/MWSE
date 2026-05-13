#include "NICollisionGroup.h"

#include "ExceptionUtil.h"

namespace NI {
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
#if defined(SE_NI_COLLISIONGROUP_FNADDR_REMOVECOLLIDER) && SE_NI_COLLISIONGROUP_FNADDR_REMOVECOLLIDER > 0
		const auto NI_CollisionGroup_removeCollider = reinterpret_cast<void(__thiscall*)(CollisionGroup*, AVObject*)>(SE_NI_COLLISIONGROUP_FNADDR_REMOVECOLLIDER);
		NI_CollisionGroup_removeCollider(this, obj);
#else
		throw not_implemented_exception();
#endif
	}
}
