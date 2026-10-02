#include "CSSpell.h"

#include "StringUtil.h"

namespace se::cs {
	bool Spell::getSpellFlag(SpellFlag::Flag flag) const {
		return (spellFlags & flag) == flag;
	}

	bool Spell::getPlayerStart() const {
		return getSpellFlag(SpellFlag::PCStartSpell);
	}

	bool Spell::search(std::string_view needle, const SearchSettings& settings, std::regex* regex) const {
		if (Object::search(needle, settings, regex)) {
			return true;
		}

		if (settings.effect) {
			return std::ranges::any_of(effects, [&](const auto& effect) {
				return effect.search(needle, settings, regex);
			});
		}

		return false;
	}
}
