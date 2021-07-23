#pragma once

#include "LuaUtil.h"
#include "TES3Util.h"

#include "TES3Activator.h"
#include "TES3Misc.h"
#include "TES3Sound.h"
#include "TES3Static.h"
#include "TES3Enchantment.h"
#include "TES3Weapon.h"
#include "TES3WorldController.h"

namespace mwse::lua {

	class ObjectCreatorBase {
	public:
		virtual TES3::BaseObject* create(sol::table, bool) const = 0;
	};

	template<typename ObjectType>
	class ObjectCreator : public ObjectCreatorBase {
	public:
		TES3::BaseObject* create(sol::table, bool) const override {
			throw std::runtime_error{ "tes3.createObject: Cannot create an object of that type." };
		}
	};

	using InvalidObjectCreator = ObjectCreator<void>;

	template<>
	class ObjectCreator<TES3::Activator> : public ObjectCreatorBase {
	public:
		TES3::BaseObject* create(sol::table params, bool getIfExists) const override {
			std::string id = getOptionalParam<std::string>(params, "id", {});

			if (id.size() > 31)
				throw std::invalid_argument{ "tes3activator.create: 'id' parameter must be less than 32 character long." };

			if (auto existingObject = TES3::DataHandler::get()->nonDynamicData->resolveObject(id.c_str()); existingObject != nullptr)
				return (getIfExists && (existingObject->objectType == TES3::ObjectType::Activator)) ?
				existingObject :
				throw std::invalid_argument{ "tes3activator.create: 'id' parameter already assigned to an existing object that is not an activator." };

			std::string name = getOptionalParam<std::string>(params, "name", "Activator");
			if (name.size() > 31)
				throw std::invalid_argument{ "tes3activator.create: 'name' parameter must be less than 32 character long." };

			auto mesh = getOptionalParam<std::string>(params, "mesh", {});
			if (mesh.size() > 31)
				throw std::invalid_argument{ "tes3activator.create: 'mesh' parameter must be less than 32 character long." };

			auto activator = new TES3::Activator();

			activator->setID(id.c_str());
			activator->setName(name.c_str());
			activator->setModelPath(mesh.c_str());

			auto script = getOptionalParamScript(params, "script");

			if (script != nullptr)
				activator->script = script;

			activator->objectFlags = getOptionalParam<unsigned int>(params, "objectFlags", 0);

			activator->objectFlags |= TES3::ObjectFlag::Modified;

			if (!TES3::DataHandler::get()->nonDynamicData->addNewObject(activator))
				throw std::runtime_error("tes3activator.create: could not add the newly created activator in its proper collection.");

			// If created outside of a save game, mark the object as sourceless.
			if (getOptionalParam<bool>(params, "sourceless", false) || TES3::WorldController::get()->getMobilePlayer() == nullptr) {
				TES3::BaseObject::setSourcelessObject(activator);
			}

			return activator;
		}
	};

	template<>
	class ObjectCreator<TES3::Misc> : public ObjectCreatorBase {
	public:
		TES3::BaseObject* create(sol::table params, bool getIfExists) const override {
			std::string id = getOptionalParam<std::string>(params, "id", {});

			if (id.size() > 31)
				throw std::invalid_argument{ "tes3misc.create: 'id' parameter must be less than 32 character long." };

			if (auto existingObject = TES3::DataHandler::get()->nonDynamicData->resolveObject(id.c_str()); existingObject != nullptr)
				return (getIfExists && existingObject->objectType == TES3::ObjectType::Misc) ?
				existingObject :
				throw std::invalid_argument{ "tes3misc.create: 'id' parameter already assigned to an existing object that is not a misc item." };

			std::string name = getOptionalParam<std::string>(params, "name", "Miscellaneous item");
			if (name.size() > 31)
				throw std::invalid_argument{ "tes3misc.create: 'name' parameter must be less than 32 character long." };

			std::string mesh = getOptionalParam<std::string>(params, "mesh", {});
			if (mesh.size() > 31)
				throw std::invalid_argument{ "tes3misc.create: 'mesh' parameter must be less than 32 character long." };

			auto miscItem = new TES3::Misc();

			miscItem->setID(id.c_str());
			miscItem->setName(name.c_str());
			miscItem->setModelPath(mesh.c_str());

			auto script = getOptionalParamScript(params, "script");

			if (script != nullptr)
				miscItem->script = script;

			std::string icon = getOptionalParam<std::string>(params, "icon", {});

			if (!icon.empty() && icon.size() < 31)
				tes3::setDataString(&miscItem->icon, icon.c_str());

			miscItem->objectFlags = getOptionalParam<unsigned int>(params, "objectFlags", 0);
			miscItem->weight = getOptionalParam<float>(params, "weight", 0.0f);
			miscItem->value = getOptionalParam<int>(params, "value", 0);
			miscItem->flags = getOptionalParam<unsigned int>(params, "flags", 0);

			miscItem->objectFlags |= TES3::ObjectFlag::Modified;

			if (!TES3::DataHandler::get()->nonDynamicData->addNewObject(miscItem))
				throw std::runtime_error("tes3misc.create: could not add the newly created misc item in its proper collection.");

			// If created outside of a save game, mark the object as sourceless.
			if (getOptionalParam<bool>(params, "sourceless", false) || TES3::WorldController::get()->getMobilePlayer() == nullptr) {
				TES3::BaseObject::setSourcelessObject(miscItem);
			}

			return miscItem;
		}
	};

	template<>
	class ObjectCreator<TES3::Sound> : public ObjectCreatorBase {
	public:
		TES3::BaseObject* create(sol::table params, bool getIfExists) const override {
			std::string id = getOptionalParam<std::string>(params, "id", {});

			if (id.size() > 31) {
				throw std::invalid_argument{ "tes3sound.create: 'id' parameter must be less than 32 character long." };
			}

			if (auto existingObject = TES3::DataHandler::get()->nonDynamicData->findSound(id.c_str()); existingObject != nullptr) {
				if (getIfExists && existingObject->objectType == TES3::ObjectType::Sound) {
					return existingObject;
				}

				throw std::invalid_argument{ "tes3sound.create: 'id' parameter already assigned to an existing object that is not a weapon item." };
			}

			std::string filename = getOptionalParam<std::string>(params, "filename", {});
			if (filename.size() > 31) {
				throw std::invalid_argument{ "tes3sound.create: 'filename' parameter must be less than 32 character long." };
			}

			auto soundObject = new TES3::Sound();
			soundObject->setObjectID(id.c_str());
			soundObject->setFilename(filename.c_str());
			soundObject->setVolume(getOptionalParam<float>(params, "volume", 1.0f));
			soundObject->setMinDistance_lua(getOptionalParam<double>(params, "minDistance", 0.0));
			soundObject->setMaxDistance_lua(getOptionalParam<double>(params, "maxDistance", 0.0));

			soundObject->objectFlags = getOptionalParam<unsigned int>(params, "objectFlags", 0);
			soundObject->objectFlags |= TES3::ObjectFlag::Modified;

			if (!TES3::DataHandler::get()->nonDynamicData->addSound(soundObject)) {
				throw std::runtime_error("tes3sound.create: Could not add the newly created sound in its proper collection.");
			}

			// If created outside of a save game, mark the object as sourceless.
			if (getOptionalParam<bool>(params, "sourceless", false) || TES3::WorldController::get()->getMobilePlayer() == nullptr) {
				TES3::BaseObject::setSourcelessObject(soundObject);
			}

			return soundObject;
		}
	};

	template<>
	class ObjectCreator<TES3::Static> : public ObjectCreatorBase {
	public:
		TES3::BaseObject* create(sol::table params, bool getIfExists) const override {
			std::string id = getOptionalParam<std::string>(params, "id", {});

			if (id.size() > 31)
				throw std::invalid_argument{ "tes3static.create: 'id' parameter must be less than 32 character long." };

			if (auto existingObject = TES3::DataHandler::get()->nonDynamicData->resolveObject(id.c_str()); existingObject != nullptr) {
				if (getIfExists && existingObject->objectType == TES3::ObjectType::Static) {
					return existingObject;
				}

				throw std::invalid_argument{ "tes3weapon.create: 'id' parameter already assigned to an existing object that is not a weapon item." };
			}

			std::string mesh = getOptionalParam<std::string>(params, "mesh", {});
			if (mesh.size() > 31)
				throw std::invalid_argument{ "tes3static.create: 'mesh' parameter must be less than 32 character long." };

			auto staticObject = new TES3::Static();

			staticObject->setID(id.c_str());
			staticObject->setModelPath(mesh.c_str());

			staticObject->objectFlags = getOptionalParam<unsigned int>(params, "objectFlags", 0);

			staticObject->objectFlags |= TES3::ObjectFlag::Modified;

			if (!TES3::DataHandler::get()->nonDynamicData->addNewObject(staticObject))
				throw std::runtime_error("tes3static.create: could not add the newly created static in its proper collection.");

			// If created outside of a save game, mark the object as sourceless.
			if (getOptionalParam<bool>(params, "sourceless", false) || TES3::WorldController::get()->getMobilePlayer() == nullptr) {
				TES3::BaseObject::setSourcelessObject(staticObject);
			}

			return staticObject;
		}
	};

	template<>
	class ObjectCreator<TES3::Weapon> : public ObjectCreatorBase {
	public:
		TES3::BaseObject* create(sol::table params, bool getIfExists) const override {
			std::string id = getOptionalParam<std::string>(params, "id", {});
			if (id.size() > 31) {
				throw std::invalid_argument{ "tes3weapon.create: 'id' parameter must be less than 32 character long." };
			}

			auto objectType = getOptionalParam<unsigned int>(params, "objectType", TES3::ObjectType::Invalid);
			if (auto existingObject = TES3::DataHandler::get()->nonDynamicData->resolveObject(id.c_str()); existingObject != nullptr) {
				if (getIfExists && existingObject->objectType == objectType) {
					return existingObject;
				}

				throw std::invalid_argument{ "tes3weapon.create: 'id' parameter already assigned to an existing object of a different type." };
			}

			std::string name = getOptionalParam<std::string>(params, "name", "Miscellaneous item");
			if (name.size() > 31) {
				throw std::invalid_argument{ "tes3weapon.create: 'name' parameter must be less than 32 character long." };
			}

			std::string mesh = getOptionalParam<std::string>(params, "mesh", {});
			if (mesh.size() > 31) {
				throw std::invalid_argument{ "tes3weapon.create: 'mesh' parameter must be less than 32 character long." };
			}

			auto weapon = new TES3::Weapon();

			if (objectType == TES3::ObjectType::Ammo) {
				weapon->objectType = TES3::ObjectType::Ammo;
			}

			weapon->setID(id.c_str());
			weapon->setName(name.c_str());
			weapon->setModelPath(mesh.c_str());

			auto script = getOptionalParamScript(params, "script");

			if (script != nullptr) {
				weapon->script = script;
			}

			auto enchantment = getOptionalParamObject<TES3::Enchantment>(params, "enchantment");
			if (enchantment && enchantment->objectType == TES3::ObjectType::Enchantment) {
				weapon->enchantment = enchantment;
			}

			std::string icon = getOptionalParam<std::string>(params, "icon", {});
			if (!icon.empty() && icon.size() < 31) {
				tes3::setDataString(&weapon->icon, icon.c_str());
			}

			weapon->objectFlags = getOptionalParam<unsigned int>(params, "objectFlags", 0);
			weapon->weight = getOptionalParam<float>(params, "weight", 1.0f);
			weapon->value = getOptionalParam<int>(params, "value", 1);
			weapon->weaponType = getOptionalParam<unsigned char>(params, "type", TES3::WeaponType::ShortBlade1H);
			weapon->maxCondition = getOptionalParam<unsigned short>(params, "maxCondition", 100);
			weapon->speed = getOptionalParam<float>(params, "speed", 1.0f);
			weapon->reach = getOptionalParam<float>(params, "reach", 1.0f);
			weapon->enchantCapacity = getOptionalParam<unsigned short>(params, "enchantCapacity", 100);
			weapon->chopMin = getOptionalParam<unsigned char>(params, "chopMin", 1);
			weapon->chopMax = getOptionalParam<unsigned char>(params, "chopMax", 5);
			weapon->slashMin = getOptionalParam<unsigned char>(params, "slashMin", 1);
			weapon->slashMax = getOptionalParam<unsigned char>(params, "slashMax", 5);
			weapon->thrustMin = getOptionalParam<unsigned char>(params, "thrustMin", 1);
			weapon->thrustMax = getOptionalParam<unsigned char>(params, "thrustMax", 5);
			weapon->materialFlags = getOptionalParam<unsigned int>(params, "materialFlags", 0);

			weapon->setIgnoresNormalWeaponResistance(getOptionalParam<bool>(params, "ignoresNormalWeaponResistance", weapon->getIgnoresNormalWeaponResistance()));

			weapon->objectFlags |= TES3::ObjectFlag::Modified;

			if (!TES3::DataHandler::get()->nonDynamicData->addNewObject(weapon)) {
				throw std::runtime_error("tes3weapon.create: could not add the newly created misc item in its proper collection.");
			}

			// If created outside of a save game, mark the object as sourceless.
			if (getOptionalParam<bool>(params, "sourceless", false) || TES3::WorldController::get()->getMobilePlayer() == nullptr) {
				TES3::BaseObject::setSourcelessObject(weapon);
			}

			return weapon;
		}
	};

	template<>
	class ObjectCreator<TES3::Enchantment> : public ObjectCreatorBase {
	public:
		TES3::BaseObject* create(sol::table params, bool getIfExists) const override {
			std::string id = getOptionalParam<std::string>(params, "id", {});

			if (id.size() > 31)
				throw std::invalid_argument{ "tes3enchantment.create: 'id' parameter must be less than 32 character long." };

			auto existingObject = TES3::DataHandler::get()->nonDynamicData->resolveObject(id.c_str());
			if (existingObject) {
				if (existingObject->objectType == TES3::ObjectType::Enchantment) {
					return existingObject;
				}
				else {
					throw std::invalid_argument{ "tes3enchantment.create: 'id' parameter already assigned to an existing object that is not an enchantment." };
				}
			}

			auto castType = getOptionalParam(params, "castType", TES3::EnchantmentCastType::Invalid);
			if (castType >= TES3::EnchantmentCastType::Invalid)
				throw std::invalid_argument{ "tes3enchantment.create: 'castType' parameter as an incorrect value. Use values in tes3.enchantmentType" };

			unsigned short chargeCost = getOptionalParam<unsigned short>(params, "chargeCost", 0);
			if (chargeCost == 0)
				throw std::invalid_argument{ "tes3enchantment.create: 'chargeCost' parameter must be greater than 0." };

			unsigned short maxCharge = getOptionalParam<unsigned short>(params, "maxCharge", 0);
			if (maxCharge == 0)
				throw std::invalid_argument{ "tes3enchantment.create: 'maxCharge' parameter must be greater than 0." };

			auto enchantment = new TES3::Enchantment();

			enchantment->setID(id.c_str());
			enchantment->castType = castType;
			enchantment->chargeCost = chargeCost;
			enchantment->maxCharge = maxCharge;
			enchantment->flags = getOptionalParam<unsigned int>(params, "flags", 0);
			enchantment->objectFlags = getOptionalParam<unsigned int>(params, "objectFlags", 0);
			enchantment->objectFlags |= TES3::ObjectFlag::Modified;

			if (!TES3::DataHandler::get()->nonDynamicData->addNewObject(enchantment))
				throw std::runtime_error("tes3enchantment.create: could not add the newly created enchantment in its proper collection.");

			// If created outside of a save game, mark the object as sourceless.
			if (getOptionalParam<bool>(params, "sourceless", false) || TES3::WorldController::get()->getMobilePlayer() == nullptr) {
				TES3::BaseObject::setSourcelessObject(enchantment);
			}

			return enchantment;
		}
	};

	std::unique_ptr<ObjectCreatorBase> makeObjectCreator(TES3::ObjectType::ObjectType objectType);

}
