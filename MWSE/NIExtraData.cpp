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

	const char* StringExtraData::getString() const {
		return string;
	}

	void StringExtraData::setString(const char* str) {
		mwse::tes3::setDataString(&string, str);
	}

	const char* TextKey::getText() const {
		return text;
	}

	void TextKey::setText(const char* t) {
		mwse::tes3::setDataString(&text, t);
	}

	nonstd::span<TextKey> TextKeyExtraData::getKeys() const {
		if (key) {
			return nonstd::span(key, keyCount);
		}
		return {};
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::ExtraData)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::StringExtraData)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::Tes3ExtraData)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::TextKeyExtraData)
