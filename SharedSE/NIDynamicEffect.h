#pragma once

#include "NIAVObject.h"

namespace NI {
	struct DynamicEffect : AVObject {
		// Effect-type tag for getType() dispatch. Pure constants; both targets
		// can use them.
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

		// Light-effect predicate (TYPE_*_LIGHT). Implementation lives in
		// MWSE-private NIDynamicEffect.cpp; CSSE doesn't currently call it.
		bool isLight() const;

		void attachAffectedNode(Node* node);
		void detachAffectedNode(Node* node);
		void detachAllAffectedNodes();
	};
	static_assert(sizeof(DynamicEffect) == 0xA8, "NI::DynamicEffect failed size validation");

	struct DynamicEffect_vTable : AVObject_vTable {
		int(__thiscall* getType)(const DynamicEffect*); // 0x94
	};
	static_assert(sizeof(DynamicEffect_vTable) == 0x98, "NI::DynamicEffect's vtable failed size validation");

	void __cdecl ClearDynamicEffectNodes(DynamicEffect* effect);
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::DynamicEffect)
#endif
