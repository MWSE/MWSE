#pragma once

#include "NIAVObject.h"

namespace NI {
	struct DynamicEffect : AVObject {
#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		// MWSE-source-of-truth: enum Type for dispatch.
		enum Type : int {
			TYPE_AMBIENT_LIGHT,
			TYPE_DIRECTIONAL_LIGHT,
			TYPE_POINT_LIGHT,
			TYPE_SPOT_LIGHT,
			TYPE_TEXTURE_EFFECT,
		};
#endif

		bool enabled; // 0x90
#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		// MWSE-original field names + types.
		unsigned int index; // 0x94
		unsigned int pushCount; // 0x98
#else
		int index; // 0x94
		int unknown_0x98;
#endif
		unsigned int revisionId; // 0x9C
		NodeLinkedList affectedNodes; // 0xA0

		DynamicEffect();
		~DynamicEffect();

		//
		// vTable wrappers.
		//

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		// MWSE-original: const-qualified, matches MWSE-private NIDynamicEffect.cpp.
		int getType() const;
#else
		// SharedSE/CSSE: non-const, matches SharedSE/NIDynamicEffect.cpp.
		int getType();
#endif

		//
		// Other related this-call functions.
		//

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		// MWSE-only: light-effect predicate. Impl in MWSE-private NIDynamicEffect.cpp.
		bool isLight() const;
#endif

		void attachAffectedNode(Node* node);
		void detachAffectedNode(Node* node);
		void detachAllAffectedNodes();
	};
	static_assert(sizeof(DynamicEffect) == 0xA8, "NI::DynamicEffect failed size validation");

	struct DynamicEffect_vTable : AVObject_vTable {
#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		int(__thiscall* getType)(const DynamicEffect*); // 0x94
#else
		int(__thiscall* getType)(DynamicEffect*); // 0x94
#endif
	};
	static_assert(sizeof(DynamicEffect_vTable) == 0x98, "NI::DynamicEffect's vtable failed size validation");

	void __cdecl ClearDynamicEffectNodes(DynamicEffect* effect);
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::DynamicEffect)
#endif
