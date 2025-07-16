#include "NISourceTextureLua.h"

#include "NITextureLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NISourceTexture.h"
#include "NIRTTI.h"

namespace mwse::lua {
	void bindNISourceTexture() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Binding for NI::SourceTexture::FormatPrefs.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::SourceTexture::FormatPrefs>("niFormatPrefs");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["alphaLayout"] = &NI::SourceTexture::FormatPrefs::alphaFormat;
			usertypeDefinition["mipLayout"] = &NI::SourceTexture::FormatPrefs::mipMapped;
			usertypeDefinition["pixelLayout"] = &NI::SourceTexture::FormatPrefs::pixelLayout;
		}

		// Binding for NI::SourceTexture.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::SourceTexture>("niSourceTexture");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::Texture, NI::ObjectNET, NI::Object>();
			setUserdataForNITexture(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["isStatic"] = &NI::SourceTexture::isStatic;
			usertypeDefinition["pixelData"] = &NI::SourceTexture::pixelData;

			// Basic function binding.
			usertypeDefinition["clearPixelData"] = &NI::SourceTexture::clearPixelData;
			usertypeDefinition["loadPixelDataFromFile"] = &NI::SourceTexture::loadPixelDataFromFile;

			// Functions bound as properties.
			usertypeDefinition["fileName"] = sol::readonly_property(&NI::SourceTexture::fileName);
			usertypeDefinition["platformFileName"] = sol::readonly_property(&NI::SourceTexture::platformFileName);

			// Functions that need their results wrapped.
			usertypeDefinition["createFromPath"] = &NI::SourceTexture::createFromPath_lua;
		}
	}
}
