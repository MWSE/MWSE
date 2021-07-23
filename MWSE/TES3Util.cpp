#include "TES3Util.h"

#include "Log.h"

#include "mwOffsets.h"

#include "TES3Actor.h"
#include "TES3Attachment.h"
#include "TES3Class.h"
#include "TES3Clothing.h"
#include "TES3DataHandler.h"
#include "TES3Enchantment.h"
#include "TES3GameSetting.h"
#include "TES3MagicEffectController.h"
#include "TES3Misc.h"
#include "TES3MobilePlayer.h"
#include "TES3NPC.h"
#include "TES3Reference.h"
#include "TES3Skill.h"
#include "TES3SoulGemData.h"
#include "TES3Spell.h"
#include "TES3MagicSourceInstance.h"
#include "TES3WorldController.h"

#include "TES3UIManager.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "LuaLoadGameEvent.h"
#include "LuaLoadedGameEvent.h"

#include "MemoryUtil.h"

namespace mwse {
	namespace tes3 {
		TES3::Reference* getReference(const char* id) {
			TES3::DataHandler* dataHandler = TES3::DataHandler::get();
			if (!dataHandler || !dataHandler->nonDynamicData) {
				return nullptr;
			}

			if (!_stricmp(id, "player") || !_stricmp(id, "playersavegame")) {
				return dataHandler->nonDynamicData->playerSaveGame;
			}
			else {
				if (dataHandler) {
					return dataHandler->nonDynamicData->findFirstCloneOfActor(id);
				}
			}

			return nullptr;
		}

		TES3::Reference* getReference(std::string& id) {
			return getReference(id.c_str());
		}

		const auto TES3_general_setStringSlot = reinterpret_cast<void(__cdecl *)(char**, const char*)>(0x47B410);
		void setDataString(char** container, const char* string, bool allowEmpty) {
			if (allowEmpty && string && string[0] == '\0') {
				if (*container) {
					_delete(*container);
				}
				*container = _new<char>(1);
				**container = '\0';
			}
			else {
				TES3_general_setStringSlot(container, string);
			}
		}

		bool setEffect(TES3::Effect * effects, long index, long effectId,
			long skillAttributeId, long range, long area, long duration,
			long minimumMagnitude, long maximumMagnitude) {
			// Validate effect pointer.
			if (effects == NULL) {
				mwse::log::getLog() << __FUNCTION__ << ": No effect passed." << std::endl;
				return false;
			}

			// Validate index.
			if (index < 1 || index > 8) {
				mwse::log::getLog() << __FUNCTION__ << ": Index must be between [1,8]." << std::endl;
				return false;
			}

			// Validate effect id.
			if (effectId < TES3::EffectID::FirstEffect || effectId > TES3::EffectID::LastEffect) {
				mwse::log::getLog() << __FUNCTION__ << ": Effect id outside bounds." << std::endl;
				return false;
			}

			// Validate that the effect supports the range type.
			auto flags = TES3::DataHandler::get()->nonDynamicData->magicEffects->getEffectFlags(effectId);
			const auto effectRange = static_cast<TES3::EffectRange>(range);
			if ((effectRange == TES3::EffectRange::Self && !(flags & TES3::EffectFlag::CanCastSelf)) ||
				(effectRange == TES3::EffectRange::Touch && !(flags & TES3::EffectFlag::CanCastTouch)) ||
				(effectRange == TES3::EffectRange::Target && !(flags & TES3::EffectFlag::CanCastTarget))) {
#if _DEBUG
				mwse::log::getLog() << __FUNCTION__ << ": Effect " << effectId << " (with flags " << std::hex << flags << std::dec << ") does not support given range type of " << range << "." << std::endl;
#endif
				return false;
			}

			// Convert index to zero-based.
			index--;

			// Set basic effect data.
			TES3::Effect& effect = effects[index];
			effect.effectID = effectId;
			effect.rangeType = effectRange;
			effect.radius = area;

			// Set skill.
			if (flags & TES3::EffectFlag::TargetSkill) {
				effect.skillID = skillAttributeId;
			}
			else {
				effect.skillID = TES3::SkillID::Invalid;
			}

			// Set attribute.
			if (flags & TES3::EffectFlag::TargetAttribute) {
				effect.attributeID = skillAttributeId;
			}
			else {
				effect.attributeID = TES3::Attribute::Invalid;
			}

			// Set duration.
			if (flags & TES3::EffectFlag::NoDuration) {
				effect.duration = 0;
			}
			else {
				effect.duration = duration;
			}

			// Set magnitude.
			if (flags & TES3::EffectFlag::NoMagnitude) {
				effect.magnitudeMin = 0;
				effect.magnitudeMax = 0;
			}
			else {
				effect.magnitudeMin = minimumMagnitude;
				effect.magnitudeMax = maximumMagnitude;
			}

			return true;
		}

		void checkForLevelUp(long progress) {
			TES3::NonDynamicData* nonDynamicData = TES3::DataHandler::get()->nonDynamicData;
			if (progress >= nonDynamicData->GMSTs[TES3::GMST::iLevelupTotal]->value.asLong) {
				const char* levelUpMessage = nonDynamicData->GMSTs[TES3::GMST::sLevelUpMsg]->value.asString;
				TES3::UI::showMessageBox(levelUpMessage);
			}
		}


		int getSkillNameGMST(int id) {
			return TES3::Skill::NAME_GMSTS[id];
		}

		int getAttributeNameGMST(int id) {
			return reinterpret_cast<int*>(0x794410)[id];
		}

		int getCastRangeNameGMST(int id) {
			return reinterpret_cast<int*>(0x7947C8)[id];
		}

		static std::unordered_map<const TES3::Misc*, TES3::SoulGemData*> customSoulGems;

		bool isSoulGem(const TES3::Object* objectOrReference) {
			if (reinterpret_cast<bool(__cdecl *)(const TES3::Object*)>(0x49ABE0)(objectOrReference)) {
				return true;
			}

			// If we were given a reference, look at the base object.
			if (objectOrReference->objectType == TES3::ObjectType::Reference) {
				objectOrReference = reinterpret_cast<const TES3::Reference*>(objectOrReference)->baseObject;
			}

			if (objectOrReference->objectType == TES3::ObjectType::Misc) {
				auto searchResult = customSoulGems.find(reinterpret_cast<const TES3::Misc*>(objectOrReference));
				if (searchResult != customSoulGems.end()) {
					return true;
				}
			}

			return false;
		}

		TES3::SoulGemData * addCustomSoulGem(const TES3::Misc * item) {
			if (isSoulGem(item)) {
				return nullptr;
			}

			auto data = new TES3::SoulGemData();
			data->item = const_cast<TES3::Misc*>(item);
			data->id = item->objectID;
			data->name = item->name;
			data->mesh = item->model;
			data->texture = item->icon;
			data->value = item->value;
			data->weight = item->weight;

			customSoulGems[item] = data;
			return data;
		}

		TES3::SoulGemData * getSoulGemData(const TES3::Misc * item) {
			TES3::SoulGemData * vanillaSoulGems = reinterpret_cast<TES3::SoulGemData*>(0x791C98);
			for (size_t i = 0; i < 6; i++) {
				if (vanillaSoulGems[i].item == item) {
					return &vanillaSoulGems[i];
				}
			}

			auto searchResult = customSoulGems.find(item);
			if (searchResult != customSoulGems.end()) {
				return searchResult->second;
			}

			return nullptr;
		}

		static std::map<int, std::shared_ptr<TES3::ClothingSlotData>> customClothingSlots;

		std::shared_ptr<TES3::ClothingSlotData> getClothingSlotData(int slot) {
			auto searchResult = customClothingSlots.find(slot);
			if (searchResult != customClothingSlots.end()) {
				return searchResult->second;
			}

			return nullptr;
		}

		void setClothingSlotData(std::shared_ptr<TES3::ClothingSlotData> data) {
			customClothingSlots[data->slot] = data;
		}

		static std::map<int, std::shared_ptr<TES3::ArmorSlotData>> customArmorSlots;

		std::shared_ptr<TES3::ArmorSlotData> getArmorSlotData(int slot) {
			auto searchResult = customArmorSlots.find(slot);
			if (searchResult != customArmorSlots.end()) {
				return searchResult->second;
			}

			return nullptr;
		}

		void setArmorSlotData(std::shared_ptr<TES3::ArmorSlotData> data) {
			customArmorSlots[data->slot] = data;
		}

		TES3::Reference* exteriorRefs[9] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };

		void clearExteriorRefs() {
			for (size_t i = 0; i < 9; i++) {
				exteriorRefs[i] = NULL;
			}
		}

		const auto TES3_newGame = reinterpret_cast<void(__stdcall *)()>(0x5FAEA0);
		void startNewGame() {
			// Call our load event.
			mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
			if (mwse::lua::event::LoadGameEvent::getEventEnabled()) {
				luaManager.getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::LoadGameEvent(NULL, false, true));
			}

			// Call original function.
			TES3_newGame();

			// Clear any timers.
			TES3::DataHandler::previousVisitedCell = nullptr;
			luaManager.clearTimers();

			// Call our post-load event.
			if (mwse::lua::event::LoadedGameEvent::getEventEnabled()) {
				luaManager.getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::LoadedGameEvent(NULL, false, true));
			}
		}

		const auto TES3_restInterruptHour = reinterpret_cast<int*>(0x7BC068);
		const auto TES3_restInterruptCreatures = reinterpret_cast<int*>(0x7D7530);

		int getRestHoursInterrupted() {
			return *TES3_restInterruptHour;
		}

		void setRestHoursInterrupted(int hour) {
			if (hour < -1) {
				hour = -1;
			}

			*TES3_restInterruptHour = hour;
		}

		int getRestInterruptCount() {
			return *TES3_restInterruptCreatures;
		}

		void setRestInterruptCount(int count) {
			*TES3_restInterruptCreatures = count;
		}

		const auto TES3_ResolveAssetPath = reinterpret_cast<int(__cdecl*)(const char*, char*)>(0x47A960);
		int resolveAssetPath(const char* path, char * out_buffer) {
			return TES3_ResolveAssetPath(path, out_buffer);
		}

		const auto TES3_getThreadSafeStringBuffer = reinterpret_cast<char* (__thiscall*)(char*)>(0x4D51B0);
		char* getThreadSafeStringBuffer() {
			return TES3_getThreadSafeStringBuffer(reinterpret_cast<char*>(0x7CB478));
		}

		const auto TES3_testLineOfSight = reinterpret_cast<bool (__cdecl*)(TES3::Vector3*, float, TES3::Vector3*, float)>(0x53B200);
		bool testLineOfSight(TES3::Vector3* pos1, float height1, TES3::Vector3* pos2, float height2) {
			return TES3_testLineOfSight(pos1, height1, pos2, height2);
		}

		const auto TES3_rand = reinterpret_cast<int(__cdecl*)(int)>(0x47B3B0);
		int rand(unsigned int arg0) {
			return TES3_rand(arg0);
		}
	}
}
