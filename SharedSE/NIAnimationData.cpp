#include "NIAnimationData.h"

#include "ExceptionUtil.h"

namespace NI {
#if defined(SE_NI_FLOATDATA_FNADDR_GETKEYSIZE) && SE_NI_FLOATDATA_FNADDR_GETKEYSIZE > 0
	unsigned int FloatData::getKeyDataSize() const {
		return reinterpret_cast<unsigned int(__cdecl*)(AnimationKey::KeyType)>(SE_NI_FLOATDATA_FNADDR_GETKEYSIZE)(keyType);
	}
#else
	unsigned int FloatData::getKeyDataSize() const { throw not_implemented_exception(); }
#endif

#if defined(SE_NI_POSDATA_FNADDR_GETKEYSIZE) && SE_NI_POSDATA_FNADDR_GETKEYSIZE > 0
	unsigned int PosData::getKeyDataSize() const {
		return reinterpret_cast<unsigned int(__cdecl*)(AnimationKey::KeyType)>(SE_NI_POSDATA_FNADDR_GETKEYSIZE)(keyType);
	}
#else
	unsigned int PosData::getKeyDataSize() const { throw not_implemented_exception(); }
#endif

	nonstd::span<ColorKey*> ColorData::getKeys() const {
		return nonstd::span(keys, keyCount);
	}
}
