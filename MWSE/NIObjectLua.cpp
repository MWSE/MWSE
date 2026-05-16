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

#include "StringUtil.h"


namespace {
	bool passesTraverseFilters(const NI::AVObject* object, const std::unordered_set<unsigned int>& typeFilters, std::string_view prefix) {
		bool passesFilter = typeFilters.empty() ? true : false;
		if (!passesFilter) {
			for (const auto type : typeFilters) {
				if (object->isInstanceOfType((uintptr_t)type)) {
					passesFilter = true;
					break;
				}
			}
		}
		bool passesPrefix = prefix.empty() ? true : object->name && se::string::starts_with(object->name, prefix);
		return passesFilter && passesPrefix;
	}
}

namespace mwse::lua {
	std::function<NI::Pointer<NI::AVObject>()> traverse(NI::AVObject* self, sol::optional<sol::table> param) {
		bool recursive = getOptionalParam(param, "recursive", true);
		std::string prefix = getOptionalParam(param, "prefix", std::string(""));
		std::unordered_set<unsigned int> filters;

		if (param) {
			sol::table paramTable = param.value().as<sol::table>();
			sol::object maybeValue = paramTable["type"];
			if (maybeValue.valid()) {
				if (maybeValue.is<unsigned int>()) {
					filters.insert(maybeValue.as<unsigned int>());
				}
				else if (maybeValue.is<sol::table>()) {
					sol::table filterTable = maybeValue.as<sol::table>();
					for (auto [_, value] : filterTable) {
						filters.insert(value.as<unsigned int>());
					}
				}
				else {
					throw std::invalid_argument("Iteration can only be filtered by a NI object type, or a table of object types.");
				}
			}
		}


		std::queue<NI::Pointer<NI::AVObject>> queue;
		std::function<void(const NI::AVObject*)> traverseChild = [&](const NI::AVObject* object) {
			if (!object->isInstanceOfType(NI::RTTIStaticPtr::NiNode)) {
				return;
			}

			const auto asNode = static_cast<const NI::Node*>(object);
			for (const auto& nodeChild : asNode->children) {
				if (passesTraverseFilters(nodeChild, filters, prefix)) {
					queue.push(nodeChild);
				}
				if (recursive) {
					traverseChild(nodeChild);
				}
			}
		};

		if (passesTraverseFilters(self, filters, prefix)) {
			queue.push(self);
		}
		traverseChild(self);

		return [queue]() mutable -> NI::Pointer<NI::AVObject> {
			if (queue.empty()) {
				return nullptr;
			}
			auto ret = queue.front();
			queue.pop();
			return ret;
		};
	}

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
