#include "NINode.h"

#include "NIDynamicEffect.h"

#include "ExceptionUtil.h"

namespace NI {

	Node::Node() {
#if defined(SE_NI_NODE_FNADDR_CTOR) && SE_NI_NODE_FNADDR_CTOR > 0
		const auto NI_Node_ctor = reinterpret_cast<void(__thiscall*)(const Node*)>(SE_NI_NODE_FNADDR_CTOR);
		NI_Node_ctor(this);
#else
		throw not_implemented_exception();
#endif
	}

	Node::~Node() {
		// Call dtor without deletion.
		vTable.asObject->destructor(this, 0);
	}

	void Node::attachChild(AVObject* child, bool useFirstAvailable) {
		vTable.asNode->attachChild(this, child, useFirstAvailable);
	}

	Pointer<AVObject> Node::detachChild(AVObject* child) {
		Pointer<AVObject> result;
		vTable.asNode->detachChild(this, &result, child);
		return result;
	}

	Pointer<AVObject> Node::detachChildAt(unsigned int index) {
		Pointer<AVObject> result;
		vTable.asNode->detachChildAt(this, &result, index);
		return result;
	}

	std::vector<const NI::Pointer<NI::AVObject>> Node::getActiveChildren() const {
		std::vector<const NI::Pointer<NI::AVObject>> result;
		result.reserve(children.getFilledCount());
		for (const auto& child : children) {
			if (!child) {
				continue;
			}
			if (child->getAppCulled()) {
				continue;
			}
			result.emplace_back(child);
		}
		return result;
	}

	Pointer<AVObject> Node::setChildAt(unsigned int index, AVObject* child) {
		Pointer<AVObject> displaced;
		vTable.asNode->setChildAt(this, &displaced, index, child);
		return displaced;
	}

	Pointer<Node> Node::create() {
		return new Node();
	}

	void Node::attachEffect(DynamicEffect* effect) {
#if defined(SE_NI_NODE_FNADDR_ATTACHEFFECT) && SE_NI_NODE_FNADDR_ATTACHEFFECT > 0
		const auto NI_Node_AttachEffect = reinterpret_cast<void(__thiscall*)(Node*, DynamicEffect*)>(SE_NI_NODE_FNADDR_ATTACHEFFECT);
		NI_Node_AttachEffect(this, effect);
#else
		throw not_implemented_exception();
#endif
	}

	void Node::detachEffect(DynamicEffect* effect) {
#if defined(SE_NI_NODE_FNADDR_DETACHEFFECT) && SE_NI_NODE_FNADDR_DETACHEFFECT > 0
		const auto NI_Node_DetachEffect = reinterpret_cast<void(__thiscall*)(Node*, DynamicEffect*)>(SE_NI_NODE_FNADDR_DETACHEFFECT);
		NI_Node_DetachEffect(this, effect);
#else
		throw not_implemented_exception();
#endif
	}

	void Node::detachAllEffects() {
#if defined(SE_NI_NODE_FNADDR_DETACHALLEFFECTS) && SE_NI_NODE_FNADDR_DETACHALLEFFECTS > 0
		const auto NI_Node_DetachAllEffects = reinterpret_cast<void(__thiscall*)(Node*)>(SE_NI_NODE_FNADDR_DETACHALLEFFECTS);
		NI_Node_DetachAllEffects(this);
#else
		throw not_implemented_exception();
#endif
	}

	Pointer<DynamicEffect> Node::getEffect(int type) {
		auto effectNode = &effectList;
		while (effectNode) {
			if (effectNode->data && effectNode->data->getType() == type) {
				return effectNode->data;
			}

			effectNode = effectNode->next;
		}

		return nullptr;
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	void Node::attachChild_lua(AVObject* child, sol::optional<bool> useFirstAvailable) {
		attachChild(child, useFirstAvailable.value_or(false));
		updateProperties();
	}

	Pointer<AVObject> Node::detachChildAt_lua(size_t index) {
		if (index == 0) {
			throw std::invalid_argument("This function is 1-indexed. Cannot accept a param less than 1.");
		}
		return detachChildAtHandled(index - 1);
	}
#endif
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::Node)
#endif
