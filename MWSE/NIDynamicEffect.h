#pragma once

#include "NIAVObject.h"

namespace NI {


	struct DynamicEffect : AVObject {
		enum Type : int {
			TYPE_AMBIENT_LIGHT,
			TYPE_DIRECTIONAL_LIGHT,
			TYPE_POINT_LIGHT,
			TYPE_SPOT_LIGHT,
			TYPE_TEXTURE_EFFECT,
		};

		bool enabled; // 0x90
		unsigned int index; // 0x94
		unsigned int pushCount; // 0x98
		unsigned int revisionId; // 0x9C
		NodeLinkedList affectedNodes; // 0xA0

		DynamicEffect();
		~DynamicEffect();

		//
		// vTable wrappers.
		//

		int getType() const;

		//
		// Other related this-call functions.
		//

		bool isLight() const;

		void attachAffectedNode(Node* node);
		void detachAffectedNode(Node* node);

	};
	static_assert(sizeof(DynamicEffect) == 0xA8, "NI::DynamicEffect failed size validation");


	struct DynamicEffect_vTable : AVObject_vTable {
		int (__thiscall * getType)(const DynamicEffect*); // 0x94
	};
	static_assert(sizeof(DynamicEffect_vTable) == 0x98, "NI::DynamicEffect's vtable failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::DynamicEffect)
