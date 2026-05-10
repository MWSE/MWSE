#include "NIUtil.h"

#include "ExceptionUtil.h"

namespace NI {
	Pick* getGlobalPick() {
#if defined(SE_NI_GLOBAL_PICK) && SE_NI_GLOBAL_PICK > 0
		return *reinterpret_cast<Pick**>(SE_NI_GLOBAL_PICK);
#else
		throw not_implemented_exception();
#endif
	}

	GameReferenceType* getAssociatedReference(AVObject* object) {
#if defined(SE_NI_GETASSOCIATEDREFERENCE) && SE_NI_GETASSOCIATEDREFERENCE > 0
		return reinterpret_cast<GameReferenceType* (__cdecl*)(AVObject*)>(SE_NI_GETASSOCIATEDREFERENCE)(object);
#else
throw not_implemented_exception();
#endif
	}
}
