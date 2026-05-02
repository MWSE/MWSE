#include "CSBook.h"

#include "CSDataHandler.h"
#include "CSRecordHandler.h"
#include "CSGameSetting.h"

#include "StringUtil.h"

namespace se::cs {
	const char* Book::getTaughtSkillName() const {
		const auto gmst = DataHandler::get()->recordHandler->getGameSettingForSkill(skillId);
		if (gmst == nullptr) {
			return "";
		}

		return gmst->value.asString;
	}

	bool Book::search(const std::string_view& needle, const SearchSettings& settings, std::regex* regex) const {
		if (Object::search(needle, settings, regex)) {
			return true;
		}

		if (text && settings.book_text && string::complex_contains(text, needle, settings, regex)) {
			return true;
		}

		return false;
	}
}
