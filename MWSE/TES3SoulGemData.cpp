#include "TES3SoulGemData.h"

#include "TES3DataHandler.h"
#include "TES3GameSetting.h"
#include "TES3Misc.h"

namespace TES3 {
	std::string SoulGemData::toString() const{
		return id;
	}

	std::string SoulGemData::toJson() const {
		std::ostringstream ss;
		ss << "\"tes3soulGemData:" << id << "\"";
		return std::move(ss.str());
	}

	int SoulGemData::getValue() const {
		if (item) {
			return item->value;
		}
		else {
			return value;
		}
	}

	float SoulGemData::getWeight() const {
		if (item) {
			return item->weight;
		}
		else {
			return weight;
		}
	}

	int SoulGemData::getCapacity() const {
		if (item) {
			return item->getSoulGemCapacity();
		}
		else {
			return value * TES3::DataHandler::get()->nonDynamicData->GMSTs[TES3::GMST::fSoulGemMult]->value.asFloat;
		}
	}
}
