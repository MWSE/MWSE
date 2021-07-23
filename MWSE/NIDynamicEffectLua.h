#pragma once

#include "NIObjectLua.h"

#include "NIDynamicEffect.h"
#include "NILinkedList.h"

namespace mwse {
	namespace lua {
		template <typename T>
		void setUserdataForNIDynamicEffect(sol::usertype<T>& usertypeDefinition) {
			setUserdataForNIAVObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["affectedNodes"] = &NI::DynamicEffect::affectedNodes;
			usertypeDefinition["enabled"] = &NI::DynamicEffect::enabled;
			usertypeDefinition["type"] = sol::readonly_property(&NI::DynamicEffect::getType);

			// Basic function binding.
			usertypeDefinition["attachAffectedNode"] = &NI::DynamicEffect::attachAffectedNode;
			usertypeDefinition["detachAffectedNode"] = &NI::DynamicEffect::detachAffectedNode;
		}

		void bindNIDynamicEffect();
	}
}
