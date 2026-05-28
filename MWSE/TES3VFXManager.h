#pragma once

#include "TES3Defines.h"
#include "NIDefines.h"

#include "Deque.h"

#include "NINode.h"

namespace TES3 {
	using se::Deque;

	struct VFX {
		Reference* target; // 0x0
		NI::Point3 position; // 0x4
		NI::AVObject* createdFromNode; // 0x10
		float maxAge; // 0x14
		float age; // 0x18
		NI::Pointer<NI::Node> clonedEffectNode; // 0x1C
		bool expired; // 0x20
		float beginKeyTime; // 0x24
		float endKeyTime; // 0x28
		float keyTime; // 0x2C
		float verticalOffset; // 0x30
		unsigned int sourceInstanceSerial; // 0x34
		PhysicalObject* effectObject; // 0x38
		int vfxId; // 0x3C

		static constexpr auto AGE_INFINITE = 0.0f;

		VFX() = delete;
		~VFX() = delete;

		MagicSourceInstance* getSourceInstance() const;
	};
	static_assert(sizeof(VFX) == 0x40, "TES3::VFX failed size validation");

	struct VFXSerialized {
		float maxAge; // 0x0
		float age; // 0x4
		bool expired; // 0x8
		float beginKeyTime; // 0xC
		float endKeyTime; // 0x10
		float keyTime; // 0x14
		float scale; // 0x18
		NI::Point3 position; // 0x1C
		unsigned int sourceInstanceSerial; // 0x28
		int vfxId; // 0x2C
		char effectObjectId[32]; // 0x30
		unsigned int targetSourceId; // 0x50
		char targetActorId[32]; // 0x54
		char targetCellId[32]; // 0x74
		float verticalOffset; // 0x94
	};
	static_assert(sizeof(VFXSerialized) == 0x98, "TES3::VFXSerialized failed size validation");

	struct VFXManager {
		se::Deque<VFX*> vfxNodes; // 0x0
		NI::Pointer<NI::Node> worldVFXRoot; // 0x8
		NI::Pointer<NI::AVObject> unknown_0xC;

		VFXManager() = delete;
		~VFXManager() = delete;

		VFX* createForMagicEffect(int magicEffectID, Reference* reference, float lifespan);
		VFX* createForSource(MagicSourceInstance* source, Reference* reference, float lifespan);
		VFX* createForEffect(unsigned int serial, PhysicalObject* effect, int animLoopCount, float lifespan, float scale, float verticalOffset);

		VFX* createForReference(unsigned int serial, PhysicalObject* effect, Reference* reference, int animLoopCount, float lifespan, float scale, float verticalOffset);
		VFX* createAtPosition(unsigned int serial, PhysicalObject* effect, NI::Point3* position, int animLoopCount, float lifespan, float scale, float verticalOffset);
		VFX* createForAVObject(unsigned int serial, PhysicalObject* effect, NI::AVObject* avObject, int animLoopCount, float lifespan, float scale, float verticalOffset);

		void remove(VFX* vfx);
		void removeForReference(Reference* reference);
		void removeFromAVObject(NI::AVObject* avObject);
		void removeForSerialForReference(unsigned int serial, Reference* reference);

		void reset();
		void resetOnNewScene();
	};
	static_assert(sizeof(VFXManager) == 0x14, "TES3::VFXManager failed size validation");
}
