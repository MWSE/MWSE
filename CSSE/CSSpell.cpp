#include "CSSpell.h"

#include "StringUtil.h"

namespace se::cs {
	bool Spell::getSpellFlag(SpellFlag::Flag flag) const {
		return (spellFlags & flag) == flag;
	}

	bool Spell::getPlayerStart() const {
		return getSpellFlag(SpellFlag::PCStartSpell);
	}

	bool Spell::search(const std::string_view& needle, const SearchSettings& settings, std::regex* regex) const {
		if (Object::search(needle, settings, regex)) {
			return true;
		}

		if (settings.effect) {
			for (const auto& effect : effects) {
				if (effect.search(needle, settings, regex)) {
					return true;
				}
			}
		}

		return false;
	}
}
