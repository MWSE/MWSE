#pragma once

#include "NINode.h"

#include "BitUtil.h"

namespace NI {
	struct CollisionSwitch : Node {
		static constexpr unsigned short flagCollision = 0x20;
		static constexpr unsigned short flagPickProxy = 0x400; // Added by MWSE

		CollisionSwitch();

		bool getCollisionActive() const;
		void setCollisionActive(bool active);

		void linkObject(Stream* stream);
		bool findIntersections(TES3::Vector3* position, TES3::Vector3* direction, Pick* pick);

		static Pointer<CollisionSwitch> create();
	};
	static_assert(sizeof(CollisionSwitch) == 0xB0, "NI::CollisionSwitch failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::CollisionSwitch)