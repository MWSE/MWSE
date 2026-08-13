#pragma once

#include "TES3BaseObject.h"

namespace se::cs {
	struct Sound : BaseObject {
		char field_10;
		char id[32]; // 0x11
		char filename[32]; // 0x31
		unsigned char volume; // 0x51
		unsigned char minDistance; // 0x52
		unsigned char maxDistance; // 0x53
		void* soundBuffer; // 0x54

		inline const char* getObjectID() const {
			return id;
		}
	};
	static_assert(sizeof(Sound) == 0x58, "Sound failed size validation");
}
