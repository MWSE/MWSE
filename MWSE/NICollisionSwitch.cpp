#include "NICollisionSwitch.h"

#include "NIPick.h"

namespace NI {
	CollisionSwitch::CollisionSwitch() {
		vTable.asNode = reinterpret_cast<NI::Node_vTable*>(0x74F418);
		BITMASK_SET_ON(flags, flagCollision);
	}

	bool CollisionSwitch::getCollisionActive() const {
		return BITMASK_TEST(flags, flagCollision);
	}

	void CollisionSwitch::setCollisionActive(bool active) {
		BITMASK_SET(flags, flagCollision, active);
	}

	const char* pickProxyObjectName = "PickProxy";
	const auto NI_Node_LinkObject = reinterpret_cast<void(__thiscall*)(CollisionSwitch*, Stream*)>(0x6CA7F0);

	void CollisionSwitch::linkObject(Stream* stream) {
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
	}

	const auto NI_Node_FindIntersections = reinterpret_cast<bool(__thiscall*)(Node*, TES3::Vector3*, TES3::Vector3*, Pick*)>(0x6C9F20);

	bool CollisionSwitch::findIntersections(TES3::Vector3* position, TES3::Vector3* direction, Pick* pick) {
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
			return NI_Node_FindIntersections(this, position, direction, pick);
		}
	}

	Pointer<CollisionSwitch> CollisionSwitch::create() {
		return new CollisionSwitch();
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::CollisionSwitch)
