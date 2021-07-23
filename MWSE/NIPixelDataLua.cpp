#include "NIPixelDataLua.h"

#include "NIObjectLua.h"

#include "Log.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIPixelData.h"
#include "NIRTTI.h"
#include "NISourceTexture.h"

namespace mwse {
	namespace lua {
		void bindNIPixelData() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Binding for NI::PixelData.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<NI::PixelData>("niPixelData");
				usertypeDefinition["new"] = &NI::PixelData::create_lua;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::Object>();
				setUserdataForNIObject(usertypeDefinition);

				// Basic property binding.
				usertypeDefinition["bytesPerPixel"] = sol::readonly_property(&NI::PixelData::bytesPerPixel);
				usertypeDefinition["mipMapLevels"] = sol::readonly_property(&NI::PixelData::mipMapLevels);

				// Basic function binding.
				usertypeDefinition["createSourceTexture"] = &NI::PixelData::createSourceTexture;
				usertypeDefinition["getWidth"] = &NI::PixelData::getWidth_lua;
				usertypeDefinition["getHeight"] = &NI::PixelData::getHeight_lua;
				usertypeDefinition["setPixelsByte"] = &NI::PixelData::setPixelsByte_lua;
				usertypeDefinition["setPixelsFloat"] = &NI::PixelData::setPixelsFloat_lua;
				usertypeDefinition["fill"] = &NI::PixelData::fill_lua;
			}
		}
	}
}
