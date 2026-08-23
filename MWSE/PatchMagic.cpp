#include "PatchMagic.h"

#include "CodePatchUtil.h"

#include "TES3Actor.h"
#include "TES3DataHandler.h"
#include "TES3Inventory.h"
#include "TES3ItemData.h"
#include "TES3MagicEffect.h"
#include "TES3MagicEffectController.h"
#include "TES3MagicEffectInstance.h"
#include "TES3MagicSourceInstance.h"
#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse::patch::magic {
	//
	// Patch: Allow bound armour function to also summon bracers and pauldrons.
	//

	const auto TES3_SwapBoundArmor = reinterpret_cast<bool(__cdecl*)(TES3::MagicEffectInstance*, const char*, const char*)>(0x465DE0);
	const auto TES3_UI_PostAddAndEquipBoundItem = reinterpret_cast<void(__cdecl*)(TES3::Item*, TES3::ItemData*, int)>(0x5D1F00);

	TES3::EquipmentStack* createEquipBoundItem(TES3::Item* item, TES3::Actor* actor, TES3::MobileActor* mobile) {
		// Create and equip bound item. Excerpted from bound gauntlet code.
		TES3::EquipmentStack* equipped = nullptr;
		auto itemData = TES3::ItemData::createForBoundItem(item);

		actor->inventory.addItem(mobile, item, 1, false, &itemData);
		actor->equipItem(item, itemData, &equipped, mobile);
		if (mobile->actorType == TES3::MobileActorType::Player) {
			TES3_UI_PostAddAndEquipBoundItem(item, itemData, 1);
		}
		mobile->wearItem(item, itemData, false, false, true);
		return equipped->canonicalCopy();
	}

	bool __cdecl PatchSwapBoundArmor(TES3::MagicEffectInstance* effectInstance, const char* armorId1, const char* armorId2) {
		auto records = TES3::DataHandler::get()->nonDynamicData;

		auto armor1 = static_cast<TES3::Armor*>(records->resolveObject(armorId1));
		auto armor2 = armorId2 ? static_cast<TES3::Armor*>(records->resolveObject(armorId2)) : nullptr;

		if (armor1 == nullptr || armor1->objectType != TES3::ObjectType::Armor) {
			return false;
		}

		if (armor1->slot == TES3::ArmorSlot::LeftBracer) {
			auto mobile = effectInstance->target->getAttachedMobileActor();
			auto actor = static_cast<TES3::Actor*>(effectInstance->target->baseObject);
			auto mcpGlovesWithBracers = mcp::getFeatureEnabled(mcp::feature::AllowGlovesWithBracers);

			// Left hand.
			// Un-equip and memorize any item in the same location.
			auto equipLeftHand = actor->getEquippedArmorBySlot(TES3::ArmorSlot::LeftGauntlet);
			if (!equipLeftHand) {
				equipLeftHand = actor->getEquippedArmorBySlot(TES3::ArmorSlot::LeftBracer);
			}
			if (!equipLeftHand && !mcpGlovesWithBracers) {
				equipLeftHand = actor->getEquippedClothingBySlot(TES3::ClothingSlot::LeftGlove);
			}

			if (equipLeftHand) {
				// The original left hand item is memorized in the lastUsedArmor member.
				effectInstance->lastUsedArmor = equipLeftHand->canonicalCopy();
				if (equipLeftHand->object == mobile->currentEnchantedItem.object) {
					effectInstance->lastUsedEnchItem = mobile->currentEnchantedItem.canonicalCopy();
				}
			}

			// Create bound item and record created stack.
			effectInstance->createdData.equipmentOrSummon = createEquipBoundItem(armor1, actor, mobile);

			// Right hand.
			if (armor2) {
				// Un-equip and memorize any item in the same location.
				auto equipRightHand = actor->getEquippedArmorBySlot(TES3::ArmorSlot::RightGauntlet);
				if (!equipRightHand) {
					equipRightHand = actor->getEquippedArmorBySlot(TES3::ArmorSlot::RightBracer);
				}
				if (!equipRightHand && !mcpGlovesWithBracers) {
					equipRightHand = actor->getEquippedClothingBySlot(TES3::ClothingSlot::RightGlove);
				}

				if (equipRightHand) {
					// The original right hand item is memorized in the lastUsedWeapon member.
					effectInstance->lastUsedWeapon = equipRightHand->canonicalCopy();
					if (equipRightHand->object == mobile->currentEnchantedItem.object) {
						effectInstance->lastUsedEnchItem = mobile->currentEnchantedItem.canonicalCopy();
					}
				}

				// Create bound item and record created stack.
				effectInstance->createdData2 = createEquipBoundItem(armor2, actor, mobile);
			}

			return true;
		}
		else if (armor1->slot == TES3::ArmorSlot::LeftPauldron) {
			auto mobile = effectInstance->target->getAttachedMobileActor();
			auto actor = static_cast<TES3::Actor*>(effectInstance->target->baseObject);

			// Left shoulder.
			// Un-equip and memorize any item in the same location.
			auto equipLeftPauldron = actor->getEquippedArmorBySlot(TES3::ArmorSlot::LeftPauldron);
			if (equipLeftPauldron) {
				// The original left side item is memorized in the lastUsedArmor member.
				effectInstance->lastUsedArmor = equipLeftPauldron->canonicalCopy();
				if (equipLeftPauldron->object == mobile->currentEnchantedItem.object) {
					effectInstance->lastUsedEnchItem = mobile->currentEnchantedItem.canonicalCopy();
				}
			}

			// Create bound item and record created stack.
			effectInstance->createdData.equipmentOrSummon = createEquipBoundItem(armor1, actor, mobile);

			// Right shoulder.
			if (armor2) {
				// Un-equip and memorize any item in the same location.
				auto equipRightPauldron = actor->getEquippedArmorBySlot(TES3::ArmorSlot::RightPauldron);
				if (equipRightPauldron) {
					// The original right side item is memorized in the lastUsedWeapon member.
					effectInstance->lastUsedWeapon = equipRightPauldron->canonicalCopy();
					if (equipRightPauldron->object == mobile->currentEnchantedItem.object) {
						effectInstance->lastUsedEnchItem = mobile->currentEnchantedItem.canonicalCopy();
					}
				}

				// Create bound item and record created stack.
				effectInstance->createdData2 = createEquipBoundItem(armor2, actor, mobile);
			}

			return true;
		}
		else {
			// Use original code for all other slots.
			return TES3_SwapBoundArmor(effectInstance, armorId1, armorId2);
		}
	}

	//
	// Patch: Set ActiveMagicEffect.isIllegalSummon correctly on loading a savegame.
	//

	// Patches ActiveMagicManager::addLoadedMagicSourceInstance.
	__declspec(naked) void PatchLoadActiveMagicEffect() {
		__asm {
			mov edx, eax
			shr eax, 4				// eax >>= HarmfulBit
			and al, 1
			mov[esp + 0x2C], al		// activeMagicEffect.isHarmful = al
			shr edx, 15				// edx >>= IllegalDaedraBit
			and dl, 1
			mov[esp + 0x2D], dl		// activeMagicEffect.isIllegalSummon = dl
			mov ecx, esi
			call $ + 0x42763		// call MagicSourceCombo__getEffects
			mov cx, [eax + ebp + 0xC]	// cx = effect[ebp]->duration
			mov[esp + 0x2E], cx		// activeMagicEffect.duration = cx
			mov dx, [eax + ebp + 0x10]	// dx = effect[ebp]->magnitudeMin
			mov[esp + 0x30], dx		// activeMagicEffect.magnitudeMin = dx
			nop
			nop
		}
	}
	const size_t PatchLoadActiveMagicEffect_size = 0x32;

	//
	// Patch: Prevent crash with magic effects on invalid targets.
	//

	template <DWORD effectTickFunc>
	static void __cdecl PatchMagicEffect_RequireMobile(TES3::MagicSourceInstance* sourceInstance, float deltaTime, TES3::MagicEffectInstance* effectInstance, int effectIndex) {
		auto mobile = effectInstance->target->getAttachedMobileActor();
		if (mobile == nullptr) {
			return;
		}

		const auto fn = reinterpret_cast<TES3::MagicEffectController::spellEffectTickFunction>(effectTickFunc);
		fn(sourceInstance, deltaTime, effectInstance, effectIndex);
	}

	template <DWORD effectTickFunc>
	inline static void WritePatchMagicEffect_RequireMobile(TES3::EffectID::EffectID effectId) {
		se::memory::writeDoubleWordEnforced(0x7884B0 + (effectId * 4), effectTickFunc, reinterpret_cast<DWORD>(PatchMagicEffect_RequireMobile<effectTickFunc>));
	}

	//
	// Patch: Ensure that losing Stunted Magicka doesn't remove the flag permanently.
	//

	static void __cdecl PatchMagicEffectStuntedMagicka(TES3::MagicSourceInstance* sourceInstance, float deltaTime, TES3::MagicEffectInstance* effectInstance, int effectIndex) {
		auto mobile = effectInstance->target->getAttachedMobileActor();
		if (mobile == nullptr) {
			return;
		}

		const auto magicEffectController = TES3::DataHandler::get()->nonDynamicData->magicEffects;
		const auto appliesOnce = TES3::MagicEffectController::getEffectFlag(TES3::EffectID::StuntedMagicka, TES3::EffectFlag::AppliedOnceBit);
		unsigned int attributeVariant = 0;
		TES3::MagicEffectController::spellEffectEvent(sourceInstance, deltaTime, effectInstance, effectIndex, true, appliesOnce, &attributeVariant, 0x7886F0, TES3::EffectAttribute::NonResistable, nullptr);
		if (attributeVariant == 0) {
			return;
		}

		// Re-flag stunted magicka bit based on any other effects.
		const auto stillStunted = mobile->isAffectedByEffect(TES3::EffectID::StuntedMagicka);
		mobile->setMobileActorFlag(TES3::MobileActorFlag::StuntedMagicka, stillStunted);
	}

	//
	// Patch: Fix cure spells incorrectly triggering MagicEffectState_Ending for magic that hasn't taken effect yet.
	//

	__declspec(naked) void PatchRemoveMagicsByEffect() {
		__asm {
			cmp byte ptr[esp + 0x4C], 5		// if (magicEffectInstance.state == MagicEffectState_Working)
			jnz done
			mov byte ptr[esp + 0x4C], 6		// magicEffectInstance.state = MagicEffectState_Ending
			done:
			ret
		}
	}

	//
	// Patch: Fix loading crashes where there are links to missing objects from mods that were removed.
	//

	// Prevent crashes when a casting item is no longer present.
	__declspec(naked) void PatchMagicSourceInstanceDtor() {
		__asm {
			test edi, edi						// if (!castingItem)
			__asm _emit 0x74 __asm _emit 0x45	// jz short $ + 0x47 (assembler can't output short offsets correctly)
			lea esi, [edi + 4]					// esi = &castingItem.objectType
				mov eax, [esi]						// eax = castingItem.objectType
				nop
		}
	}
	const size_t PatchMagicSourceInstanceDtor_size = 0xA;

	// Prevent deleting itemData when a soul trapped creature is no longer present.
	__declspec(naked) void PatchSoulTrappedCreatureNotFound1() {
		__asm {
			add esp, 0x8
			__asm _emit 0xEB __asm _emit 0x0D	// jmp short $ + 0xF (assembler can't output short offsets correctly)
		}
	}
	const size_t PatchSoulTrappedCreatureNotFound1_size = 0x5;

	__declspec(naked) void PatchSoulTrappedCreatureNotFound2() {
		__asm {
			add esp, 0x8
			__asm _emit 0xEB __asm _emit 0x09	// jmp short $ + 0xB (assembler can't output short offsets correctly)
		}
	}
	const size_t PatchSoulTrappedCreatureNotFound2_size = 0x5;

	__declspec(naked) void PatchSoulTrappedCreatureNotFound3() {
		__asm {
			add esp, 0x8
			__asm _emit 0xEB __asm _emit 0x17	// jmp short $ + 0x19 (assembler can't output short offsets correctly)
		}
	}
	const size_t PatchSoulTrappedCreatureNotFound3_size = 0x5;

	// Prevent crashes when the actor holding a casting item is no longer present.
	// The engine computes &actor->inventory without checking that the actor from the save's
	// target reference id resolved. Returning no result makes loading take the normal
	// invalid-spell path ("Failed to load spell ...") instead of crashing.
	static TES3::ItemStack* __fastcall PatchFindRestoredCastingItemEntry(TES3::Inventory* inventory, DWORD _EDX_, TES3::Object* item) {
		if (reinterpret_cast<DWORD>(inventory) == offsetof(TES3::Actor, inventory)) {
			return nullptr;
		}
		return inventory->findItemStack(item);
	}

	//
	// Patch: Fix enchantment copying on books and weapons.
	//

	__declspec(naked) void PatchCopyBookEnchantmentCaller() {
		__asm {
			push ebp
			mov ecx, ebx
		}
	}
	__declspec(naked) void PatchCopyWeaponEnchantmentCaller() {
		__asm {
			push ebx
			mov ecx, ebp
		}
	}
	constexpr size_t PatchCopyEnchantmentCaller_size = 0x3;

	void __fastcall PatchCopyEnchantment(TES3::Item* item, DWORD _EDX_, const TES3::Item* from) {
		// Free existing enchantment ID string if available.
		if (item->getEnchantment() && !item->getLinksResolved()) {
			se::memory::free(item->getEnchantment());
		}
		item->setEnchantment(nullptr);

		if (from->getEnchantment()) {
			if (from->getLinksResolved()) {
				item->setEnchantment(from->getEnchantment());
			}
			else {
				// Helper union so we don't have to reinterpret memory all the time.
				union EnchantUnion { TES3::Enchantment* enchantment; char* id; };
				EnchantUnion toEnchantment = {}, fromEnchantment = {};

				// Make a copy of the enchantment's ID.
				fromEnchantment.enchantment = from->getEnchantment();
				const auto enchantmentIDLength = strnlen_s(fromEnchantment.id, 32) + 1;
				toEnchantment.id = reinterpret_cast<char*>(se::memory::malloc(enchantmentIDLength));
				strncpy_s(toEnchantment.id, enchantmentIDLength, fromEnchantment.id, enchantmentIDLength);
				item->setEnchantment(toEnchantment.enchantment);
			}
		}
	}

	void install() {
		using se::memory::genCallEnforced;
		using se::memory::genCallUnprotected;
		using se::memory::genNOPUnprotected;
		using se::memory::writeDoubleWordEnforced;
		using se::memory::writePatchCodeUnprotected;

		// Patch: Allow bound armour function to also summon bracers and pauldrons.
		genCallEnforced(0x466457, 0x465DE0, reinterpret_cast<DWORD>(PatchSwapBoundArmor));

		// Patch: Set ActiveMagicEffect.isIllegalSummon correctly on loading a savegame.
		writePatchCodeUnprotected(0x454826, (BYTE*)&PatchLoadActiveMagicEffect, PatchLoadActiveMagicEffect_size);

		// Patch: Prevent crash with magic effects on invalid targets.
		{
			WritePatchMagicEffect_RequireMobile<0x45F170>(TES3::EffectID::WaterBreathing);
			WritePatchMagicEffect_RequireMobile<0x45F1D0>(TES3::EffectID::SwiftSwim);
			WritePatchMagicEffect_RequireMobile<0x45F230>(TES3::EffectID::WaterWalking);
			WritePatchMagicEffect_RequireMobile<0x45F2B0>(TES3::EffectID::Shield);
			WritePatchMagicEffect_RequireMobile<0x45F310>(TES3::EffectID::FireShield);
			WritePatchMagicEffect_RequireMobile<0x45F410>(TES3::EffectID::LightningShield);
			WritePatchMagicEffect_RequireMobile<0x45F390>(TES3::EffectID::FrostShield);
			WritePatchMagicEffect_RequireMobile<0x45F490>(TES3::EffectID::Burden);
			WritePatchMagicEffect_RequireMobile<0x45F5C0>(TES3::EffectID::Feather);
			WritePatchMagicEffect_RequireMobile<0x45F6F0>(TES3::EffectID::Jump);
			WritePatchMagicEffect_RequireMobile<0x45F750>(TES3::EffectID::Levitate);
			WritePatchMagicEffect_RequireMobile<0x45F840>(TES3::EffectID::SlowFall);
			//WritePatchMagicEffect_RequireMobile<0x45F9A0>(TES3::EffectID::Lock);
			//WritePatchMagicEffect_RequireMobile<0x45FB40>(TES3::EffectID::Open);
			WritePatchMagicEffect_RequireMobile<0x45FF20>(TES3::EffectID::FireDamage);
			WritePatchMagicEffect_RequireMobile<0x45FF80>(TES3::EffectID::ShockDamage);
			WritePatchMagicEffect_RequireMobile<0x45FFE0>(TES3::EffectID::FrostDamage);
			WritePatchMagicEffect_RequireMobile<0x460040>(TES3::EffectID::DrainAttribute);
			WritePatchMagicEffect_RequireMobile<0x460120>(TES3::EffectID::DrainHealth);
			WritePatchMagicEffect_RequireMobile<0x460180>(TES3::EffectID::DrainMagicka);
			WritePatchMagicEffect_RequireMobile<0x460300>(TES3::EffectID::DrainFatigue);
			WritePatchMagicEffect_RequireMobile<0x460240>(TES3::EffectID::DrainSkill);
			WritePatchMagicEffect_RequireMobile<0x460040>(TES3::EffectID::DamageAttribute);
			WritePatchMagicEffect_RequireMobile<0x460120>(TES3::EffectID::DamageHealth);
			WritePatchMagicEffect_RequireMobile<0x460180>(TES3::EffectID::DamageMagicka);
			WritePatchMagicEffect_RequireMobile<0x460300>(TES3::EffectID::DamageFatigue);
			WritePatchMagicEffect_RequireMobile<0x460240>(TES3::EffectID::DamageSkill);
			WritePatchMagicEffect_RequireMobile<0x4603C0>(TES3::EffectID::Poison);
			WritePatchMagicEffect_RequireMobile<0x460420>(TES3::EffectID::WeaknessToFire);
			WritePatchMagicEffect_RequireMobile<0x460480>(TES3::EffectID::WeaknessToFrost);
			WritePatchMagicEffect_RequireMobile<0x4604E0>(TES3::EffectID::WeaknessToShock);
			WritePatchMagicEffect_RequireMobile<0x460540>(TES3::EffectID::WeaknessToMagicka);
			WritePatchMagicEffect_RequireMobile<0x4605A0>(TES3::EffectID::WeaknessToCommonDisease);
			WritePatchMagicEffect_RequireMobile<0x460600>(TES3::EffectID::WeaknessToBlightDisease);
			WritePatchMagicEffect_RequireMobile<0x460660>(TES3::EffectID::WeaknessToCorprus);
			WritePatchMagicEffect_RequireMobile<0x4606C0>(TES3::EffectID::WeaknessToPoison);
			WritePatchMagicEffect_RequireMobile<0x460720>(TES3::EffectID::WeaknessToNormalWeapons);
			WritePatchMagicEffect_RequireMobile<0x460780>(TES3::EffectID::DisintegrateWeapon);
			WritePatchMagicEffect_RequireMobile<0x4608C0>(TES3::EffectID::DisintegrateArmor);
			WritePatchMagicEffect_RequireMobile<0x460D30>(TES3::EffectID::Invisibility);
			WritePatchMagicEffect_RequireMobile<0x460BE0>(TES3::EffectID::Chameleon);
			WritePatchMagicEffect_RequireMobile<0x460ED0>(TES3::EffectID::Light);
			WritePatchMagicEffect_RequireMobile<0x460E70>(TES3::EffectID::Sanctuary);
			WritePatchMagicEffect_RequireMobile<0x4610F0>(TES3::EffectID::NightEye);
			WritePatchMagicEffect_RequireMobile<0x4611F0>(TES3::EffectID::Charm);
			WritePatchMagicEffect_RequireMobile<0x461350>(TES3::EffectID::Paralyze);
			WritePatchMagicEffect_RequireMobile<0x461490>(TES3::EffectID::Silence);
			WritePatchMagicEffect_RequireMobile<0x4614F0>(TES3::EffectID::Blind);
			WritePatchMagicEffect_RequireMobile<0x461690>(TES3::EffectID::Sound);
			WritePatchMagicEffect_RequireMobile<0x461800>(TES3::EffectID::CalmHumanoid);
			WritePatchMagicEffect_RequireMobile<0x4619E0>(TES3::EffectID::CalmCreature);
			WritePatchMagicEffect_RequireMobile<0x461890>(TES3::EffectID::FrenzyHumanoid);
			WritePatchMagicEffect_RequireMobile<0x461A70>(TES3::EffectID::FrenzyCreature);
			WritePatchMagicEffect_RequireMobile<0x461970>(TES3::EffectID::DemoralizeHumanoid);
			WritePatchMagicEffect_RequireMobile<0x461B50>(TES3::EffectID::DemoralizeCreature);
			WritePatchMagicEffect_RequireMobile<0x461900>(TES3::EffectID::RallyHumanoid);
			WritePatchMagicEffect_RequireMobile<0x461AE0>(TES3::EffectID::RallyCreature);
			WritePatchMagicEffect_RequireMobile<0x461CC0>(TES3::EffectID::Dispel);
			WritePatchMagicEffect_RequireMobile<0x463270>(TES3::EffectID::SoulTrap);
			WritePatchMagicEffect_RequireMobile<0x4634D0>(TES3::EffectID::Telekinesis);
			WritePatchMagicEffect_RequireMobile<0x463580>(TES3::EffectID::Mark);
			WritePatchMagicEffect_RequireMobile<0x463650>(TES3::EffectID::Recall);
			WritePatchMagicEffect_RequireMobile<0x463820>(TES3::EffectID::DivineIntervention);
			WritePatchMagicEffect_RequireMobile<0x463900>(TES3::EffectID::AlmsiviIntervention);
			WritePatchMagicEffect_RequireMobile<0x4639E0>(TES3::EffectID::DetectAnimal);
			WritePatchMagicEffect_RequireMobile<0x463A90>(TES3::EffectID::DetectEnchantment);
			WritePatchMagicEffect_RequireMobile<0x463B10>(TES3::EffectID::DetectKey);
			WritePatchMagicEffect_RequireMobile<0x463B90>(TES3::EffectID::SpellAbsorption);
			WritePatchMagicEffect_RequireMobile<0x463B90>(TES3::EffectID::Reflect);
			WritePatchMagicEffect_RequireMobile<0x461BC0>(TES3::EffectID::CureCommonDisease);
			WritePatchMagicEffect_RequireMobile<0x461C40>(TES3::EffectID::CureBlightDisease);
			WritePatchMagicEffect_RequireMobile<0x461DC0>(TES3::EffectID::CureCorprus);
			WritePatchMagicEffect_RequireMobile<0x461EC0>(TES3::EffectID::CurePoison);
			WritePatchMagicEffect_RequireMobile<0x461E40>(TES3::EffectID::CureParalyzation);
			WritePatchMagicEffect_RequireMobile<0x461F40>(TES3::EffectID::RestoreAttribute);
			WritePatchMagicEffect_RequireMobile<0x462030>(TES3::EffectID::RestoreHealth);
			WritePatchMagicEffect_RequireMobile<0x4620B0>(TES3::EffectID::RestoreMagicka);
			WritePatchMagicEffect_RequireMobile<0x462180>(TES3::EffectID::RestoreFatigue);
			WritePatchMagicEffect_RequireMobile<0x462250>(TES3::EffectID::RestoreSkill);
			WritePatchMagicEffect_RequireMobile<0x462330>(TES3::EffectID::FortifyAttribute);
			WritePatchMagicEffect_RequireMobile<0x462410>(TES3::EffectID::FortifyHealth);
			WritePatchMagicEffect_RequireMobile<0x462470>(TES3::EffectID::FortifyMagicka);
			WritePatchMagicEffect_RequireMobile<0x462530>(TES3::EffectID::FortifyFatigue);
			WritePatchMagicEffect_RequireMobile<0x4625F0>(TES3::EffectID::FortifySkill);
			WritePatchMagicEffect_RequireMobile<0x4626E0>(TES3::EffectID::FortifyMagickaMultiplier);
			WritePatchMagicEffect_RequireMobile<0x4627F0>(TES3::EffectID::AbsorbAttribute);
			WritePatchMagicEffect_RequireMobile<0x462940>(TES3::EffectID::AbsorbHealth);
			WritePatchMagicEffect_RequireMobile<0x462A00>(TES3::EffectID::AbsorbMagicka);
			WritePatchMagicEffect_RequireMobile<0x462B60>(TES3::EffectID::AbsorbFatigue);
			WritePatchMagicEffect_RequireMobile<0x462CC0>(TES3::EffectID::AbsorbSkill);
			WritePatchMagicEffect_RequireMobile<0x462E00>(TES3::EffectID::ResistFire);
			WritePatchMagicEffect_RequireMobile<0x462E60>(TES3::EffectID::ResistFrost);
			WritePatchMagicEffect_RequireMobile<0x462EC0>(TES3::EffectID::ResistShock);
			WritePatchMagicEffect_RequireMobile<0x462F20>(TES3::EffectID::ResistMagicka);
			WritePatchMagicEffect_RequireMobile<0x462F80>(TES3::EffectID::ResistCommonDisease);
			WritePatchMagicEffect_RequireMobile<0x462FE0>(TES3::EffectID::ResistBlightDisease);
			WritePatchMagicEffect_RequireMobile<0x463040>(TES3::EffectID::ResistCorprus);
			WritePatchMagicEffect_RequireMobile<0x4630A0>(TES3::EffectID::ResistPoison);
			WritePatchMagicEffect_RequireMobile<0x463100>(TES3::EffectID::ResistNormalWeapons);
			WritePatchMagicEffect_RequireMobile<0x463160>(TES3::EffectID::ResistParalysis);
			WritePatchMagicEffect_RequireMobile<0x461D40>(TES3::EffectID::RemoveCurse);
			WritePatchMagicEffect_RequireMobile<0x4631C0>(TES3::EffectID::TurnUndead);
			WritePatchMagicEffect_RequireMobile<0x463E00>(TES3::EffectID::SummonScamp);
			WritePatchMagicEffect_RequireMobile<0x463E30>(TES3::EffectID::SummonClannfear);
			WritePatchMagicEffect_RequireMobile<0x463E60>(TES3::EffectID::SummonDaedroth);
			WritePatchMagicEffect_RequireMobile<0x463E90>(TES3::EffectID::SummonDremora);
			WritePatchMagicEffect_RequireMobile<0x463EC0>(TES3::EffectID::SummonGhost);
			WritePatchMagicEffect_RequireMobile<0x463EF0>(TES3::EffectID::SummonSkeleton);
			WritePatchMagicEffect_RequireMobile<0x463F20>(TES3::EffectID::SummonLeastBonewalker);
			WritePatchMagicEffect_RequireMobile<0x463F50>(TES3::EffectID::SummonGreaterBonewalker);
			WritePatchMagicEffect_RequireMobile<0x463F80>(TES3::EffectID::SummonBonelord);
			WritePatchMagicEffect_RequireMobile<0x463FB0>(TES3::EffectID::SummonTwilight);
			WritePatchMagicEffect_RequireMobile<0x463FE0>(TES3::EffectID::SummonHunger);
			WritePatchMagicEffect_RequireMobile<0x464010>(TES3::EffectID::SummonGoldenSaint);
			WritePatchMagicEffect_RequireMobile<0x464040>(TES3::EffectID::SummonFlameAtronach);
			WritePatchMagicEffect_RequireMobile<0x464070>(TES3::EffectID::SummonFrostAtronach);
			WritePatchMagicEffect_RequireMobile<0x4640A0>(TES3::EffectID::SummonStormAtronach);
			WritePatchMagicEffect_RequireMobile<0x464220>(TES3::EffectID::FortifyAttackBonus);
			WritePatchMagicEffect_RequireMobile<0x463490>(TES3::EffectID::CommandCreature);
			WritePatchMagicEffect_RequireMobile<0x4634B0>(TES3::EffectID::CommandHumanoid);
			WritePatchMagicEffect_RequireMobile<0x463BE0>(TES3::EffectID::BoundDagger);
			WritePatchMagicEffect_RequireMobile<0x463C10>(TES3::EffectID::BoundLongsword);
			WritePatchMagicEffect_RequireMobile<0x463C40>(TES3::EffectID::BoundMace);
			WritePatchMagicEffect_RequireMobile<0x463C70>(TES3::EffectID::BoundBattleAxe);
			WritePatchMagicEffect_RequireMobile<0x463CA0>(TES3::EffectID::BoundSpear);
			WritePatchMagicEffect_RequireMobile<0x463CD0>(TES3::EffectID::BoundLongbow);
			WritePatchMagicEffect_RequireMobile<0x464C90>(TES3::EffectID::ExtraSpell);
			WritePatchMagicEffect_RequireMobile<0x463D00>(TES3::EffectID::BoundCuirass);
			WritePatchMagicEffect_RequireMobile<0x463D30>(TES3::EffectID::BoundHelm);
			WritePatchMagicEffect_RequireMobile<0x463D60>(TES3::EffectID::BoundBoots);
			WritePatchMagicEffect_RequireMobile<0x463D90>(TES3::EffectID::BoundShield);
			WritePatchMagicEffect_RequireMobile<0x463DC0>(TES3::EffectID::BoundGloves);
			WritePatchMagicEffect_RequireMobile<0x464490>(TES3::EffectID::Corprus);
			WritePatchMagicEffect_RequireMobile<0x464280>(TES3::EffectID::Vampirism);
			WritePatchMagicEffect_RequireMobile<0x4640D0>(TES3::EffectID::SummonCenturionSphere);
			WritePatchMagicEffect_RequireMobile<0x464BB0>(TES3::EffectID::SunDamage);
			WritePatchMagicEffect_RequireMobile<0x464100>(TES3::EffectID::SummonFabricant);
			WritePatchMagicEffect_RequireMobile<0x464130>(TES3::EffectID::SummonWolf);
			WritePatchMagicEffect_RequireMobile<0x464160>(TES3::EffectID::SummonBear);
			WritePatchMagicEffect_RequireMobile<0x464190>(TES3::EffectID::SummonBoneWolf);
			WritePatchMagicEffect_RequireMobile<0x4641C0>(TES3::EffectID::Summon04);
			WritePatchMagicEffect_RequireMobile<0x4641F0>(TES3::EffectID::Summon05);
		}

		// Other magic effect patches.
		writeDoubleWordEnforced(0x7884B0 + (TES3::EffectID::StuntedMagicka * 4), 0x464F20, reinterpret_cast<DWORD>(PatchMagicEffectStuntedMagicka));

		// Patch: Fix cure spells incorrectly triggering MagicEffectState_Ending for magic that hasn't taken effect yet.
		genCallUnprotected(0x4559B2, reinterpret_cast<DWORD>(PatchRemoveMagicsByEffect), 0x8);

		// Patch: Fix loading crashes where there are links to missing objects from mods that were removed.
		writePatchCodeUnprotected(0x512485, (BYTE*)&PatchMagicSourceInstanceDtor, PatchMagicSourceInstanceDtor_size);
		writePatchCodeUnprotected(0x49DEE1, (BYTE*)&PatchSoulTrappedCreatureNotFound1, PatchSoulTrappedCreatureNotFound1_size);
		writePatchCodeUnprotected(0x4A4BEC, (BYTE*)&PatchSoulTrappedCreatureNotFound2, PatchSoulTrappedCreatureNotFound2_size);
		writePatchCodeUnprotected(0x4D8DD7, (BYTE*)&PatchSoulTrappedCreatureNotFound3, PatchSoulTrappedCreatureNotFound3_size);
		genCallEnforced(0x51428D, 0x49A6C0, reinterpret_cast<DWORD>(PatchFindRestoredCastingItemEntry));

		// Patch: Fix book enchantment copying.
		genNOPUnprotected(0x4A2618, 0x4A26D8 - 0x4A2618);
		writePatchCodeUnprotected(0x4A2618, (BYTE*)&PatchCopyBookEnchantmentCaller, PatchCopyEnchantmentCaller_size);
		genCallUnprotected(0x4A2618 + PatchCopyEnchantmentCaller_size, reinterpret_cast<DWORD>(PatchCopyEnchantment));

		// Patch: Fix weapon enchantment copying.
		genNOPUnprotected(0x4F26FF, 0x4F27BC - 0x4F26FF);
		writePatchCodeUnprotected(0x4F26FF, (BYTE*)&PatchCopyWeaponEnchantmentCaller, PatchCopyEnchantmentCaller_size);
		genCallUnprotected(0x4F26FF + PatchCopyEnchantmentCaller_size, reinterpret_cast<DWORD>(PatchCopyEnchantment));
	}
}
