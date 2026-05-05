#include "NISourceTexture.h"

namespace NI {
	const auto NI_SourceTexture_createFromPath = reinterpret_cast<SourceTexture*(__cdecl*)(const char*, SourceTexture::FormatPrefs*)>(0x6DE7F0);
	Pointer<SourceTexture> SourceTexture::createFromPath(const char* path, SourceTexture::FormatPrefs* formatPrefs) {
		return NI_SourceTexture_createFromPath(path, formatPrefs);
	}

	const auto NI_SourceTexture_bPreload = reinterpret_cast<bool*>(0x7C4C90);
	const auto NI_SourceTexture_createFromPixelData = reinterpret_cast<SourceTexture*(__cdecl*)(PixelData*, SourceTexture::FormatPrefs*)>(0x6DE940);
	Pointer<SourceTexture> SourceTexture::createFromPixelData(PixelData* pixelData, SourceTexture::FormatPrefs* formatPrefs) {
		bool preload = false;
		std::swap(*NI_SourceTexture_bPreload, preload);
		auto result = NI_SourceTexture_createFromPixelData(pixelData, formatPrefs);
		std::swap(*NI_SourceTexture_bPreload, preload);
		return result;
	}

	void SourceTexture::loadPixelDataFromFile() {
		vTable.asSourceTexture->loadPixelDataFromFile(this);
	}

	void SourceTexture::clearPixelData() {
		vTable.asSourceTexture->clearPixelData(this);
	}
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::SourceTexture)
#endif
