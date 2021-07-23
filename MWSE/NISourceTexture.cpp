#include "NISourceTexture.h"

namespace NI {
	const auto NI_SourceTexture_createFromPath = reinterpret_cast<SourceTexture*(__cdecl*)(const char*, SourceTexture::FormatPrefs *)>(0x6DE7F0);
	Pointer<SourceTexture> SourceTexture::createFromPath(const char* path, SourceTexture::FormatPrefs * formatPrefs) {
		return NI_SourceTexture_createFromPath(path, formatPrefs);
	}

	const auto NI_SourceTexture_createFromPixelData = reinterpret_cast<SourceTexture*(__cdecl*)(PixelData*, SourceTexture::FormatPrefs *)>(0x6DE940);
	Pointer<SourceTexture> SourceTexture::createFromPixelData(PixelData* pixelData, SourceTexture::FormatPrefs * formatPrefs) {
		return NI_SourceTexture_createFromPixelData(pixelData, formatPrefs);
	}

	void SourceTexture::loadPixelDataFromFile() {
		vTable.asSourceTexture->loadPixelDataFromFile(this);
	}

	void SourceTexture::clearPixelData() {
		vTable.asSourceTexture->clearPixelData(this);
	}

	Pointer<SourceTexture> SourceTexture::createFromPath_lua(const char* path) {
		return createFromPath(path);
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::SourceTexture)
