#include "NICollisionSwitch.h"

#include "NIPick.h"

#include "ExceptionUtil.h"

#include <cstring>

namespace NI {
	CollisionSwitch::CollisionSwitch() {
#if defined(SE_NI_COLLISIONSWITCH_VTBL) && SE_NI_COLLISIONSWITCH_VTBL > 0
		vTable.asNode = reinterpret_cast<NI::Node_vTable*>(SE_NI_COLLISIONSWITCH_VTBL);
		BITMASK_SET_ON(flags, flagCollision);
#else
		throw not_implemented_exception();
#endif
	}

	bool CollisionSwitch::getCollisionActive() const {
		return BITMASK_TEST(flags, flagCollision);
	}

	void CollisionSwitch::setCollisionActive(bool active) {
		BITMASK_SET(flags, flagCollision, active);
	}

	static const char* pickProxyObjectName = "PickProxy";

	void CollisionSwitch::linkObject(Stream* stream) {
#if defined(SE_NI_NODE_FNADDR_LINKOBJECT) && SE_NI_NODE_FNADDR_LINKOBJECT > 0
		const auto NI_Node_LinkObject = reinterpret_cast<void(__thiscall*)(CollisionSwitch*, Stream*)>(SE_NI_NODE_FNADDR_LINKOBJECT);
		NI_Node_LinkObject(this, stream);

		// Detect special case name for pick proxies.
		if (name && strcmp(name, pickProxyObjectName) == 0) {
			BITMASK_SET_ON(flags, flagPickProxy);

			// Ensure pick proxy starts culled.
			if (children.filledCount > 0) {
				auto& proxy = children.at(0);
				proxy->setAppCulled(true);
			}
		}
#else
		throw not_implemented_exception();
#endif
	}

	bool CollisionSwitch::findIntersections(Vector3* position, Vector3* direction, Pick* pick) {
#if defined(SE_NI_NODE_FNADDR_FINDINTERSECTIONS) && SE_NI_NODE_FNADDR_FINDINTERSECTIONS > 0
		if (pick->observeAppCullFlag && getAppCulled() || !BITMASK_TEST(flags, flagCollision)) {
			return false;
		}
		else if (BITMASK_TEST(flags, flagPickProxy)) {
			// Use first child as a proxy for picking (raycasts) on this subtree.
			bool result = false;
			if (children.filledCount > 0) {
				auto& proxy = children.at(0);
				proxy->setAppCulled(false);
				result = proxy->vTable.asAVObject->findIntersections(proxy, position, direction, pick);
				proxy->setAppCulled(true);
			}
			return result;
		}
		else {
			const auto NI_Node_FindIntersections = reinterpret_cast<bool(__thiscall*)(Node*, Vector3*, Vector3*, Pick*)>(SE_NI_NODE_FNADDR_FINDINTERSECTIONS);
			return NI_Node_FindIntersections(this, position, direction, pick);
		}
#else
		throw not_implemented_exception();
#endif
	}

	Pointer<CollisionSwitch> CollisionSwitch::create() {
		return new CollisionSwitch();
	}
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::CollisionSwitch)
#endif
