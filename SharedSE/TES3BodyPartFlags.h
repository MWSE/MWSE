#pragma once

namespace TES3 {
	namespace BodyPartFlag {
		enum Flag : unsigned int {
			Female = 0x1,
			NotPlayable = 0x2, // Note: Flag is presented as Playable in the CS, but the bit is set when not playable.
		};

	}
}
