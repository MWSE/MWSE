#pragma once

#include "NITexture.h"
#include "NIPixelData.h"

namespace NI {
	struct SourceTexture_vTable : Texture_vTable {
		void(__thiscall* loadPixelDataFromFile)(SourceTexture*); // 0x34
		void(__thiscall* clearPixelData)(SourceTexture*); // 0x38
	};
	static_assert(sizeof(SourceTexture_vTable) == 0x3C, "NI::SourceTexture's vtable failed size validation");

	struct SourceTexture : Texture {
		const char* fileName; // 0x2C
		const char* platformFileName; // 0x30
		Pointer<PixelData> pixelData; // 0x34
		bool isStatic; // 0x38

		static Pointer<SourceTexture> createFromPath(const char* path, SourceTexture::FormatPrefs* formatPrefs = FormatPrefs::DEFAULT_PREFS);
		static Pointer<SourceTexture> createFromPixelData(PixelData* pixelData, SourceTexture::FormatPrefs* formatPrefs = FormatPrefs::DEFAULT_PREFS);

		void loadPixelDataFromFile();
		void clearPixelData();

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		// Cached variant that routes through TES3::DataHandler — implemented only in Morrowind-context builds.
		static Pointer<SourceTexture> createFromPath_lua(const char* path, sol::optional<bool> useCached);
#endif
	};
	static_assert(sizeof(SourceTexture) == 0x3C, "NI::SourceTexture failed size validation");
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::SourceTexture)
#endif
