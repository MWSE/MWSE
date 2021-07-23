#include "TES3Alchemy.h"

#include "MemoryUtil.h"
#include "TES3DataHandler.h"
#include "TES3MagicEffectController.h"
#include "TES3Util.h"

namespace TES3 {
	Alchemy::Alchemy() {
		ctor();
	}

	Alchemy::~Alchemy() {
		dtor();
	}

	const auto TES3_Alchemy_ctor = reinterpret_cast<TES3::Alchemy * (__thiscall*)(TES3::Alchemy*)>(0x4ABA40);
	void Alchemy::ctor() {
		TES3_Alchemy_ctor(this);
	}

	const auto TES3_Alchemy_dtor = reinterpret_cast<void (__thiscall*)(TES3::Alchemy*)>(0x4ABB70);
	void Alchemy::dtor() {
		TES3_Alchemy_dtor(this);
	}

	const auto TES3_Alchemy_loadObjectSpecific = reinterpret_cast<bool(__thiscall*)(TES3::Alchemy*, TES3::GameFile*)>(0x4ABD90);
	bool Alchemy::loadObjectSpecific(TES3::GameFile* file) {
		bool success = TES3_Alchemy_loadObjectSpecific(this, file);
		if (success) {
			cleanUnusedAttributeSkillIds();
		}
		return success;
	}

	size_t Alchemy::getActiveEffectCount() {
		size_t count = 0;
		for (size_t i = 0; i < 8; i++) {
			if (effects[i].effectID != TES3::EffectID::None) {
				count++;
			}
		}
		return count;
	}

	int Alchemy::getFirstIndexOfEffect(int effectId) {
		for (size_t i = 0; i < 8; i++) {
			if (effects[i].effectID == effectId) {
				return i;
			}
		}
		return -1;
	}

	bool Alchemy::effectsMatchWith(const Alchemy * other) const {
		for (size_t i = 0; i < 8; i++) {
			if (effects[i].matchesEffectsWith(&other->effects[i])) {
				return false;
			}
		}
		return true;
	}

	void Alchemy::setIconPath(const char* path) {
		if (strnlen_s(path, 32) >= 32) {
			throw std::invalid_argument("Path must not be 32 or more characters.");
		}
		mwse::tes3::setDataString(&icon, path);
	}

	void Alchemy::cleanUnusedAttributeSkillIds() {
		auto mgefs = TES3::DataHandler::get()->nonDynamicData->magicEffects;
		for (auto& effect : effects) {
			if (effect.effectID == TES3::EffectID::None) {
				break;
			}

			auto effectFlags = mgefs->getEffectFlags(effect.effectID);
			if (!(effectFlags & TES3::EffectFlag::TargetSkill)) {
				effect.skillID = -1;
			}
			if (!(effectFlags & TES3::EffectFlag::TargetAttribute)) {
				effect.attributeID = -1;
			}
		}
	}

	std::reference_wrapper<Effect[8]> Alchemy::getEffects() {
		return std::ref(effects);
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Alchemy)
