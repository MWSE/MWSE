#include "TES3Misc.h"

#include "TES3Util.h"

#include "TES3DataHandler.h"
#include "TES3GameSetting.h"
#include "TES3GoldData.h"

namespace TES3 {
	const auto TES3_MiscItem_ctor = reinterpret_cast<void(__thiscall*)(Misc*)>(0x4A6320);
	Misc::Misc() :
		stolenList{},
		name{},
		script{},
		model{},
		icon{},
		weight{},
		value{},
		flags{}
	{
		TES3_MiscItem_ctor( this );
	}

	const auto TES3_MiscItem_dtor = reinterpret_cast<void(__thiscall*)(Misc*)>(0x4A6400);
	Misc::~Misc(){
		TES3_MiscItem_dtor( this );
	}

	const auto TES3_Misc_getGoldStackCount = reinterpret_cast<int(__cdecl*)(const Misc*)>(0x49A750);
	int Misc::getGoldStackCount() const {
		return TES3_Misc_getGoldStackCount(this);
	}

	void Misc::setIconPath(const char* path) {
		if (strnlen_s(path, 32) >= 32) {
			throw std::invalid_argument("Path must not be 32 or more characters.");
		}
		mwse::tes3::setDataString(&icon, path);
	}

	SoulGemData * Misc::getSoulGemData() const {
		return mwse::tes3::getSoulGemData(this);
	}

	bool Misc::isSoulGem() const {
		return mwse::tes3::isSoulGem(this);
	}

	int  Misc::getSoulGemCapacity() const {
		if (isSoulGem()) {
			return value * TES3::DataHandler::get()->nonDynamicData->GMSTs[TES3::GMST::fSoulGemMult]->value.asFloat;
		}
		return 0;
	}
	
	bool Misc::getIsKey() const {
		return flags & 1;
	}

	void Misc::setIsKey(bool value) {
		if (value) {
			flags |= 1;
		}
		else {
			flags &= ~1;
		}
	}

	bool Misc::isGold() const {
		auto goldData = GoldData::DATA_ARRAY;
		for (auto i = 0U; i < GoldData::DATA_ARRAY_COUNT; i++) {
			if (goldData[i].object == this) {
				return true;
			}
		}
		return false;
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Misc)
