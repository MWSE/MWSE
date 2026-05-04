#include "NIKeyframeManager.h"

#include "MemoryUtil.h"
#include "ExceptionUtil.h"

namespace NI {
	const auto NI_Sequence_dtor = reinterpret_cast<void(__thiscall*)(Sequence*)>(0x70F720);
	void Sequence::release() {
		NI_Sequence_dtor(this);
#if defined(SE_MEMORY_FNADDR_FREE) && SE_MEMORY_FNADDR_FREE > 0
		se::memory::free(this);
#else
		throw not_implemented_exception();
#endif
	}

	const auto NI_KeyframeManager_activateSequence = reinterpret_cast<void(__thiscall*)(KeyframeManager*, Sequence*)>(0x711360);
	void KeyframeManager::activateSequence(Sequence* seq) {
		NI_KeyframeManager_activateSequence(this, seq);
	}

	const auto NI_KeyframeManager_deactivateSequence = reinterpret_cast<void(__thiscall*)(KeyframeManager*, Sequence*)>(0x711390);
	void KeyframeManager::deactivateSequence(Sequence* seq) {
		NI_KeyframeManager_deactivateSequence(this, seq);
	}
}
