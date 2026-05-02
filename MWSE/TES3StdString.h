#pragma once

namespace TES3 {
	struct StdString {
		int unknown_0x0;
		char * c_str; // 0x4
		int length; // 0x8
		int storageSize; // 0xC

		//
		// Related this-call functions.
		//

		StdString();
		StdString(const char* c_str);
		~StdString();

		void operator=(const char* c_str);

	};
}
