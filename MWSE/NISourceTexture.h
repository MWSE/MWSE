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
		const char * fileName; // 0x2C
		const char * platformFileName; // 0x30
		Pointer<PixelData> pixelData; // 0x34
		bool isStatic; // 0x38

		//
		// Static functions.
		//

		static Pointer<SourceTexture> createFromPath(const char* path, SourceTexture::FormatPrefs * formatPrefs = FormatPrefs::DEFAULT_PREFS);
		static Pointer<SourceTexture> createFromPixelData(PixelData* pixelData, SourceTexture::FormatPrefs * formatPrefs = FormatPrefs::DEFAULT_PREFS);

		//
		// Virtual table functions.
		//

		void loadPixelDataFromFile();
		void clearPixelData();

		//
		// Custom functions.
		//

		static Pointer<SourceTexture> createFromPath_lua(const char* path);

	};
	static_assert(sizeof(SourceTexture) == 0x3C, "NI::SourceTexture failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::SourceTexture)
