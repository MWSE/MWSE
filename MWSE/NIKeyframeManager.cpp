#include "NIKeyframeManager.h"

#include "MemoryUtil.h"

namespace NI {
	const auto NI_Sequence_dtor = reinterpret_cast<void(__thiscall*)(Sequence*)>(0x70F720);
	void Sequence::release() {
		NI_Sequence_dtor(this);
		mwse::tes3::free(this);
	}

	Pointer<KeyframeController> Sequence::getController(const char* name) const {
		for (int i = 0; i < objectNames.endIndex; ++i) {
			if (_stricmp(objectNames[i], name) == 0) {
				return controllers[i].get();
			}
		}
		return nullptr;
	}

	const auto NI_KeyframeManager_addSequence = reinterpret_cast<void(__thiscall*)(KeyframeManager*, Sequence*)>(0x7111A0);
	void KeyframeManager::addSequence(Sequence* seq) {
		NI_KeyframeManager_addSequence(this, seq);
	}

	void KeyframeManager::removeSequence(Sequence* seq) {
		// Best guess implementation, as this was not linked into Morrowind.
		const char* key = seq->name;
		if (this->sequences.eraseKey(key)) {
			seq->manager = nullptr;
		}
	}

	const auto NI_KeyframeManager_removeAll = reinterpret_cast<void(__thiscall*)(KeyframeManager*)>(0x711240);
	void KeyframeManager::removeAll() {
		NI_KeyframeManager_removeAll(this);
	}

	const auto NI_KeyframeManager_activateSequence = reinterpret_cast<bool(__thiscall*)(KeyframeManager*, Sequence*)>(0x711360);
	bool KeyframeManager::activateSequence(Sequence* seq) {
		return NI_KeyframeManager_activateSequence(this, seq);
	}

	const auto NI_KeyframeManager_deactivateSequence = reinterpret_cast<bool(__thiscall*)(KeyframeManager*, Sequence*)>(0x711390);
	bool KeyframeManager::deactivateSequence(Sequence* seq) {
		return NI_KeyframeManager_deactivateSequence(this, seq);
	}

	const auto NI_KeyframeManager_deactivateAll = reinterpret_cast<void(__thiscall*)(KeyframeManager*)>(0x7113C0);
	void KeyframeManager::deactivateAll() {
		NI_KeyframeManager_deactivateAll(this);
	}

}