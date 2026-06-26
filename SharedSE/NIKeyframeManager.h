#pragma once

#include "NITimeController.h"
#include "NIKeyframeController.h"
#include "NIExtraData.h"
#include "NIHashMap.h"
#include "NIMatrix33.h"
#include "NIPoint3.h"

namespace NI {
	enum struct SequenceState : int {
		Inactive,
		Animating,
		LayerBlend,
		SyncSeqBlend,
		BlendSource,
		BlendDest,
		MorphSource,
		MorphDest,
		SumSource,
		SumDest
	};

	struct Sequence {
		char* name; // 0x0
		char* filename; // 0x4
		int fileNum;
		NI::TArray<char*> objectNames; // 0xC
		NI::TArray<Pointer<KeyframeController>> controllers; // 0x24
		Pointer<TextKeyExtraData> textKeys;
		unsigned int textKeyControllerIndex; // 0x40
		KeyframeManager* manager; // 0x44
		SequenceState state;
		float endPointTime;
		Sequence* partnerSequence;
		float offset;
		char bCumulative;
		char bFirstFrame;
		float lastTime;
		NI::Matrix33 m_kLoopScaleRotation;
		NI::Point3 m_kLoopTranslation;
		NI::Matrix33 m_kTempScaleRotation;
		NI::Point3 m_kTempTranslation;

		Pointer<KeyframeController> getController(const char* name) const;
		void release();
	};
	static_assert(sizeof(Sequence) == 0xC0, "NI::Sequence failed size validation");

	struct KeyframeManager : TimeController {
		NI::HashMap<const char*, Sequence*> sequences; // 0x34
		bool cumulative; // 0x44
		NI::Matrix33 globalScaleRotation; // 0x48
		NI::Point3 globalTranslation; // 0x6C

		void addSequence(Sequence* seq);
		void removeSequence(Sequence* seq);
		void removeAll();
		bool activateSequence(Sequence* seq);
		bool deactivateSequence(Sequence* seq);
		void deactivateAll();
	};
	static_assert(sizeof(KeyframeManager) == 0x78, "NI::KeyframeManager failed size validation");
}
