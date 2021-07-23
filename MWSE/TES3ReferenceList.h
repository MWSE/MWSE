#pragma once

#include "TES3Defines.h"

#include "TES3Reference.h"

namespace TES3 {
	struct ReferenceList : LinkedObjectList<Reference> {
		Cell * cell;

		//
		// Other related this-call functions.
		//

		void insertAfter(Reference* after, Reference* ref);
		void insertAtEnd(Reference* ref);
		void remove(Reference* ref);
	};
	static_assert(sizeof(ReferenceList) == 0x10, "TES3::ReferenceList failed size validation");
}
