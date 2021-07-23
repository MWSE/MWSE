#include "LuaObject.h"

namespace mwse::lua {
	std::unique_ptr<ObjectCreatorBase> makeObjectCreator(TES3::ObjectType::ObjectType objectType) {
		switch (objectType) {
		case TES3::ObjectType::Activator:
			return std::make_unique<ObjectCreator<TES3::Activator>>();
		case TES3::ObjectType::Ammo:
			return std::make_unique<ObjectCreator<TES3::Weapon>>();
		case TES3::ObjectType::Misc:
			return std::make_unique<ObjectCreator<TES3::Misc>>();
		case TES3::ObjectType::Sound:
			return std::make_unique<ObjectCreator<TES3::Sound>>();
		case TES3::ObjectType::Static:
			return std::make_unique<ObjectCreator<TES3::Static>>();
		case TES3::ObjectType::Enchantment:
			return std::make_unique<ObjectCreator<TES3::Enchantment>>();
		case TES3::ObjectType::Weapon:
			return std::make_unique<ObjectCreator<TES3::Weapon>>();
		default:
			return std::make_unique<InvalidObjectCreator>();
		}
	}
}