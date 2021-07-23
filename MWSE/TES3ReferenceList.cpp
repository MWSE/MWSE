#include "TES3ReferenceList.h"

namespace TES3 {
	const auto TES3_ReferenceList_insertAfter = reinterpret_cast<void(__thiscall*)(ReferenceList*, Reference*, Reference*)>(0x4F19F0);
	void ReferenceList::insertAfter(Reference* after, Reference* ref) {
		TES3_ReferenceList_insertAfter(this, after, ref);
	}

	void ReferenceList::insertAtEnd(Reference* ref) {
		TES3_ReferenceList_insertAfter(this, tail, ref);
	}

	const auto TES3_ReferenceList_remove = reinterpret_cast<void(__thiscall*)(ReferenceList*, Reference*)>(0x4F19A0);
	void ReferenceList::remove(Reference* ref) {

	}
}
