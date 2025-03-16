#include "TES3Armor.h"

#include "LuaManager.h"

#include "LuaCalcArmorRatingEvent.h"
#include "LuaUpdateBodyPartsForItemEvent.h"

#include "TES3Util.h"
#include "LuaUtil.h"

#include "TES3BodyPart.h"
#include "TES3BodyPartManager.h"
#include "TES3DataHandler.h"
#include "TES3GameSetting.h"
#include "TES3MobileActor.h"
#include "TES3NPC.h"
#include "TES3Reference.h"
#include "TES3Skill.h"

namespace TES3 {
	const auto TES3_Armor_ctor = reinterpret_cast<void(__thiscall*)(Armor*)>(0x4A0360);
	Armor::Armor() {
		TES3_Armor_ctor(this);
	}

	const auto TES3_Armor_dtor = reinterpret_cast<void(__thiscall*)(Armor*)>(0x4A0450);
	Armor::~Armor() {
		TES3_Armor_dtor(this);
	}

	const auto TES3_Armor_calculateArmorRating = reinterpret_cast<float(__thiscall *)(Armor*, MobileActor*)>(0x4A1120);
	float Armor::calculateArmorRating(MobileActor * actor) {
		// Allow the event to override the value.
		if (mwse::lua::event::CalculateArmorRatingEvent::getEventEnabled()) {
			auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::object eventResult = stateHandle.triggerEvent(new mwse::lua::event::CalculateArmorRatingEvent(this, actor));
			if (eventResult.valid()) {
				sol::table eventData = eventResult;

				sol::optional<float> value = eventData["armorRating"];
				if (eventData.get_or("block", false) && value) {
					return value.value();
				}
			}
		}

		// Figure out armor value for custom slots.
		if (slot < ArmorSlot::First || slot > ArmorSlot::Last) {
			float armorSkillValue = 0.0f;
			switch (getWeightClass()) {
			case TES3::ArmorWeightClass::Light:
				armorSkillValue = actor->getSkillValue(TES3::SkillID::LightArmor);
				break;
			case TES3::ArmorWeightClass::Medium:
				armorSkillValue = actor->getSkillValue(TES3::SkillID::MediumArmor);
				break;
			case TES3::ArmorWeightClass::Heavy:
				armorSkillValue = actor->getSkillValue(TES3::SkillID::HeavyArmor);
				break;
			default:
				return 0.0f;
			}

			return armorRating * armorSkillValue / TES3::DataHandler::get()->nonDynamicData->GMSTs[TES3::GMST::iBaseArmorSkill]->value.asLong * getArmorScalar();
		}

		return TES3_Armor_calculateArmorRating(this, actor);
	}

	const auto TES3_Armor_calculateArmorRatingForNPC = reinterpret_cast<float(__thiscall *)(Armor*, NPC*)>(0x4A0FD0);
	float Armor::calculateArmorRatingForNPC(NPC * npc) {
		// Allow the event to override the value.
		if (mwse::lua::event::CalculateArmorRatingEvent::getEventEnabled()) {
			auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::object eventResult = stateHandle.triggerEvent(new mwse::lua::event::CalculateArmorRatingEvent(this, npc));
			if (eventResult.valid()) {
				sol::table eventData = eventResult;

				sol::optional<float> value = eventData["armorRating"];
				if (eventData.get_or("block", false) && value) {
					return value.value();
				}
			}
		}

		// Figure out armor value for custom slots.
		if (slot < ArmorSlot::First || slot > ArmorSlot::Last) {
			float armorSkillValue = 0.0f;
			switch (getWeightClass()) {
			case TES3::ArmorWeightClass::Light:
				armorSkillValue = npc->skills[TES3::SkillID::LightArmor];
				break;
			case TES3::ArmorWeightClass::Medium:
				armorSkillValue = npc->skills[TES3::SkillID::MediumArmor];
				break;
			case TES3::ArmorWeightClass::Heavy:
				armorSkillValue = npc->skills[TES3::SkillID::HeavyArmor];
				break;
			default:
				return 0.0f;
			}

			return armorRating * armorSkillValue / TES3::DataHandler::get()->nonDynamicData->GMSTs[TES3::GMST::iBaseArmorSkill]->value.asLong * getArmorScalar();
		}

		return TES3_Armor_calculateArmorRatingForNPC(this, npc);
	}

	const auto TES3_Armor_getSlotName = reinterpret_cast<const char*(__thiscall *)(Armor*)>(0x4A1270);
	const char* Armor::getSlotName() {
		// If this armor has weight and is of an invalid slot, return straight up armor rating.
		if (slot < ArmorSlot::First || slot > ArmorSlot::Last) {
			auto slotData = mwse::tes3::getArmorSlotData(slot);
			if (slotData) {
				return slotData->name.c_str();
			}

			return "Unknown";
		}

		return TES3_Armor_getSlotName(this);
	}

	const auto TES3_Armor_getWeightClass = reinterpret_cast<int(__thiscall *)(Armor*)>(0x4A0F30);
	int Armor::getWeightClass() {
		// Figure out custom slots.
		if (slot < ArmorSlot::First || slot > ArmorSlot::Last) {
			// If we have no custom data, assume light.
			auto slotData = mwse::tes3::getArmorSlotData(slot);
			if (slotData == nullptr) {
				return ArmorWeightClass::Light;
			}

			float armorEpsilon = *reinterpret_cast<float*>(0x7483A0);
			auto dataHandler = TES3::DataHandler::get();
			if (slotData->weight * dataHandler->nonDynamicData->GMSTs[TES3::GMST::fLightMaxMod]->value.asFloat + armorEpsilon < weight) {
				return ArmorWeightClass::Light;
			}
			else if (slotData->weight * dataHandler->nonDynamicData->GMSTs[TES3::GMST::fMedMaxMod]->value.asFloat + armorEpsilon < weight) {
				return ArmorWeightClass::Medium;
			}
			else {
				return ArmorWeightClass::Heavy;
			}
		}

		// Finally, we fall back to the vanilla function.
		return TES3_Armor_getWeightClass(this);
	}

	const auto TES3_Armor_setupBodyParts = reinterpret_cast<void(__thiscall*)(const Armor*, BodyPartManager*, bool, bool)>(0x4A1280);
	void Armor::setupBodyParts(BodyPartManager* bodyPartManager, bool isFemale, bool isFirstPerson) {
		auto item = this;

		// Add event replacing/adding body parts for an item.
		if (mwse::lua::event::UpdateBodyPartsForItemEvent::getEventEnabled()) {
			auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::object eventResult = stateHandle.triggerEvent(new mwse::lua::event::UpdateBodyPartsForItemEvent(this, bodyPartManager, isFemale, isFirstPerson));
			if (eventResult.valid()) {
				sol::table eventData = eventResult;
				if (eventData.get_or("block", false)) {
					return;
				}

				isFemale = mwse::lua::getOptionalParam(eventData, "isFemale", isFemale);
				const auto maybeItem = mwse::lua::getOptionalParamObject<Item>(eventData, "item");
				if (maybeItem && maybeItem->objectType == OBJECT_TYPE) {
					item = static_cast<Armor*>(maybeItem);
				}
			}
		}

		item->removeBodyPartsUnder(bodyPartManager);
		item->addActiveBodyParts(bodyPartManager, isFemale, isFirstPerson);
	}

	void Armor::addActiveBodyParts(BodyPartManager* bodyPartManager, bool isFemale, bool isFirstperson) {
		for (const auto& wearable : parts) {
			if (!wearable.isValid()) continue;

			const auto index = BodyPartManager::ActiveBodyPart::Index(wearable.bodypartID);
			bodyPartManager->setBodyPartForObject(this, index, wearable.getPart(isFemale), isFirstperson);
		}
	}

	void Armor::removeBodyPartsUnder(BodyPartManager* bodyPartManager) const {
		for (auto& wearable : parts) {
			if (!wearable.isValid()) continue;

			const auto index = BodyPartManager::ActiveBodyPart::Index(wearable.bodypartID);
			if (index == BodyPartManager::ActiveBodyPart::Index::Head) {
				bodyPartManager->removeActiveBodyPart(BodyPartManager::ActiveBodyPart::Layer::Base, BodyPartManager::ActiveBodyPart::Index::Hair, 1, 0);
			}
		}
	}

	float Armor::getArmorScalar() const {
		// Handle custom slots.
		if (slot < ArmorSlot::First || slot > ArmorSlot::Last) {
			auto slotData = mwse::tes3::getArmorSlotData(slot);
			if (slotData) {
				return slotData->armorScalar;
			}
		}
		else {
			// Default slot values.
			switch (slot) {
			case ArmorSlot::Cuirass:
				return 0.3f;
			case ArmorSlot::Helmet:
			case ArmorSlot::LeftPauldron:
			case ArmorSlot::RightPauldron:
			case ArmorSlot::Greaves:
			case ArmorSlot::Boots:
			case ArmorSlot::Shield:
				return 0.1f;
			case ArmorSlot::LeftGauntlet:
			case ArmorSlot::RightGauntlet:
			case ArmorSlot::LeftBracer:
			case ArmorSlot::RightBracer:
				return 0.05f;
			}
		}

		return 0.0f;
	}

	void Armor::setDurability(int value) {
		maxCondition = value;
	}

	void Armor::setIconPath(const char* path) {
		if (strnlen_s(path, 32) >= 32) {
			throw std::invalid_argument("Path must not be 32 or more characters.");
		}
		mwse::tes3::setDataString(&icon, path);
	}

	std::reference_wrapper<WearablePart[7]> Armor::getParts() {
		return std::ref(parts);
	}

	float Armor::calculateArmorRating_lua(sol::object actor) {
		// If we're explicitly given a mobile actor, use that.
		if (actor.is<TES3::MobileActor>()) {
			return calculateArmorRating(actor.as<TES3::MobileActor*>());
		}

		// If we're given a reference, try to get its mobile actor.
		else if (actor.is<TES3::Reference>()) {
			TES3::MobileActor* mobileActor = actor.as<TES3::Reference*>()->getAttachedMobileActor();
			if (mobileActor) {
				return calculateArmorRating(mobileActor);
			}
			else {
				throw std::exception("Reference does not have an attached mobile actor. Is this an NPC or creature reference?");
			}
		}

		// If we were given something else, tell them they goofed.
		else {
			throw std::exception("Invalid function call. Requires mobile actor or reference as a parameter.");
		}
	}

	bool Armor::isClosedHelmet() const {
		for (auto &part : parts) {
			// Does the helmet cover head? If so, it's a closed helmet.
			if (part.bodypartID == static_cast<int>(TES3::BodyPartManager::ActiveBodyPart::Index::Head)) {
				return true;
			}
		}
		return false;
	}

	bool Armor::isUsableByBeasts() const {
		if (isClosedHelmet()) {
			return false;
		}
		constexpr auto leftFootID = static_cast<int>(TES3::BodyPartManager::ActiveBodyPart::Index::LeftFoot);
		constexpr auto rightFootID = static_cast<int>(TES3::BodyPartManager::ActiveBodyPart::Index::RightFoot);
		for (auto& part : parts) {
			if (part.bodypartID == leftFootID || part.bodypartID == rightFootID) {
				return false;
			}
		}
		return true;
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Armor)
