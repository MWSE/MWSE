#include "NICollisionSwitch.h"

namespace NI {
	CollisionSwitch::CollisionSwitch() {
		vTable.asNode = reinterpret_cast<NI::Node_vTable*>(0x74F418);
		BITMASK_SET_ON(flags, flagCollision);
	}

	bool CollisionSwitch::getCollisionActive() {
		return BITMASK_TEST(flags, flagCollision);
	}

	void CollisionSwitch::setCollisionActive(bool active) {
		BITMASK_SET(flags, flagCollision, active);
	}

	Pointer<CollisionSwitch> CollisionSwitch::create() {
		return new CollisionSwitch();
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::CollisionSwitch)
