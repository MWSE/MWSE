#include "CSObject.h"

#include "CSAlchemy.h"
#include "CSBirthsign.h"
#include "CSBook.h"
#include "CSClass.h"
#include "CSEnchantment.h"
#include "CSFaction.h"
#include "CSIngredient.h"
#include "CSRace.h"
#include "CSScript.h"
#include "CSSpell.h"
#include "CSNPC.h"

#include "StringUtil.h"

namespace se::cs {
	const char* Object::getName() const {
		return vTable.object->getName(this);
	}

	bool Object::isLocationMarker() const {
		return vTable.object->isLocationMarker(this);
	}

	char* Object::getIcon() const {
		return vTable.object->getIconPath(this);
	}

	char* Object::getModel() const {
		return vTable.object->getModelPath(this);
	}

	Enchantment* Object::getEnchantment() const {
		return vTable.object->getEnchantment(this);
	}

	Script* Object::getScript() const {
		return vTable.object->getScript(this);
	}

	float Object::getScale() const {
		return vTable.object->getScale(this);
	}

	void Object::setScale(float scale, bool clamp) {
		vTable.object->setScale(this, scale, clamp);
	}

	int Object::getCount() const {
		return vTable.object->getCount(this);
	}

	const char* Object::getTypeName() const {
		return vTable.object->getTypeName(this);
	}

	Sound* Object::getSound() const {
		return vTable.object->getSound(this);
	}

	const char* Object::getRaceID() const {
		return vTable.object->getRaceID(this);
	}

	const char* Object::getClassID() const {
		return vTable.object->getClassID(this);
	}

	const char* Object::getFactionID() const {
		return vTable.object->getFactionID(this);
	}

	Faction* Object::getFaction() const {
		return vTable.object->getFaction(this);
	}

	bool Object::getIsFemale() const {
		return vTable.object->getIsFemale(this);
	}

	bool Object::getIsEssential() const {
		return vTable.object->getIsEssential(this);
	}

	bool Object::getRespawns() const {
		return vTable.object->getRespawns(this);
	}

	int Object::getLevel() const {
		return vTable.object->getLevel(this);
	}

	bool Object::getAutoCalc() const {
		return vTable.object->getAutoCalc(this);
	}

	float Object::getWeight() const {
		return vTable.object->getWeight(this);
	}

	int Object::getValue() const {
		return vTable.object->getValue(this);
	}

	void Object::populateObjectWindow(HWND hWnd) const {
		vTable.object->populateObjectWindow(this, hWnd);
	}

	bool Object::search(std::string_view needle, const SearchSettings& settings, std::regex* regex) const {
		if (BaseObject::search(needle, settings, regex)) {
			return true;
		}

		const auto name = getName();
		if (name && settings.name && string::complex_contains(name, needle, settings, regex)) {
			return true;
		}

		const auto model = getModel();
		if (model && settings.model_path && string::complex_contains(model, needle, settings, regex)) {
			return true;
		}

		const auto icon = getIcon();
		if (icon && settings.icon_path && string::complex_contains(icon, needle, settings, regex)) {
			return true;
		}

		const auto script = getScript();
		if (script && settings.script_id && string::complex_contains(script->getObjectID(), needle, settings, regex)) {
			return true;
		}

		const auto enchantment = getEnchantment();
		if (enchantment && settings.enchantment_id && string::complex_contains(enchantment->getObjectID(), needle, settings, regex)) {
			return true;
		}

		return false;
	}

	bool Object::searchWithInheritance(std::string_view needle, const SearchSettings& settings, std::regex* regex) const {
		switch (objectType) {
		case ObjectType::Alchemy:
			return static_cast<const Alchemy*>(this)->search(needle, settings, regex);
		case ObjectType::Book:
			return static_cast<const Book*>(this)->search(needle, settings, regex);
		case ObjectType::Enchantment:
			return static_cast<const Enchantment*>(this)->search(needle, settings, regex);
		case ObjectType::Ingredient:
			return static_cast<const Ingredient*>(this)->search(needle, settings, regex);
		case ObjectType::Spell:
			return static_cast<const Spell*>(this)->search(needle, settings, regex);
		case ObjectType::NPC:
			return static_cast<const NPC*>(this)->search(needle, settings, regex);
		}

		// Do the usual object searches then pass off to BaseObject tests.
		if (search(needle, settings, regex)) {
			return true;
		}

		return false;
	}
}
