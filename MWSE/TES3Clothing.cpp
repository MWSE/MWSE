#include "TES3Clothing.h"

#include "LuaManager.h"

#include "TES3Util.h"
#include "LuaUtil.h"

#include "LuaUpdateBodyPartsForItemEvent.h"

#include "TES3BodyPart.h"
#include "TES3BodyPartManager.h"

namespace TES3 {
	const auto TES3_Clothing_ctor = reinterpret_cast<void(__thiscall*)(Clothing*)>(0x4A2C70);
	Clothing::Clothing() {
		TES3_Clothing_ctor(this);
	}

	const auto TES3_Clothing_dtor = reinterpret_cast<void(__thiscall*)(Clothing*)>(0x4A2D60);
	Clothing::~Clothing() {
		TES3_Clothing_dtor(this);
	}

	const auto TES3_Clothing_setupBodyParts = reinterpret_cast<void(__thiscall*)(const Clothing*, BodyPartManager*, bool, bool)>(0x4A38F0);
	void Clothing::setupBodyParts(BodyPartManager* bodyPartManager, bool isFemale, bool isFirstPerson) {
		auto item = this;

		// Add event replacing/adding body parts for an item.
		if (mwse::lua::event::UpdateBodyPartsForItemEvent::getEventEnabled()) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::object eventResult = stateHandle.triggerEvent(new mwse::lua::event::UpdateBodyPartsForItemEvent(this, bodyPartManager, isFemale, isFirstPerson));
			if (eventResult.valid()) {
				sol::table eventData = eventResult;
				if (eventData.get_or("block", false)) {
					return;
				}

				isFemale = mwse::lua::getOptionalParam(eventData, "isFemale", isFemale);
				const auto maybeItem = mwse::lua::getOptionalParamObject<Item>(eventData, "item");
				if (maybeItem && maybeItem->objectType == OBJECT_TYPE) {
					item = static_cast<Clothing*>(maybeItem);
				}
			}
		}

		item->removeBodyPartsUnder(bodyPartManager);
		item->addActiveBodyParts(bodyPartManager, isFemale, isFirstPerson);
	}

	void Clothing::addActiveBodyParts(BodyPartManager* bodyPartManager, bool isFemale, bool isFirstperson) {
		for (const auto& wearable : parts) {
			if (!wearable.isValid()) continue;

			const auto index = BodyPartManager::ActiveBodyPart::Index(wearable.bodypartID);
			bodyPartManager->setBodyPartForObject(this, index, wearable.getPart(isFemale), isFirstperson);
		}
	}

	void Clothing::removeBodyPartsUnder(BodyPartManager* bodyPartManager) const {
		using Layer = BodyPartManager::ActiveBodyPart::Layer;
		using Index = BodyPartManager::ActiveBodyPart::Index;

		switch (slot) {
		case ClothingSlot::Robe:
			bodyPartManager->removeActiveBodyPart(Layer::Base, Index::Chest, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Base, Index::Groin, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Base, Index::RightForearm, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Base, Index::LeftForearm, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Base, Index::RightUpperArm, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Base, Index::LeftUpperArm, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Base, Index::RightKnee, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Base, Index::LeftKnee, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Base, Index::RightUpperLeg, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Base, Index::LeftUpperLeg, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Clothing, Index::Chest, true, 0);
			bodyPartManager->removeActiveBodyPart(Layer::Clothing, Index::Groin, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Clothing, Index::RightForearm, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Clothing, Index::LeftForearm, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Clothing, Index::RightUpperArm, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Clothing, Index::LeftUpperArm, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Clothing, Index::RightKnee, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Clothing, Index::LeftKnee, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Clothing, Index::RightUpperLeg, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Clothing, Index::LeftUpperLeg, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Clothing, Index::Skirt, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Armor, Index::Chest, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Armor, Index::Groin, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Armor, Index::RightForearm, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Armor, Index::LeftForearm, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Armor, Index::RightUpperArm, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Armor, Index::LeftUpperArm, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Armor, Index::RightKnee, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Armor, Index::LeftKnee, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Armor, Index::RightUpperLeg, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Armor, Index::LeftUpperLeg, true, -1);
			break;
		case ClothingSlot::Skirt:
			bodyPartManager->removeActiveBodyPart(Layer::Base, Index::Groin, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Base, Index::RightUpperLeg, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Base, Index::LeftUpperLeg, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Clothing, Index::Groin, true, 0);
			bodyPartManager->removeActiveBodyPart(Layer::Clothing, Index::RightUpperLeg, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Clothing, Index::LeftUpperLeg, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Armor, Index::Groin, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Armor, Index::RightUpperLeg, true, -1);
			bodyPartManager->removeActiveBodyPart(Layer::Armor, Index::LeftUpperLeg, true, -1);
			break;
		}
	}

	void Clothing::setIconPath(const char* path) {
		if (strnlen_s(path, 32) >= 32) {
			throw std::invalid_argument("Path must not be 32 or more characters.");
		}
		mwse::tes3::setDataString(&icon, path);
	}

	const auto TES3_Clothing_getSlotName = reinterpret_cast<const char* (__thiscall*)(Clothing*)>(0x4A38E0);
	const char* Clothing::getSlotName() {
		// If this armor has weight and is of an invalid slot, return straight up armor rating.
		if (slot < ClothingSlot::First || slot > ClothingSlot::Last) {
			auto slotData = mwse::tes3::getClothingSlotData(slot);
			if (slotData) {
				return slotData->name.c_str();
			}

			return "Unknown";
		}

		return TES3_Clothing_getSlotName(this);
	}

	std::reference_wrapper<WearablePart[7]> Clothing::getParts() {
		return std::ref(parts);
	}

	bool Clothing::isUsableByBeasts() const {
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

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Clothing)
