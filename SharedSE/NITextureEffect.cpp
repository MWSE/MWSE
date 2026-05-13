#include "NITextureEffect.h"

#include "ExceptionUtil.h"

namespace NI {
#if defined(SE_NI_TEXTUREEFFECT_FNADDR_CTOR) && SE_NI_TEXTUREEFFECT_FNADDR_CTOR > 0
	TextureEffect::TextureEffect() {
		reinterpret_cast<void(__thiscall*)(const DynamicEffect*)>(SE_NI_TEXTUREEFFECT_FNADDR_CTOR)(this);
	}
#else
	TextureEffect::TextureEffect() { throw not_implemented_exception(); }
#endif

	TextureEffect::~TextureEffect() {
		vTable.asObject->destructor(this, 0);
	}

	Pointer<TextureEffect> TextureEffect::create() {
		return new TextureEffect();
	}
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::TextureEffect)
#endif
