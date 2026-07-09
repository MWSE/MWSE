module;

#include "NIDefines.h"

#include "NIAccumulator.h"
#include "NINode.h"

export module NISortAdjustNode;

namespace NI {
	export enum struct SortAdjustMode : int {
		SORTING_INHERIT = 0,
		SORTING_OFF = 1,
		SORTING_SUBSORT = 2,
		SORTING_ORDERED_SUBTREE_MWSE = 64,
	};

	export struct SortAdjustNode : Node {
		SortAdjustMode sortingMode; // 0xB0
		Pointer<Accumulator> accumulator; // 0xB4

		SortAdjustNode();
		static Pointer<SortAdjustNode> create();
	};
	static_assert(sizeof(SortAdjustNode) == 0xB8, "NI::SortAdjustNode failed size validation");
}