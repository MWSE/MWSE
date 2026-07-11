#pragma once

#include "CSPhysicalObject.h"

#include "NINode.h"

namespace se::cs {
	namespace BodyPartFlag {
		enum Flag : unsigned int {
			Female = 0x1,
			Playable = 0x2
		};
	}

	struct BodyPart : PhysicalObject {
		char* model; // 0x48
		char* fileName; // 0x4C
		byte part; // 0x50
		bool vampiric; // 0x51
		byte flags; // 0x52
		byte partType; // 0x53
		NI::Pointer<NI::Node> mirrorNode; // 0x54

		bool getIsFemale() const;
		bool getIsPlayable() const;

		const char* getPartName() const;
		const char* getTypeName() const;

		// Virtual table overwrite functions.
		const char* getRaceName() const;
	};
	static_assert(sizeof(BodyPart) == 0x58, "BodyPart failed size validation");
}
