#include "NINodeLua.h"

#include "NIObjectLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NINode.h"
#include "NIRTTI.h"

namespace mwse {
	namespace lua {
		void bindNINode() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Binding for NI::Node.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<NI::Node>("niNode");
				usertypeDefinition["new"] = &NI::Node::create;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::AVObject, NI::ObjectNET, NI::Object>();
				setUserdataForNINode(usertypeDefinition);
			}
		}
	}
}
