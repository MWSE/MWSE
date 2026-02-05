#include "NIExtraData.h"

#include "LuaUtil.h"

#include "TES3Util.h"

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

	const auto NI_StringExtraData_ctor = reinterpret_cast<void(__thiscall*)(NI::StringExtraData*, const char*)>(0x6E0EB0);
	StringExtraData::StringExtraData(const char* str) {
		NI_StringExtraData_ctor(this, str);
	}

	const auto NI_StringExtraData_dtor = reinterpret_cast<void(__thiscall*)(NI::StringExtraData*)>(0x6E1530);
	StringExtraData::~StringExtraData() {
		NI_StringExtraData_dtor(this);
	}

	const char* StringExtraData::getString() const {
		return string;
	}

	void StringExtraData::setString(const char* str) {
		mwse::tes3::setDataString(&string, str);
		genericDataLength = strlen(str) + 4;
	}

	const char* TextKey::getText() const {
		return text;
	}

	void TextKey::setText(const char* t) {
		mwse::tes3::setDataString(&text, t);
	}

	nonstd::span<TextKey> TextKeyExtraData::getKeys() const {
		if (keys) {
			return nonstd::span(keys, keyCount);
		}
		return {};
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::ExtraData)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::StringExtraData)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::Tes3ExtraData)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::TextKeyExtraData)
