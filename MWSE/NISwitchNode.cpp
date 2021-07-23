#include "NISwitchNode.h"

namespace NI {
	int SwitchNode::getSwitchIndex() {
		return switchIndex;
	}

	void SwitchNode::setSwitchIndex(int index) {
		if (index < 0 || index > int(children.getFilledCount() - 1) || children.at(index) == nullptr) {
			throw std::exception("Attempted to set switchIndex beyond bounds!");
		}
		switchIndex = index;
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::SwitchNode)
