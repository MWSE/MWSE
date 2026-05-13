#pragma once

#include "NINode.h"

namespace NI {
	namespace BillboardNodeFlags {
		typedef unsigned short value_type;
		const int ModeBit = 5;

		enum BillboardNodeFlags {
			AlwaysFaceCamera = 0,
			RotateAboutUp = 0x20,
			RigidFaceCamera = 0x40,
			Mask = 0x60
		};
	}

	struct BillboardNode : Node {
		float savedTime;
		bool updateControllers;

		BillboardNode();
		static Pointer<BillboardNode> create();

		void rotateToCamera(Camera* camera);

		unsigned int getMode() const;
		void setMode(unsigned int mode);
	};
	static_assert(sizeof(BillboardNode) == 0xB8, "NI::BillboardNode failed size validation");
}
