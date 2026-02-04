#pragma once

#include "NIObjectLua.h"
#include "NIDynamicEffect.h"

namespace mwse::lua {
	std::function<NI::Pointer<NI::AVObject>()> traverse(NI::Node* self, sol::optional<sol::table> param);

	template <typename T>
	void setUserdataForNINode(sol::usertype<T>& usertypeDefinition) {
		setUserdataForNIAVObject(usertypeDefinition);

		// Basic property binding.
		usertypeDefinition["children"] = sol::readonly_property(&NI::Node::children);
		usertypeDefinition["effectList"] = sol::readonly_property(&NI::Node::effectList);

		// Basic function binding.
		usertypeDefinition["attachChild"] = &NI::Node::attachChild_lua;
		usertypeDefinition["attachEffect"] = &NI::Node::attachEffect;
		usertypeDefinition["createBoundingBox"] = &NI::Node::createBoundingBox_lua;
		usertypeDefinition["detachAllChildren"] = &NI::Node::detachAllChildren;
		usertypeDefinition["detachAllEffects"] = &NI::Node::detachAllEffects;
		usertypeDefinition["detachChild"] = &NI::Node::detachChild;
		usertypeDefinition["detachChildAt"] = &NI::Node::detachChildAt_lua;
		usertypeDefinition["detachEffect"] = &NI::Node::detachEffect;
		usertypeDefinition["getEffect"] = &NI::Node::getEffect;
		usertypeDefinition["traverse"] = traverse;
	}

	void bindNINode();
}
