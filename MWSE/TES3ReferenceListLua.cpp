#include "TES3ReferenceLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Cell.h"
#include "TES3Reference.h"
#include "TES3ReferenceList.h"

namespace mwse {
	namespace lua {
		void bindTES3ReferenceList() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<TES3::ReferenceList>("tes3referenceList");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["size"] = sol::readonly_property(&TES3::ReferenceList::size);

			// Access to other objects that need to be packaged.
			usertypeDefinition["cell"] = sol::readonly_property(&TES3::ReferenceList::cell);
			usertypeDefinition["head"] = sol::readonly_property(&TES3::ReferenceList::head);
			usertypeDefinition["tail"] = sol::readonly_property(&TES3::ReferenceList::tail);
		}
	}
}
