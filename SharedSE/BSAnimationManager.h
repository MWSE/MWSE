#pragma once

#include "NINode.h"

#include "NIBSAnimationNode.h"

namespace NI {
	struct BSAnimationManager : Node {
		TArray<Pointer<BSAnimationNode>> managedNodes; // 0xB0
	};
	static_assert(sizeof(BSAnimationManager) == 0xC8, "NI::BSAnimationManager failed size validation");
}
