#pragma once

#include "NIDynamicEffectLua.h"

import NITextureEffect;

namespace mwse::lua {
	template <typename T>
	void setUserdataForNITextureEffect(sol::usertype<T>& usertypeDefinition) {
		setUserdataForNIDynamicEffect(usertypeDefinition);

		// Basic property binding. 
		usertypeDefinition["sourceTexture"] = &NI::TextureEffect::sourceTexture;
	}

	void bindNITextureEffect();
}
