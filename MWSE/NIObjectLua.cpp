#include "NIObjectLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIDefines.h"
#include "NIAVObject.h"
#include "NIDynamicEffect.h"
#include "NINode.h"
#include "NIObject.h"
#include "NIObjectNET.h"
#include "NIRTTI.h"

namespace mwse::lua {
	void bindNIObject() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Binding for NI::RTTI.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::RTTI>("niRTTI");
			usertypeDefinition["new"] = sol::no_constructor;
			usertypeDefinition[sol::meta_function::to_string] = &NI::RTTI::toString;

			// Basic property binding.
			usertypeDefinition["name"] = &NI::RTTI::name;
			usertypeDefinition["parent"] = &NI::RTTI::baseRTTI;
		}

		// Binding for NI::Object.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::Object>("niObject");
			usertypeDefinition["new"] = sol::no_constructor;

			// Inherit NI::Object.
			setUserdataForNIObject(usertypeDefinition);
		}

		// Binding for NI::ObjectNET.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::ObjectNET>("niObjectNET");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::Object>();
			setUserdataForNIObjectNET(usertypeDefinition);
		}

		// Binding for NI::AVObject.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::AVObject>("niAVObject");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::ObjectNET, NI::Object>();
			setUserdataForNIAVObject(usertypeDefinition);
		}

		// Binding for NI::DynamicEffectLinkedList.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::DynamicEffectLinkedList>("niDynamicEffectLinkedList");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["data"] = sol::readonly_property(&NI::DynamicEffectLinkedList::data);
			usertypeDefinition["next"] = sol::readonly_property(&NI::DynamicEffectLinkedList::next);
		}

		// Binding for NI::NodeLinkedList.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::NodeLinkedList>("niNodeLinkedList");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["data"] = sol::readonly_property(&NI::NodeLinkedList::data);
			usertypeDefinition["next"] = sol::readonly_property(&NI::NodeLinkedList::next);
		}

		// Binding for NI::PropertyLinkedList.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::PropertyLinkedList>("niPropertyLinkedList");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["data"] = sol::readonly_property(&NI::PropertyLinkedList::data);
			usertypeDefinition["next"] = sol::readonly_property(&NI::PropertyLinkedList::next);
		}
	}
}
