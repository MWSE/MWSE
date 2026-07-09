module;

#include "NINode.h"

export module NIBSAnimationManager;

import NIBSAnimationNode;

namespace NI {
	export struct BSAnimationManager : Node {
		TArray<Pointer<BSAnimationNode>> managedNodes; // 0xB0

		static bool isExactType(const AVObject* object);

		void growWorldBoundFromChild(AVObject* child);
		void attachChildConservative(AVObject* child, bool useFirstAvailable = false);
		Pointer<AVObject> setChildAtConservative(unsigned int index, AVObject* child);
	};
	static_assert(sizeof(BSAnimationManager) == 0xC8, "NI::BSAnimationManager failed size validation");
}
