#include "NIUVController.h"

#include "ExceptionUtil.h"

namespace NI {
	void UVController::copy(UVController* to) const {
		to->textureSet = textureSet;
#if defined(SE_NI_UVCONTROLLER_FNADDR_COPY) && SE_NI_UVCONTROLLER_FNADDR_COPY > 0
		const auto NI_UVController_copy = reinterpret_cast<void(__thiscall*)(const UVController*, UVController*)>(SE_NI_UVCONTROLLER_FNADDR_COPY);
		NI_UVController_copy(this, to);
#else
		throw not_implemented_exception();
#endif
	}
}
