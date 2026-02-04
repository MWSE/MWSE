#include "NIUtil.h"

constexpr auto NI_global_pick = 0x7D12E8;

constexpr auto NI_getAssociatedReference = 0x4C3C40;

namespace NI {
	Pick* getGlobalPick() {
		return *reinterpret_cast<Pick**>(NI_global_pick);
	}

	TES3::Reference* getAssociatedReference(AVObject* object) {
		return reinterpret_cast<TES3::Reference * (__cdecl*)(AVObject*)>(NI_getAssociatedReference)(object);
	}
}