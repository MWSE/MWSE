#pragma once

#include "NIObjectLua.h"
#include "NIDynamicEffect.h"

namespace mwse {
	namespace lua {
		template <typename T>
		void setUserdataForNINode(sol::usertype<T>& usertypeDefinition) {
			setUserdataForNIAVObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["children"] = sol::readonly_property(&NI::Node::children);
			usertypeDefinition["effectList"] = sol::readonly_property(&NI::Node::effectList);

			// Basic function binding.
			usertypeDefinition["attachChild"] = &NI::Node::attachChild_lua;
			usertypeDefinition["createBoundingBox"] = &NI::Node::createBoundingBox_lua;
			usertypeDefinition["detachChild"] = &NI::Node::detachChildHandled;
			usertypeDefinition["detachChildAt"] = &NI::Node::detachChildAt_lua;
			usertypeDefinition["attachEffect"] = &NI::Node::attachEffect;
			usertypeDefinition["detachEffect"] = &NI::Node::detachEffect;
			usertypeDefinition["getEffect"] = &NI::Node::getEffect;
		}

		void bindNINode();
	}
}
