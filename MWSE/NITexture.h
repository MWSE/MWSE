#pragma once

#include "NIObjectNET.h"
#include "NIPixelFormat.h"

namespace NI {
	struct Texture_vTable : Object_vTable {
		unsigned int(__thiscall * getWidth)(Texture*); // 0x2C
		unsigned int(__thiscall * getHeight)(Texture*); // 0x30
	};

	struct Texture : ObjectNET {
		struct FormatPrefs {
			enum struct PixelLayout {
				PALETTIZED_8,
				HIGH_COLOR_16,
				TRUE_COLOR_32,
				COMPRESSED,
				BUMPMAP,
				PIX_DEFAULT
			};
			enum struct MipFlag {
				NO,
				YES,
				MIP_DEFAULT
			};
			enum struct AlphaFormat {
				NONE,
				BINARY,
				SMOOTH,
				ALPHA_DEFAULT
			};
			PixelLayout pixelLayout; // 0x0
			MipFlag mipMapped; // 0x4
			AlphaFormat alphaFormat; // 0x8

			FormatPrefs();
			FormatPrefs(PixelLayout p, MipFlag m, AlphaFormat a);

			static constexpr auto DEFAULT_PREFS = reinterpret_cast<FormatPrefs*>(0x7DE3B4);
			static const FormatPrefs DEFAULT_LUA_PREFS;
		};
		static_assert(sizeof(Texture::FormatPrefs) == 0xC, "NI::Texture::FormatPrefs failed size validation");

		struct RendererData {
			void* vTable;
			Texture* texture;
			int unknown_0x8;
			int unknown_0xC;
		};
		static_assert(sizeof(Texture::RendererData) == 0x10, "NI::Texture::RendererData failed size validation");

		FormatPrefs formatPrefs; // 0x14;
		RendererData* rendererData; // 0x20
		Texture * previousTexture; // 0x24
		Texture * nextTexture; // 0x28

		//
		// vTable wrappers.
		//

		unsigned int getWidth();
		unsigned int getHeight();

	};
	static_assert(sizeof(Texture) == 0x2C, "NI::Texture failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::Texture)
