#include "NINodeLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIBillboardNode.h"
#include "NIDefines.h"
#include "NINode.h"
#include "NISortAdjustNode.h"
#include "NIUtil.h"

namespace mwse::lua {
	std::function<NI::Pointer<NI::AVObject>()> traverse(NI::Node* self, sol::optional<sol::table> param) {
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
		std::function<void(NI::AVObject*)> traverseChild = [&](NI::AVObject* object) {
			if (!object->isInstanceOfType(NI::RTTIStaticPtr::NiNode)) {
				return;
			}

			const auto asNode = static_cast<const NI::Node*>(object);
			for (auto& nodeChild : asNode->children) {
				if (!nodeChild) {
					continue;
				}
				if (NI::passesTraverseFilters(nodeChild, filters, prefix)) {
					queue.push(nodeChild);
				}

				if (recursive) {
					traverseChild(nodeChild);
				}
			}
		};

		if (NI::passesTraverseFilters(self, filters, prefix)) {
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

	void bindNINode() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Binding for NI::Node.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::Node>("niNode");
			usertypeDefinition["new"] = &NI::Node::create;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::AVObject, NI::ObjectNET, NI::Object>();
			setUserdataForNINode(usertypeDefinition);
		}

		// Binding for NI::BillboardNode.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::BillboardNode>("niBillboardNode");
			usertypeDefinition["new"] = &NI::BillboardNode::create;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::Node, NI::AVObject, NI::ObjectNET, NI::Object>();
			setUserdataForNINode(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["mode"] = sol::property(&NI::BillboardNode::getMode, &NI::BillboardNode::setMode);

			// Basic function binding.
			usertypeDefinition["rotateToCamera"] = &NI::BillboardNode::rotateToCamera;
		}

		// Binding for NI::SortAdjustNode.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::SortAdjustNode>("niSortAdjustNode");
			usertypeDefinition["new"] = &NI::SortAdjustNode::create;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::Node, NI::AVObject, NI::ObjectNET, NI::Object>();
			setUserdataForNINode(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["accumulator"] = &NI::SortAdjustNode::accumulator;
			usertypeDefinition["sortingMode"] = &NI::SortAdjustNode::sortingMode;
		}
	}
}
