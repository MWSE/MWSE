#pragma once

#include "TES3Object.h"

#include "NIObject.h"

namespace TES3 {
	struct Land : BaseObject {
		NI::Pointer<NI::Node> sceneNode; // 0x10
		unsigned int flags; // 0x14 // Data chunk. Masked with & 0xFFFFFC07
		unsigned int fileOffset; // 0x18
		NI::Node** landNodes; // 0x1C // Not owned by the Land.
		void* positionData; // 0x20
		void* textureUVData; // 0x24
		void* colourData; // 0x28
		unsigned short textureIndices[256]; // 0x2C
		NI::Pointer<NI::Object> unknown_0x22C;
		int gridX; // 0x230
		int gridY; // 0x234
		float minHeight; // 0x238
		float maxHeight; // 0x23C

		Land() = delete;
		~Land() = delete;

		std::reference_wrapper<decltype(Land::textureIndices)> getTextureIndices();
	};
	static_assert(sizeof(Land) == 0x240, "TES3::Land failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Land)
