#pragma once

#include "NIObject.h"

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
#include "TES3Defines.h"
#elif defined(SE_IS_CS) && SE_IS_CS == 1
#include "CSDefines.h"
#endif

namespace NI {
	struct ExtraData : Object {
		size_t genericDataLength; // 0x8
		void* genericData; // 0xC // Only loaded if NiExtraData isn't subclassed.
		Pointer<ExtraData> next; // 0x10

		//
		// Custom functions.
		//

		nonstd::span<BYTE> getGenericData();

		ExtraData* getNext() const;
		void setNext(ExtraData* next);

	};
	static_assert(sizeof(ExtraData) == 0x14, "NI::TextKey failed size validation");

	struct StringExtraData : ExtraData {
		char* string; // 0x14

		StringExtraData(const char* str);
		~StringExtraData();

		//
		// Custom functions.
		//

		const char* getString() const;
		void setString(const char* str);

	};
	static_assert(sizeof(StringExtraData) == 0x18, "NI::TextKey failed size validation");

	struct Tes3ExtraData : ExtraData {
#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		using VALUE_TYPE = TES3::Reference;
#elif defined(SE_IS_CS) && SE_IS_CS == 1
		using VALUE_TYPE = se::cs::Reference;
#else
		using VALUE_TYPE = void;
#endif
		VALUE_TYPE* reference; // 0x14
	};
	static_assert(sizeof(Tes3ExtraData) == 0x18, "NI::Tes3ExtraData failed size validation");

	struct TextKey {
		float time; // 0x0
		char* text; // 0x4

		//
		// Custom functions.
		//

		const char* getText() const;
		void setText(const char* text);

	};
	static_assert(sizeof(TextKey) == 0x8, "NI::TextKey failed size validation");

	struct TextKeyExtraData : ExtraData {
		unsigned int keyCount; // 0x14
		TextKey* key; // 0x18

		//
		// Custom functions.
		//

		nonstd::span<TextKey> getKeys() const;

	};
	static_assert(sizeof(TextKeyExtraData) == 0x1C, "NI::TextKeyExtraData failed size validation");
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::ExtraData)
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::StringExtraData)
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::Tes3ExtraData)
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::TextKeyExtraData)
#endif
