#include "CSGameSetting.h"

#include "CSDataHandler.h"
#include "CSRecordHandler.h"

#include "StringUtil.h"

namespace se::cs {
	//
	// GameSettingInitializer
	//

	int GameSettingInitializer::getIndex() const {
		return (reinterpret_cast<DWORD>(this) - 0x6A8128) / sizeof(GameSettingInitializer);
	}

	GameSettingInitializer::ValueType GameSettingInitializer::getType() const {
		switch (tolower(name[0])) {
		case 'i':
			return ValueType::Integer;
		case 'f':
			return ValueType::Float;
		case 's':
			return ValueType::String;
		}
		throw std::runtime_error("Initializer contains unexpected value type.");
	}

	GameSetting* GameSettingInitializer::getSetting() const {
		return DataHandler::get()->recordHandler->gameSettingsHandler->gameSettings[getIndex()];
	}

	nonstd::span<GameSettingInitializer> GameSettingInitializer::get() {
		const auto initializers = reinterpret_cast<GameSettingInitializer*>(0x6A8128);
		return nonstd::span(initializers, GMST::COUNT);
	}

	//
	// GameSetting
	//

	GameSettingInitializer* GameSetting::getInitializer() const {
		const auto initializers = reinterpret_cast<GameSettingInitializer*>(0x6A8128);
		return &initializers[index];
	}

	bool GameSetting::search(const std::string_view& needle, const SearchSettings& settings, std::regex* regex) const {
		if (BaseObject::search(needle, settings, regex)) {
			return true;
		}

		if (getInitializer()->getType() != GameSettingInitializer::ValueType::String) {
			return false;
		}

		return string::complex_contains(value.asString, needle, settings, regex);
	}

}