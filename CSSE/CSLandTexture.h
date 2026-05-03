#pragma once

#include "CSBaseObject.h"

#include "NITexture.h"

namespace se::cs {
	struct LandTexture : BaseObject {
		char objectID[32]; // 0x10
		NI::Pointer<NI::Texture> texture; // 0x30
		char texturePath[MAX_PATH]; // 0x34
		void* propertyMapList; // 0x138
		int index; // 0x13C
		int unknown_0x140;
		int unknown_0x144;
	};
	static_assert(sizeof(LandTexture) == 0x148, "TES3::LandTexture failed size validation");
}
