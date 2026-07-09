#pragma once

#include "NIObjectLua.h"

import NITexture;

namespace mwse::lua {
	template <typename T>
	void setUserdataForNITexture(sol::usertype<T>& usertypeDefinition) {
		setUserdataForNIObjectNET(usertypeDefinition);

		// Basic property binding.
		usertypeDefinition["formatPrefs"] = &NI::Texture::formatPrefs;

		// Functions bound as properties.
		usertypeDefinition["height"] = sol::readonly_property(&NI::Texture::getHeight);
		usertypeDefinition["width"] = sol::readonly_property(&NI::Texture::getWidth);
	}

	void bindNITexture();
}
