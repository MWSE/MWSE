#include "NIKeyframeManager.h"

#include "ExceptionUtil.h"
#include "MemoryUtil.h"

namespace NI {
	void Sequence::release() {
#if defined(SE_NI_SEQUENCE_FNADDR_DTOR) && SE_NI_SEQUENCE_FNADDR_DTOR > 0
		const auto NI_Sequence_dtor = reinterpret_cast<void(__thiscall*)(Sequence*)>(SE_NI_SEQUENCE_FNADDR_DTOR);
		NI_Sequence_dtor(this);
#if defined(SE_MEMORY_FNADDR_FREE) && SE_MEMORY_FNADDR_FREE > 0
		se::memory::free(this);
#else
		throw not_implemented_exception();
#endif
#else
		throw not_implemented_exception();
#endif
	}

	void KeyframeManager::activateSequence(Sequence* seq) {
#if defined(SE_NI_KEYFRAMEMANAGER_FNADDR_ACTIVATESEQUENCE) && SE_NI_KEYFRAMEMANAGER_FNADDR_ACTIVATESEQUENCE > 0
		const auto NI_KeyframeManager_activateSequence = reinterpret_cast<void(__thiscall*)(KeyframeManager*, Sequence*)>(SE_NI_KEYFRAMEMANAGER_FNADDR_ACTIVATESEQUENCE);
		NI_KeyframeManager_activateSequence(this, seq);
#else
		throw not_implemented_exception();
#endif
	}

	void KeyframeManager::deactivateSequence(Sequence* seq) {
#if defined(SE_NI_KEYFRAMEMANAGER_FNADDR_DEACTIVATESEQUENCE) && SE_NI_KEYFRAMEMANAGER_FNADDR_DEACTIVATESEQUENCE > 0
		const auto NI_KeyframeManager_deactivateSequence = reinterpret_cast<void(__thiscall*)(KeyframeManager*, Sequence*)>(SE_NI_KEYFRAMEMANAGER_FNADDR_DEACTIVATESEQUENCE);
		NI_KeyframeManager_deactivateSequence(this, seq);
#else
		throw not_implemented_exception();
#endif
	}
}
