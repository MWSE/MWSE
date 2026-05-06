#include "NIExtraData.h"

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
#include "LuaUtil.h"
#include "TES3Util.h"
#endif

#include "ExceptionUtil.h"

namespace NI {
	nonstd::span<BYTE> ExtraData::getGenericData() {
		if (genericData) {
			return nonstd::span((BYTE*)genericData, genericDataLength);
		}
		return {};
	}

	ExtraData* ExtraData::getNext() const {
		return next;
	}

	void ExtraData::setNext(ExtraData* n) {
		next = n;
	}

	StringExtraData::StringExtraData(const char* str) {
#if defined(SE_NI_STRINGEXTRADATA_FNADDR_CTOR) && SE_NI_STRINGEXTRADATA_FNADDR_CTOR > 0
		const auto NI_StringExtraData_ctor = reinterpret_cast<void(__thiscall*)(NI::StringExtraData*, const char*)>(SE_NI_STRINGEXTRADATA_FNADDR_CTOR);
		NI_StringExtraData_ctor(this, str);
#else
		throw not_implemented_exception();
#endif
	}
	
	StringExtraData::~StringExtraData() {
#if defined(SE_NI_STRINGEXTRADATA_FNADDR_DTOR) && SE_NI_STRINGEXTRADATA_FNADDR_DTOR > 0
		const auto NI_StringExtraData_dtor = reinterpret_cast<void(__thiscall*)(NI::StringExtraData*)>(SE_NI_STRINGEXTRADATA_FNADDR_DTOR);
		NI_StringExtraData_dtor(this);
#else
		throw not_implemented_exception();
#endif
	}

	const char* StringExtraData::getString() const {
		return string;
	}

	void StringExtraData::setString(const char* str) {
#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		mwse::tes3::setDataString(&string, str);
		genericDataLength = strlen(str) + 4;
#else
		throw not_implemented_exception();
#endif
	}

	const char* TextKey::getText() const {
		return text;
	}

	void TextKey::setText(const char* t) {
#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		mwse::tes3::setDataString(&text, t);
#else
		throw not_implemented_exception();
#endif
	}

	nonstd::span<TextKey> TextKeyExtraData::getKeys() const {
		if (key) {
			return nonstd::span(key, keyCount);
		}
		return {};
	}
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::ExtraData)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::StringExtraData)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::Tes3ExtraData)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::TextKeyExtraData)
#endif
