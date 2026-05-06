#include "NISortAdjustNode.h"

#include "ExceptionUtil.h"

namespace NI {
	SortAdjustNode::SortAdjustNode() : Node() {
#if defined(SE_NI_SORTADJUSTNODE_VTBL) && SE_NI_SORTADJUSTNODE_VTBL > 0
		vTable.asNode = reinterpret_cast<Node_vTable*>(SE_NI_SORTADJUSTNODE_VTBL);
		sortingMode = SortAdjustMode::SORTING_INHERIT;
#else
		throw not_implemented_exception();
#endif
	}

	Pointer<SortAdjustNode> SortAdjustNode::create() {
		return new SortAdjustNode();
	}
}
