#include "NISourceTexture.h"

#include "ExceptionUtil.h"

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
#include "TES3DataHandler.h"
#endif

namespace NI {
	Pointer<SourceTexture> SourceTexture::createFromPath(const char* path, SourceTexture::FormatPrefs* formatPrefs) {
#if defined(SE_NI_SOURCETEXTURE_FNADDR_CREATEFROMPATH) && SE_NI_SOURCETEXTURE_FNADDR_CREATEFROMPATH > 0
		const auto NI_SourceTexture_createFromPath = reinterpret_cast<SourceTexture*(__cdecl*)(const char*, SourceTexture::FormatPrefs*)>(SE_NI_SOURCETEXTURE_FNADDR_CREATEFROMPATH);
		return NI_SourceTexture_createFromPath(path, formatPrefs);
#else
		throw not_implemented_exception();
#endif
	}

	Pointer<SourceTexture> SourceTexture::createFromPixelData(PixelData* pixelData, SourceTexture::FormatPrefs* formatPrefs) {
#if defined(SE_NI_SOURCETEXTURE_FNADDR_CREATEFROMPIXELDATA) && SE_NI_SOURCETEXTURE_FNADDR_CREATEFROMPIXELDATA > 0 && defined(SE_NI_SOURCETEXTURE_GLOBADDR_BPRELOAD) && SE_NI_SOURCETEXTURE_GLOBADDR_BPRELOAD > 0
		const auto NI_SourceTexture_bPreload = reinterpret_cast<bool*>(SE_NI_SOURCETEXTURE_GLOBADDR_BPRELOAD);
		const auto NI_SourceTexture_createFromPixelData = reinterpret_cast<SourceTexture*(__cdecl*)(PixelData*, SourceTexture::FormatPrefs*)>(SE_NI_SOURCETEXTURE_FNADDR_CREATEFROMPIXELDATA);
		bool preload = false;
		std::swap(*NI_SourceTexture_bPreload, preload);
		auto result = NI_SourceTexture_createFromPixelData(pixelData, formatPrefs);
		std::swap(*NI_SourceTexture_bPreload, preload);
		return result;
#else
		throw not_implemented_exception();
#endif
	}

	void SourceTexture::loadPixelDataFromFile() {
		vTable.asSourceTexture->loadPixelDataFromFile(this);
	}

	void SourceTexture::clearPixelData() {
		vTable.asSourceTexture->clearPixelData(this);
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1 && defined(SE_IS_MWSE) && SE_IS_MWSE == 1
	Pointer<SourceTexture> SourceTexture::createFromPath_lua(const char* path, sol::optional<bool> useCached) {
		// Try to use caching if possible.
		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler && useCached.value_or(true)) {
			return TES3::DataHandler::get()->loadSourceTexture(path);
		}

		return createFromPath(path);
	}
#endif
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::SourceTexture)
#endif
