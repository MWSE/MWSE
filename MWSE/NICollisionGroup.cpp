#include "NICollisionGroup.h"

namespace NI {
	const auto TES3_CollisionGroup_containsCollider = reinterpret_cast<bool(__thiscall*)(CollisionGroup*, AVObject*)>(0x6FD800);
	bool CollisionGroup::containsCollider(AVObject* obj) {
		return TES3_CollisionGroup_containsCollider(this, obj);
	}

	const auto TES3_CollisionGroup_addCollider = reinterpret_cast<void(__thiscall*)(CollisionGroup*, AVObject*, int, int)>(0x6FD440);
	void CollisionGroup::addCollider(AVObject* obj) {
		return TES3_CollisionGroup_addCollider(this, obj, -1, 1);
	}

	const auto TES3_CollisionGroup_removeCollider = reinterpret_cast<void(__thiscall*)(CollisionGroup*, AVObject*)>(0x6FD5A0);
	void CollisionGroup::removeCollider(AVObject* obj) {
		return TES3_CollisionGroup_removeCollider(this, obj);
	}
}
