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

	Pointer<KeyframeController> Sequence::getController(const char* name) const {
		for (int i = 0; i < objectNames.endIndex; ++i) {
			if (_stricmp(objectNames[i], name) == 0) {
				return controllers[i].get();
			}
		}
		return nullptr;
	}

	void KeyframeManager::addSequence(Sequence* seq) {
#if defined(SE_NI_KEYFRAMEMANAGER_FNADDR_ADDSEQUENCE) && SE_NI_KEYFRAMEMANAGER_FNADDR_ADDSEQUENCE > 0
		const auto NI_KeyframeManager_addSequence = reinterpret_cast<void(__thiscall*)(KeyframeManager*, Sequence*)>(SE_NI_KEYFRAMEMANAGER_FNADDR_ADDSEQUENCE);
		NI_KeyframeManager_addSequence(this, seq);
#else
		throw not_implemented_exception();
#endif
	}

	void KeyframeManager::removeSequence(Sequence* seq) {
		const char* key = seq->name;
		if (this->sequences.eraseKey(key)) {
			seq->manager = nullptr;
		}
	}

	void KeyframeManager::removeAll() {
#if defined(SE_NI_KEYFRAMEMANAGER_FNADDR_REMOVEALL) && SE_NI_KEYFRAMEMANAGER_FNADDR_REMOVEALL > 0
		const auto NI_KeyframeManager_removeAll = reinterpret_cast<void(__thiscall*)(KeyframeManager*)>(SE_NI_KEYFRAMEMANAGER_FNADDR_REMOVEALL);
		NI_KeyframeManager_removeAll(this);
#else
		throw not_implemented_exception();
#endif
	}

	bool KeyframeManager::activateSequence(Sequence* seq) {
#if defined(SE_NI_KEYFRAMEMANAGER_FNADDR_ACTIVATESEQUENCE) && SE_NI_KEYFRAMEMANAGER_FNADDR_ACTIVATESEQUENCE > 0
		const auto NI_KeyframeManager_activateSequence = reinterpret_cast<bool(__thiscall*)(KeyframeManager*, Sequence*)>(SE_NI_KEYFRAMEMANAGER_FNADDR_ACTIVATESEQUENCE);
		return NI_KeyframeManager_activateSequence(this, seq);
#else
		throw not_implemented_exception();
#endif
	}

	bool KeyframeManager::deactivateSequence(Sequence* seq) {
#if defined(SE_NI_KEYFRAMEMANAGER_FNADDR_DEACTIVATESEQUENCE) && SE_NI_KEYFRAMEMANAGER_FNADDR_DEACTIVATESEQUENCE > 0
		const auto NI_KeyframeManager_deactivateSequence = reinterpret_cast<bool(__thiscall*)(KeyframeManager*, Sequence*)>(SE_NI_KEYFRAMEMANAGER_FNADDR_DEACTIVATESEQUENCE);
		return NI_KeyframeManager_deactivateSequence(this, seq);
#else
		throw not_implemented_exception();
#endif
	}

	void KeyframeManager::deactivateAll() {
#if defined(SE_NI_KEYFRAMEMANAGER_FNADDR_DEACTIVATEALL) && SE_NI_KEYFRAMEMANAGER_FNADDR_DEACTIVATEALL > 0
		const auto NI_KeyframeManager_deactivateAll = reinterpret_cast<void(__thiscall*)(KeyframeManager*)>(SE_NI_KEYFRAMEMANAGER_FNADDR_DEACTIVATEALL);
		NI_KeyframeManager_deactivateAll(this);
#else
		throw not_implemented_exception();
#endif
	}
}
