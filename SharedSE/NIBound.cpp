#include "NIBound.h"

#include "ExceptionUtil.h"

namespace NI {
	void Bound::computeFromData(unsigned int vertexCount, const Vector3* vertices, unsigned int stride) {
#if defined(SE_NI_BOUND_FNADDR_COMPUTEFROMDATA) && SE_NI_BOUND_FNADDR_COMPUTEFROMDATA > 0
		const auto NI_Bound_computeFromData = reinterpret_cast<void(__thiscall*)(Bound*, unsigned int, const Vector3*, unsigned int)>(SE_NI_BOUND_FNADDR_COMPUTEFROMDATA);
		NI_Bound_computeFromData(this, vertexCount, vertices, stride);
#else
		throw not_implemented_exception();
#endif
	}
}
