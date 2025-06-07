#pragma once

#include "NIAVObject.h"
#include "NIDefines.h"

namespace NI {
	struct Node : AVObject {
		TArray<Pointer<AVObject>> children; // 0x90
		DynamicEffectLinkedList effectList; // 0xA8

		Node();
		~Node();

		//
		// vTable wrappers.
		//

		void attachChild(AVObject * child, bool useFirstAvailable = false);
		Pointer<AVObject> detachChild(AVObject * child);
		Pointer<AVObject> detachChildAt(unsigned int index);
		Pointer<AVObject> setChildAt(unsigned int index, AVObject * child);

		//
		// Custom functions.
		//

		static Pointer<Node> create();

		void detachAllChildren();

		void attachEffect(DynamicEffect* effect);
		void detachEffect(DynamicEffect* effect);
		void detachAllEffects();
		Pointer<DynamicEffect> getEffect(int type);
		std::vector<Pointer<DynamicEffect>> getEffects(int type);

		void attachChild_lua(AVObject* child, sol::optional<bool> useFirstAvailable);
		Pointer<AVObject> detachChildAt_lua(size_t index);

		bool isAffectedBy(const DynamicEffect* effect) const;
		size_t getLightCount() const;

		static constexpr auto LIGHT_LIMIT = 8;

		bool shouldBeAffectedByLight(const NI::PointLight* light) const;
		void updatePointLight(PointLight* light, bool isLand);
		void sortDynamicEffects(bool isLand);
	};
	static_assert(sizeof(Node) == 0xB0, "NI::Node failed size validation");

	struct Node_vTable : AVObject_vTable {
		void (__thiscall * attachChild)(Node *, AVObject *, bool); // 0x94
		void (__thiscall * detachChild)(Node *, Pointer<AVObject>*, AVObject *); // 0x98
		void (__thiscall * detachChildAt)(Node *, Pointer<AVObject>*, unsigned int); // 0x9C
		void (__thiscall * setChildAt)(Node *, Pointer<AVObject>*, unsigned int, AVObject *); // 0xA0
		void (__thiscall * updateUpwardPass)(Node *); // 0xA4
	};
	static_assert(sizeof(Node_vTable) == 0xA8, "NI::Node's vtable failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::Node)
