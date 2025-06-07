#pragma once

#include "TES3Defines.h"

#include "TES3Object.h"

namespace TES3 {
	enum class SoundType {
		LeftFoot = 0x0,
		RightFoot = 0x1,
		SwimLeft = 0x2,
		SwimRight = 0x3,
		Moan = 0x4,
		Roar = 0x5,
		Scream = 0x6,
		Land = 0x7,
	};

	struct SoundGenerator : BaseObject {
		char name[32];
		Creature* creature;
		Sound* sound;
		SoundType soundType;

		static constexpr auto OBJECT_TYPE = ObjectType::SoundGenerator;

		SoundGenerator() = delete;
		~SoundGenerator() = delete;

		//
		// Virtual table overrides.
		//

		const char* getObjectID() const;

		//
		// Custom functions.
		//

		std::string toJson() const;

	};
	static_assert(sizeof(SoundGenerator) == 0x3C, "TES3::SoundGenerator failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::SoundGenerator)
