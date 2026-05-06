#include "NISwitchNode.h"

#include <cstring>
#include <exception>

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
			const auto child = children[i].get();
			if (child != nullptr && _stricmp(name, child->name) == 0) {
				return i;
			}
		}
		return -1;
	}
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::SwitchNode)
#endif\
