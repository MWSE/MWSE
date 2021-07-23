#pragma once

#include "NIAVObject.h"

namespace NI {
	struct DynamicEffect : AVObject {
		bool enabled; // 0x90
		int index; // 0x94
		int unknown_0x98;
		unsigned int revisionId; // 0x9C
		NodeLinkedList affectedNodes; // 0xA0

		DynamicEffect();
		~DynamicEffect();

		//
		// vTable wrappers.
		//

		int getType();

		//
		// Other related this-call functions.
		//

		void attachAffectedNode(Node* node);
		void detachAffectedNode(Node* node);

	};
	static_assert(sizeof(DynamicEffect) == 0xA8, "NI::DynamicEffect failed size validation");


	struct DynamicEffect_vTable : AVObject_vTable {
		int (__thiscall * getType)(DynamicEffect*); // 0x94
	};
	static_assert(sizeof(DynamicEffect_vTable) == 0x98, "NI::DynamicEffect's vtable failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::DynamicEffect)
