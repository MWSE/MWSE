#include "NISwitchNode.h"

#include "StringUtil.h"

namespace NI {
	int SwitchNode::getSwitchIndex() const {
		return switchIndex;
	}

	void SwitchNode::setSwitchIndex(int index) {
		// Index -1 indicates no child active.
		if (index < -1 || index > int(children.getFilledCount() - 1) || children.at(index) == nullptr) {
			throw std::exception("Attempted to set switchIndex beyond bounds!");
		}
		switchIndex = index;
	}

	Pointer<AVObject> SwitchNode::getActiveChild() const {
		if (switchIndex >= 0) {
			return children.at(switchIndex);
		}
		return nullptr;
	}

	int SwitchNode::getChildIndexByName(const char* name) const {
		for (auto i = 0u; i < children.size(); ++i) {
			if (mwse::string::equal(name, children[i].get()->name)) {
				return i;
			}
		}
		return -1;
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::SwitchNode)
