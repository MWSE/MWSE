#pragma once

#include "TES3Defines.h"

#include "TES3Object.h"

namespace TES3 {
	namespace BodyPartFlag {
		enum Flag : unsigned int {
			Female = 0x1,
			NotPlayable = 0x2, // Note: Flag is presented as Playable in the CS, but the bit is set when not playable.
		};

	}

	struct BodyPart : PhysicalObject {
		char * fileName; // 0x30
		char * raceName; // 0x34
		signed char part; // 0x38
		bool vampiric; // 0x39
		unsigned char flags; // 0x3A
		signed char partType; // 0x3B
		NI::Pointer<NI::Node> mirroredNode; // 0x3C // Type: BSMirroredNode.

		static constexpr auto OBJECT_TYPE = ObjectType::Bodypart;

		BodyPart() = delete;
		~BodyPart() = delete;

		//
		// Custom functions.
		//

		bool getFlag(BodyPartFlag::Flag flag);
		void setFlag(BodyPartFlag::Flag flag, bool set);

		bool getIsFemale();
		void setIsFemale(bool value);

		bool getIsPlayable();
		void setIsPlayable(bool value);

	};
	static_assert(sizeof(BodyPart) == 0x40, "TES3::BodyPart failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::BodyPart)
