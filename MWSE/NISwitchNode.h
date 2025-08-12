#pragma once

#include "NINode.h"

namespace NI {
	struct SwitchNode : Node {
		int switchIndex; // 0xB0
		float fSavedTime;
		int updateIndex;
		TArray<unsigned int*> childRevisionIDs;
		bool bUpdateControllers;

		//
		// Custom functions.
		//

		int getSwitchIndex();
		void setSwitchIndex(int index);
		Pointer<AVObject> getActiveChild() const;
		int getChildIndexByName(const char*) const;

	};
	static_assert(sizeof(SwitchNode) == 0xD8, "NI::SwitchNode failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::SwitchNode)
