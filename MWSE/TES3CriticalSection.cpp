#include "TES3CriticalSection.h"

namespace TES3 {
	void CriticalSection::enter(const char* id) {
		EnterCriticalSection(this);
	}

	void CriticalSection::leave() {
		LeaveCriticalSection(this);
	}
}
