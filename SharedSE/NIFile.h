#pragma once

namespace NI {
	struct File {
		void* vtbl; // 0x0
		void* buffer; //0x4
		size_t bufferAllocSize; // 0x8
		size_t bufferReadSize; // 0xC
		size_t position; // 0x10
		void* filePointer; // 0x14
		int accessMode; // 0x18
		bool valid; // 0x1C
	};
	static_assert(sizeof(File) == 0x20, "NI::File failed size validation");
}
