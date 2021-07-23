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
		void detachChild(AVObject ** out_detached, AVObject * child);
		void detachChildAt(AVObject ** out_detached, unsigned int index);
		void setChildAt(AVObject ** out_detached, unsigned int index, AVObject * child);

		//
		// Custom functions.
		//

		static Pointer<Node> create();

		Pointer<AVObject> detachChildHandled(AVObject* child);
		Pointer<AVObject> detachChildAtHandled(size_t index);

		void attachEffect(DynamicEffect* effect);
		void detachEffect(DynamicEffect* effect);
		Pointer<DynamicEffect> getEffect(int type);

		void attachChild_lua(AVObject* child, sol::optional<bool> useFirstAvailable);
		Pointer<AVObject> detachChildAt_lua(size_t index);

	};
	static_assert(sizeof(Node) == 0xB0, "NI::Node failed size validation");

	struct Node_vTable : AVObject_vTable {
		void (__thiscall * attachChild)(Node *, AVObject *, bool); // 0x94
		void (__thiscall * detachChild)(Node *, AVObject **, AVObject *); // 0x98
		void (__thiscall * detachChildAt)(Node *, AVObject **, unsigned int); // 0x9C
		void (__thiscall * setChildAt)(Node *, AVObject **, unsigned int, AVObject *); // 0xA0
		void (__thiscall * updateUpwardPass)(Node *); // 0xA4
	};
	static_assert(sizeof(Node_vTable) == 0xA8, "NI::Node's vtable failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::Node)
