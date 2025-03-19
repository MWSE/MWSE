#include "NISortAdjustNode.h"

namespace NI {
	const auto SortAdjustNode_vTable = reinterpret_cast<Node_vTable*>(0x750580);
	SortAdjustNode::SortAdjustNode() : Node() {
		vTable.asNode = SortAdjustNode_vTable;
		sortingMode = SortAdjustMode::SORTING_INHERIT;
	}

	Pointer<SortAdjustNode> SortAdjustNode::create() {
		return new SortAdjustNode();
	}
}