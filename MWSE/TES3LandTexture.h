#pragma once

#include "TES3Object.h"

#include "TES3IteratedList.h"

#include "NIProperty.h"
#include "NISourceTexture.h"

namespace TES3 {
	struct LandTexture : BaseObject {
		struct PropertyMap {
			LandTexture * landTexture; // 0x0
			NI::Pointer<NI::TexturingProperty> textureProperty; // 0x4
		};
		char id[32]; // 0x10
		NI::Pointer<NI::SourceTexture> texture; // 0x30 // Loaded texture.
		char texturePath[260]; // 0x34
		int unknown_0x138;
		int index; // 0x13C
		IteratedList<PropertyMap*> * unknown_0x140; // Relates land textures to texture properties?
	};
	static_assert(sizeof(LandTexture) == 0x144, "TES3::LandTexture failed size validation");
}
