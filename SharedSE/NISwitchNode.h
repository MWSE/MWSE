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
		std::vector<const NI::Pointer<NI::AVObject>> getActiveChildren() const;

	};
	static_assert(sizeof(SwitchNode) == 0xD8, "NI::SwitchNode failed size validation");
}