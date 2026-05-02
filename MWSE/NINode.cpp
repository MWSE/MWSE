#include "NINode.h"

#include "NIBound.h"
#include "NIPointLight.h"

#include "TES3Reference.h"

#include "MathUtil.h"

namespace NI {
	const auto NI_Node_ctor = reinterpret_cast<void(__thiscall*)(const Node*)>(0x6C81E0);

	Node::Node() {
		NI_Node_ctor(this);
	}

	Node::~Node() {
		// Call dtor without deletion.
		vTable.asObject->destructor(this, 0);
	}

	void Node::attachChild(AVObject * child, bool useFirstAvailable) {
		vTable.asNode->attachChild(this, child, useFirstAvailable);
	}

	Pointer<AVObject> Node::detachChild(AVObject * child) {
		Pointer<AVObject> result;
		vTable.asNode->detachChild(this, &result, child);
		return result;
	}

	Pointer<AVObject> Node::detachChildAt(unsigned int index) {
		Pointer<AVObject> result;
		vTable.asNode->detachChildAt(this, &result, index);
		return result;
	}

	Pointer<AVObject> Node::setChildAt(unsigned int index, AVObject* child) {
		Pointer<AVObject> result;
		vTable.asNode->setChildAt(this, &result, index, child);
		return result;
	}

	Pointer<Node> Node::create() {
		return new Node();
	}

	void Node::detachAllChildren() {
		for (auto i = 0u; i < children.size(); ++i) {
			detachChildAt(i);
		}
	}

	const auto NI_Node_AttachEffect = reinterpret_cast<void(__thiscall*)(Node*, DynamicEffect*)>(0x6C91E0);
	void Node::attachEffect(DynamicEffect * effect ) {
		NI_Node_AttachEffect(this, effect);
	}

	const auto NI_Node_DetachEffect = reinterpret_cast<void(__thiscall*)(Node*, DynamicEffect*)>(0x6C9220);
	void Node::detachEffect(DynamicEffect* effect) {
		NI_Node_DetachEffect(this, effect);
	}

	const auto NI_Node_DetachAllEffects = reinterpret_cast<void(__thiscall*)(Node*)>(0x6C92C0);
	void Node::detachAllEffects() {
		NI_Node_DetachAllEffects(this);
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

	std::vector<Pointer<DynamicEffect>> Node::getEffects(int type) {
		std::vector<Pointer<DynamicEffect>> result = {};
		for (auto node = &effectList; node && node->data; node = node->next) {
			if (node->data->getType() == type) {
				result.push_back(node->data);
			}
		}
		return result;
	}

	void Node::attachChild_lua(AVObject* child, sol::optional<bool> useFirstAvailable) {
		attachChild(child, useFirstAvailable.value_or(false));
		updateProperties();
	}

	Pointer<AVObject> Node::detachChildAt_lua(size_t index) {
		if (index == 0) {
			throw std::invalid_argument("This function is 1-indexed. Cannot accept a param less than 1.");
		}
		return detachChildAt(index - 1);
	}

	bool Node::isAffectedBy(const DynamicEffect* effect) const {
		for (auto node = &effectList; node != nullptr; node = node->next) {
			if (node->data == effect) {
				return true;
			}
		}
		return false;
	}

	size_t Node::getLightCount() const {
		size_t count = 0;
		for (auto node = &effectList; node && node->data; node = node->next) {
			const auto effect = node->data;
			if (effect->isLight() && !effect->isAppCulled()) {
				count++;
			}
		}
		return count;
	}

	const auto NI_LightRadiusTest = reinterpret_cast<bool(__cdecl*)(const Node*, const TES3::Vector3*, float*)>(0x4D19C0);
	bool Node::shouldBeAffectedByLight(const PointLight* light) const {
		auto lightRadius = light->specular.r;
		const auto distance = light->worldTransform.translation.distance(&worldBoundOrigin);
		if (distance - worldBoundRadius > lightRadius) {
			return false;
		}

		return NI_LightRadiusTest(this, &light->worldTransform.translation, &lightRadius);
	}

	void Node::updatePointLight(PointLight* light, bool isLand) {
		const auto shouldAffect = shouldBeAffectedByLight(light);
		const auto isAffected = isAffectedBy(light);

		// If nothing is changing, we just need to re-sort.
		if (shouldAffect == isAffected) {
			if (shouldAffect) {
				sortDynamicEffects(isLand);
			}
			return;
		}

		// Removing is also simple.
		if (!shouldAffect) {
			detachEffect(light);
			updateEffects();
			return;
		}

		// Adding just requires a sort afterwards.
		attachEffect(light);
		updateEffects();
		sortDynamicEffects(isLand);
	}

	// Cut down on memory allocation by reusing a buffer.
	static std::vector<DynamicEffect*> dynamicEffectsBuffer;

	void Node::sortDynamicEffects(bool isLand) {
		// Skip if we don't have too many lights.
		if (getLightCount() <= LIGHT_LIMIT) {
			return;
		}

		// Store the effects in a temporary array.
		dynamicEffectsBuffer.clear();
		for (auto node = &effectList; node && node->data; node = node->next) {
			dynamicEffectsBuffer.push_back(node->data);
		}

		// Land records get sorted to favor more influential lights.
		if (isLand) {
			std::sort(dynamicEffectsBuffer.begin(), dynamicEffectsBuffer.end(),
				[&](const DynamicEffect* a, const DynamicEffect* b) -> bool {
					// Lights can be sorted by their type index.
					const auto aType = a->getType();
					const auto bType = b->getType();
					if (aType != bType) {
						return aType < bType;
					}

					// From here on we only care about point lights.
					if (aType != DynamicEffect::TYPE_POINT_LIGHT) {
						return false;
					}

					const auto aLight = static_cast<const PointLight*>(a);
					const auto bLight = static_cast<const PointLight*>(b);
					return aLight->getSortWeight() > bLight->getSortWeight();
				});
		}
		// Everything else gets sorted by distance.
		else {
			std::sort(dynamicEffectsBuffer.begin(), dynamicEffectsBuffer.end(),
				[&](const DynamicEffect* a, const DynamicEffect* b) -> bool {
					// Lights can be sorted by their type index.
					const auto aType = a->getType();
					const auto bType = b->getType();
					if (aType != bType) {
						return aType < bType;
					}

					// From here on we only care about point lights.
					if (aType != DynamicEffect::TYPE_POINT_LIGHT) {
						return false;
					}

					const auto aLight = static_cast<const PointLight*>(a);
					const auto bLight = static_cast<const PointLight*>(b);
					const auto aDistance = aLight->worldTransform.translation.distance(&getWorldBound()->center);
					const auto aRadius = aLight->getRadius();
					const auto bDistance = bLight->worldTransform.translation.distance(&getWorldBound()->center);
					const auto bRadius = bLight->getRadius();
					return aRadius * aDistance > bRadius * bDistance;
				});
		}

		// Rebuild the linked list.
		size_t refilledEffects = 0;
		for (auto node = &effectList; node != nullptr; node = node->next) {
			node->data = dynamicEffectsBuffer[refilledEffects];
			refilledEffects++;
		}

		updateEffects();
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::Node)
