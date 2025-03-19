#include "CSIngredient.h"

#include "CSDataHandler.h"
#include "CSRecordHandler.h"

namespace se::cs {
	void Ingredient::getEffectName(char* buffer, size_t bufferSize, int index) const {
		const auto recordHandler = DataHandler::get()->recordHandler;
		const auto magicEffectData = recordHandler->getMagicEffect(effects[index]);
		if (magicEffectData == nullptr) {
			buffer[0] = '\0';
			return;
		}

		const auto response = magicEffectData->getComplexName(effectAttributeIds[index], effectSkillIds[index]);
		strncpy_s(buffer, bufferSize, response.c_str(), response.size());
	}

	bool Ingredient::search(const std::string_view& needle, const SearchSettings& settings, std::regex* regex) const {
		if (Object::search(needle, settings, regex)) {
			return true;
		}

		const auto recordHandler = DataHandler::get()->recordHandler;
		for (auto i = 0; i < 4; ++i) {
			const auto effectData = recordHandler->getMagicEffect(effects[i]);
			if (effectData == nullptr) {
				return false;
			}

			if (effectData->search(needle, settings, regex, effectAttributeIds[i], effectSkillIds[i])) {
				return true;
			}
		}

		return false;
	}
}
