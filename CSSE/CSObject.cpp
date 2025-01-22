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

#include "StringUtil.h"

namespace se::cs {
	const char* Object::getName() const {
		return vtbl.object->getName(this);
	}

	bool Object::isMarker() const {
		return vtbl.object->isMarker(this);
	}

	char* Object::getIcon() const {
		return vtbl.object->getIconPath(this);
	}

	char* Object::getModel() const {
		return vtbl.object->getModelPath(this);
	}

	Object* Object::getEnchantment() const {
		return vtbl.object->getEnchantment(this);
	}

	Script* Object::getScript() const {
		return vtbl.object->getScript(this);
	}

	float Object::getScale() const {
		return vtbl.object->getScale(this);
	}

	void Object::setScale(float scale, bool clamp) {
		vtbl.object->setScale(this, scale, clamp);
	}

	int Object::getCount() const {
		return vtbl.object->getCount(this);
	}

	const char* Object::getTypeName() const {
		return vtbl.object->getTypeName(this);
	}

	Sound* Object::getSound() const {
		return vtbl.object->getSound(this);
	}

	const char* Object::getRaceName() const {
		return vtbl.object->getRaceName(this);
	}

	const char* Object::getClassName() const {
		return vtbl.object->getClassName(this);
	}

	const char* Object::getFactionName() const {
		return vtbl.object->getFactionName(this);
	}

	Faction* Object::getFaction() const {
		return vtbl.object->getFaction(this);
	}

	bool Object::getIsFemale() const {
		return vtbl.object->getIsFemale(this);
	}

	bool Object::getIsEssential() const {
		return vtbl.object->getIsEssential(this);
	}

	bool Object::getRespawns() const {
		return vtbl.object->getRespawns(this);
	}

	int Object::getLevel() const {
		return vtbl.object->getLevel(this);
	}

	bool Object::getAutoCalc() const {
		return vtbl.object->getAutoCalc(this);
	}

	float Object::getWeight() const {
		return vtbl.object->getWeight(this);
	}

	int Object::getValue() const {
		return vtbl.object->getValue(this);
	}

	void Object::populateObjectWindow(HWND hWnd) const {
		vtbl.object->populateObjectWindow(this, hWnd);
	}

	bool Object::search(const std::string_view& needle, const SearchSettings& settings, std::regex* regex) const {
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

	bool Object::searchWithInheritance(const std::string_view& needle, const SearchSettings& settings, std::regex* regex) const {
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
		}

		// Do the usual object searches then pass off to BaseObject tests.
		if (search(needle, settings, regex)) {
			return true;
		}

		return false;
	}
}
