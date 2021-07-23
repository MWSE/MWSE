#include "LuaDisableableEventManager.h"

#include "LuaManager.h"

#include "LuaActivateEvent.h"
#include "LuaActivationTargetChangedEvent.h"
#include "LuaAddSoundEvent.h"
#include "LuaAddTempSoundEvent.h"
#include "LuaAddTopicEvent.h"
#include "LuaAttackEvent.h"
#include "LuaAttackStartEvent.h"
#include "LuaBarterOfferEvent.h"
#include "LuaBodyPartAssignedEvent.h"
#include "LuaBodyPartsUpdatedEvent.h"
#include "LuaBookGetTextEvent.h"
#include "LuaButtonPressedEvent.h"
#include "LuaCalcArmorRatingEvent.h"
#include "LuaCalcBarterPriceEvent.h"
#include "LuaCalcHitArmorPieceEvent.h"
#include "LuaCalcHitChanceEvent.h"
#include "LuaCalcMovementSpeedEvent.h"
#include "LuaCalcRepairPriceEvent.h"
#include "LuaCalcRestInterruptEvent.h"
#include "LuaCalcSoulValueEvent.h"
#include "LuaCalcSpellPriceEvent.h"
#include "LuaCalcSunDamageScalarEvent.h"
#include "LuaCalcTrainingPriceEvent.h"
#include "LuaCalcTravelPriceEvent.h"
#include "LuaCellActivatedEvent.h"
#include "LuaCellChangedEvent.h"
#include "LuaCellDeactivatedEvent.h"
#include "LuaCombatStartedEvent.h"
#include "LuaCombatStartEvent.h"
#include "LuaCombatStopEvent.h"
#include "LuaCombatStoppedEvent.h"
#include "LuaContainerClosedEvent.h"
#include "LuaConvertReferenceToItemEvent.h"
#include "LuaCrimeWitnessedEvent.h"
#include "LuaDamagedEvent.h"
#include "LuaDamagedHandToHandEvent.h"
#include "LuaDamageEvent.h"
#include "LuaDamageHandToHandEvent.h"
#include "LuaDeathEvent.h"
#include "LuaDetectSneakEvent.h"
#include "LuaDetermineActionEvent.h"
#include "LuaDeterminedActionEvent.h"
#include "LuaDisarmTrapEvent.h"
#include "LuaEnchantedItemCreatedEvent.h"
#include "LuaEnchantedItemCreateFailedEvent.h"
#include "LuaEquipEvent.h"
#include "LuaEquippedEvent.h"
#include "LuaFilterBarterMenuEvent.h"
#include "LuaFilterContentsMenuEvent.h"
#include "LuaFilterInventoryEvent.h"
#include "LuaFilterInventorySelectEvent.h"
#include "LuaFilterSoulGemTargetEvent.h"
#include "LuaFrameEvent.h"
#include "LuaGenericUiActivatedEvent.h"
#include "LuaGenericUiPostEvent.h"
#include "LuaGenericUiPreEvent.h"
#include "LuaInfoFilterEvent.h"
#include "LuaInfoGetTextEvent.h"
#include "LuaInfoLinkResolveEvent.h"
#include "LuaInfoResponseEvent.h"
#include "LuaIsGuardEvent.h"
#include "LuaItemDroppedEvent.h"
#include "LuaItemTileUpdatedEvent.h"
#include "LuaJournalEvent.h"
#include "LuaKeyDownEvent.h"
#include "LuaKeyEvent.h"
#include "LuaKeyUpEvent.h"
#include "LuaLeveledCreaturePickedEvent.h"
#include "LuaLeveledItemPickedEvent.h"
#include "LuaLevelUpEvent.h"
#include "LuaLoadedGameEvent.h"
#include "LuaLoadGameEvent.h"
#include "LuaMagicCastedEvent.h"
#include "LuaMenuStateEvent.h"
#include "LuaMeshLoadedEvent.h"
#include "LuaMobileActorActivatedEvent.h"
#include "LuaMobileActorDeactivatedEvent.h"
#include "LuaMobileObjectCollisionEvent.h"
#include "LuaMobileObjectWaterImpactEvent.h"
#include "LuaMobileProjectileActorCollisionEvent.h"
#include "LuaMobileProjectileObjectCollisionEvent.h"
#include "LuaMobileProjectileTerrainCollisionEvent.h"
#include "LuaMouseAxisEvent.h"
#include "LuaMouseButtonDownEvent.h"
#include "LuaMouseButtonUpEvent.h"
#include "LuaMouseWheelEvent.h"
#include "LuaMusicSelectTrackEvent.h"
#include "LuaObjectInvalidatedEvent.h"
#include "LuaPickLockEvent.h"
#include "LuaPlayAnimationGroupEvent.h"
#include "LuaPlayItemSoundEvent.h"
#include "LuaPostInfoResponseEvent.h"
#include "LuaPotionBrewedEvent.h"
#include "LuaPotionBrewFailedEvent.h"
#include "LuaPowerRechargedEvent.h"
#include "LuaPreLevelUpEvent.h"
#include "LuaPreventRestEvent.h"
#include "LuaProjectileExpireEvent.h"
#include "LuaReferenceActivatedEvent.h"
#include "LuaReferenceDeactivatedEvent.h"
#include "LuaReferenceSceneNodeCreatedEvent.h"
#include "LuaRestInterruptEvent.h"
#include "LuaSavedGameEvent.h"
#include "LuaSaveGameEvent.h"
#include "LuaShowRestWaitMenuEvent.h"
#include "LuaSimulateEvent.h"
#include "LuaSkillExerciseEvent.h"
#include "LuaSkillRaisedEvent.h"
#include "LuaSoundObjectPlayEvent.h"
#include "LuaSpellCastedEvent.h"
#include "LuaSpellCastEvent.h"
#include "LuaSpellCreatedEvent.h"
#include "LuaSpellResistEvent.h"
#include "LuaSpellTickEvent.h"
#include "LuaUiObjectTooltipEvent.h"
#include "LuaUiRefreshedEvent.h"
#include "LuaUiSpellTooltipEvent.h"
#include "LuaUnequippedEvent.h"
#include "LuaWeaponReadiedEvent.h"
#include "LuaWeaponUnreadiedEvent.h"
#include "LuaWeatherChangedImmediateEvent.h"
#include "LuaWeatherCycledEvent.h"
#include "LuaWeatherTransitionFinishedEvent.h"
#include "LuaWeatherTransitionStartedEvent.h"

namespace mwse {
	namespace lua {
		namespace event {
			void DisableableEventManager::bindToLua() {
				// Get our lua state.
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;

				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<DisableableEventManager>("mwseDisableableEventManager");
				usertypeDefinition["new"] = sol::no_constructor;

				// Give access to the enabled state.
				usertypeDefinition["activate"] = sol::property(&ActivateEvent::getEventEnabled, &ActivateEvent::setEventEnabled);
				usertypeDefinition["activationTargetChanged"] = sol::property(&ActivationTargetChangedEvent::getEventEnabled, &ActivationTargetChangedEvent::setEventEnabled);
				usertypeDefinition["addSound"] = sol::property(&AddSoundEvent::getEventEnabled, &AddSoundEvent::setEventEnabled);
				usertypeDefinition["addTempSound"] = sol::property(&AddTempSoundEvent::getEventEnabled, &AddTempSoundEvent::setEventEnabled);
				usertypeDefinition["attack"] = sol::property(&AttackEvent::getEventEnabled, &AttackEvent::setEventEnabled);
				usertypeDefinition["attackStart"] = sol::property(&AttackStartEvent::getEventEnabled, &AttackStartEvent::setEventEnabled);
				usertypeDefinition["barterOffer"] = sol::property(&BarterOfferEvent::getEventEnabled, &BarterOfferEvent::setEventEnabled);
				usertypeDefinition["bodyPartAssigned"] = sol::property(&BodyPartAssignedEvent::getEventEnabled, &BodyPartAssignedEvent::setEventEnabled);
				usertypeDefinition["bodyPartsUpdated"] = sol::property(&BodyPartsUpdatedEvent::getEventEnabled, &BodyPartsUpdatedEvent::setEventEnabled);
				usertypeDefinition["bookGetText"] = sol::property(&BookGetTextEvent::getEventEnabled, &BookGetTextEvent::setEventEnabled);
				usertypeDefinition["buttonPressed"] = sol::property(&ButtonPressedEvent::getEventEnabled, &ButtonPressedEvent::setEventEnabled);
				usertypeDefinition["calcArmorPieceHit"] = sol::property(&CalcHitArmorPiece::getEventEnabled, &CalcHitArmorPiece::setEventEnabled);
				usertypeDefinition["calcArmorRating"] = sol::property(&CalculateArmorRatingEvent::getEventEnabled, &CalculateArmorRatingEvent::setEventEnabled);
				usertypeDefinition["calcBarterPrice"] = sol::property(&CalculateBarterPriceEvent::getEventEnabled, &CalculateBarterPriceEvent::setEventEnabled);
				usertypeDefinition["calcFlySpeed"] = sol::property(&CalculateMovementSpeed::getEventEnabled, &CalculateMovementSpeed::setEventEnabled);
				usertypeDefinition["calcHitChance"] = sol::property(&CalcHitChanceEvent::getEventEnabled, &CalcHitChanceEvent::setEventEnabled);
				usertypeDefinition["calcMoveSpeed"] = sol::property(&CalculateMovementSpeed::getEventEnabled, &CalculateMovementSpeed::setEventEnabled);
				usertypeDefinition["calcRepairPrice"] = sol::property(&CalculateRepairPriceEvent::getEventEnabled, &CalculateRepairPriceEvent::setEventEnabled);
				usertypeDefinition["calcRestInterrupt"] = sol::property(&CalcRestInterruptEvent::getEventEnabled, &CalcRestInterruptEvent::setEventEnabled);
				usertypeDefinition["calcRunSpeed"] = sol::property(&CalculateMovementSpeed::getEventEnabled, &CalculateMovementSpeed::setEventEnabled);
				usertypeDefinition["calcSoulValue"] = sol::property(&CalculateSoulValueEvent::getEventEnabled, &CalculateSoulValueEvent::setEventEnabled);
				usertypeDefinition["calcSpellPrice"] = sol::property(&CalculateSpellPriceEvent::getEventEnabled, &CalculateSpellPriceEvent::setEventEnabled);
				usertypeDefinition["calcSunDamageScalar"] = sol::property(&CalcSunDamageScalarEvent::getEventEnabled, &CalcSunDamageScalarEvent::setEventEnabled);
				usertypeDefinition["calcSwimRunSpeed"] = sol::property(&CalculateMovementSpeed::getEventEnabled, &CalculateMovementSpeed::setEventEnabled);
				usertypeDefinition["calcSwimSpeed"] = sol::property(&CalculateMovementSpeed::getEventEnabled, &CalculateMovementSpeed::setEventEnabled);
				usertypeDefinition["calcTrainingPrice"] = sol::property(&CalculateTrainingPriceEvent::getEventEnabled, &CalculateTrainingPriceEvent::setEventEnabled);
				usertypeDefinition["calcTravelPrice"] = sol::property(&CalculateTravelPriceEvent::getEventEnabled, &CalculateTravelPriceEvent::setEventEnabled);
				usertypeDefinition["calcWalkSpeed"] = sol::property(&CalculateMovementSpeed::getEventEnabled, &CalculateMovementSpeed::setEventEnabled);
				usertypeDefinition["cellActivated"] = sol::property(&CellActivatedEvent::getEventEnabled, &CellActivatedEvent::setEventEnabled);
				usertypeDefinition["cellChanged"] = sol::property(&CellChangedEvent::getEventEnabled, &CellChangedEvent::setEventEnabled);
				usertypeDefinition["cellDeactivated"] = sol::property(&CellDeactivatedEvent::getEventEnabled, &CellDeactivatedEvent::setEventEnabled);
				usertypeDefinition["collideWater"] = sol::property(&MobileObjectWaterImpactEvent::getEventEnabled, &MobileObjectWaterImpactEvent::setEventEnabled);
				usertypeDefinition["collision"] = sol::property(&MobileObjectCollisionEvent::getEventEnabled, &MobileObjectCollisionEvent::setEventEnabled);
				usertypeDefinition["combatStart"] = sol::property(&CombatStartEvent::getEventEnabled, &CombatStartEvent::setEventEnabled);
				usertypeDefinition["combatStarted"] = sol::property(&CombatStartedEvent::getEventEnabled, &CombatStartedEvent::setEventEnabled);
				usertypeDefinition["combatStop"] = sol::property(&CombatStopEvent::getEventEnabled, &CombatStopEvent::setEventEnabled);
				usertypeDefinition["combatStopped"] = sol::property(&CombatStoppedEvent::getEventEnabled, &CombatStoppedEvent::setEventEnabled);
				usertypeDefinition["containerClosed"] = sol::property(&ContainerClosedEvent::getEventEnabled, &ContainerClosedEvent::setEventEnabled);
				usertypeDefinition["convertReferenceToItem"] = sol::property(&ConvertReferenceToItemEvent::getEventEnabled, &ConvertReferenceToItemEvent::setEventEnabled);
				usertypeDefinition["crimeWitnessed"] = sol::property(&CrimeWitnessedEvent::getEventEnabled, &CrimeWitnessedEvent::setEventEnabled);
				usertypeDefinition["damage"] = sol::property(&DamageEvent::getEventEnabled, &DamageEvent::setEventEnabled);
				usertypeDefinition["damaged"] = sol::property(&DamagedEvent::getEventEnabled, &DamagedEvent::setEventEnabled);
				usertypeDefinition["damagedHandToHand"] = sol::property(&DamagedHandToHandEvent::getEventEnabled, &DamagedHandToHandEvent::setEventEnabled);
				usertypeDefinition["damageHandToHand"] = sol::property(&DamageHandToHandEvent::getEventEnabled, &DamageHandToHandEvent::setEventEnabled);
				usertypeDefinition["death"] = sol::property(&DeathEvent::getEventEnabled, &DeathEvent::setEventEnabled);
				usertypeDefinition["detectSneak"] = sol::property(&DetectSneakEvent::getEventEnabled, &DetectSneakEvent::setEventEnabled);
				usertypeDefinition["determineAction"] = sol::property(&DetermineActionEvent::getEventEnabled, &DetermineActionEvent::setEventEnabled);
				usertypeDefinition["determinedAction"] = sol::property(&DeterminedActionEvent::getEventEnabled, &DeterminedActionEvent::setEventEnabled);
				usertypeDefinition["enchantedItemCreated"] = sol::property(&EnchantedItemCreatedEvent::getEventEnabled, &EnchantedItemCreatedEvent::setEventEnabled);
				usertypeDefinition["enchantedItemCreateFailed"] = sol::property(&EnchantedItemCreateFailedEvent::getEventEnabled, &EnchantedItemCreateFailedEvent::setEventEnabled);
				usertypeDefinition["enterFrame"] = sol::property(&FrameEvent::getEventEnabled, &FrameEvent::setEventEnabled);
				usertypeDefinition["equip"] = sol::property(&EquipEvent::getEventEnabled, &EquipEvent::setEventEnabled);
				usertypeDefinition["equipped"] = sol::property(&EquippedEvent::getEventEnabled, &EquippedEvent::setEventEnabled);
				usertypeDefinition["exerciseSkill"] = sol::property(&SkillExerciseEvent::getEventEnabled, &SkillExerciseEvent::setEventEnabled);
				usertypeDefinition["filterBarterMenu"] = sol::property(&FilterBarterMenuEvent::getEventEnabled, &FilterBarterMenuEvent::setEventEnabled);
				usertypeDefinition["filterContentsMenu"] = sol::property(&FilterContentsMenuEvent::getEventEnabled, &FilterContentsMenuEvent::setEventEnabled);
				usertypeDefinition["filterInventory"] = sol::property(&FilterInventoryEvent::getEventEnabled, &FilterInventoryEvent::setEventEnabled);
				usertypeDefinition["filterInventorySelect"] = sol::property(&FilterInventorySelectEvent::getEventEnabled, &FilterInventorySelectEvent::setEventEnabled);
				usertypeDefinition["filterSoulGemTarget"] = sol::property(&FilterSoulGemTargetEvent::getEventEnabled, &FilterSoulGemTargetEvent::setEventEnabled);
				usertypeDefinition["infoFilter"] = sol::property(&InfoFilterEvent::getEventEnabled, &InfoFilterEvent::setEventEnabled);
				usertypeDefinition["infoGetText"] = sol::property(&InfoGetTextEvent::getEventEnabled, &InfoGetTextEvent::setEventEnabled);
				usertypeDefinition["infoLinkResolve"] = sol::property(&InfoLinkResolveEvent::getEventEnabled, &InfoLinkResolveEvent::setEventEnabled);
				usertypeDefinition["infoResponse"] = sol::property(&InfoResponseEvent::getEventEnabled, &InfoResponseEvent::setEventEnabled);
				usertypeDefinition["isGuard"] = sol::property(&IsGuardEvent::getEventEnabled, &IsGuardEvent::setEventEnabled);
				usertypeDefinition["itemDropped"] = sol::property(&ItemDroppedEvent::getEventEnabled, &ItemDroppedEvent::setEventEnabled);
				usertypeDefinition["itemTileUpdated"] = sol::property(&ItemTileUpdatedEvent::getEventEnabled, &ItemTileUpdatedEvent::setEventEnabled);
				usertypeDefinition["journal"] = sol::property(&JournalEvent::getEventEnabled, &JournalEvent::setEventEnabled);
				usertypeDefinition["keyDown"] = sol::property(&KeyDownEvent::getEventEnabled, &KeyDownEvent::setEventEnabled);
				usertypeDefinition["keyUp"] = sol::property(&KeyUpEvent::getEventEnabled, &KeyUpEvent::setEventEnabled);
				usertypeDefinition["leveledCreaturePicked"] = sol::property(&LeveledCreaturePickedEvent::getEventEnabled, &LeveledCreaturePickedEvent::setEventEnabled);
				usertypeDefinition["leveledItemPicked"] = sol::property(&LeveledItemPickedEvent::getEventEnabled, &LeveledItemPickedEvent::setEventEnabled);
				usertypeDefinition["levelUp"] = sol::property(&LevelUpEvent::getEventEnabled, &LevelUpEvent::setEventEnabled);
				usertypeDefinition["load"] = sol::property(&LoadGameEvent::getEventEnabled, &LoadGameEvent::setEventEnabled);
				usertypeDefinition["loaded"] = sol::property(&LoadedGameEvent::getEventEnabled, &LoadedGameEvent::setEventEnabled);
				usertypeDefinition["lockPick"] = sol::property(&PickLockEvent::getEventEnabled, &PickLockEvent::setEventEnabled);
				usertypeDefinition["magicCasted"] = sol::property(&MagicCastedEvent::getEventEnabled, &MagicCastedEvent::setEventEnabled);
				usertypeDefinition["menuEnter"] = sol::property(&MenuStateEvent::getEventEnabled, &MenuStateEvent::setEventEnabled);
				usertypeDefinition["menuExit"] = sol::property(&MenuStateEvent::getEventEnabled, &MenuStateEvent::setEventEnabled);
				usertypeDefinition["meshLoaded"] = sol::property(&MeshLoadedEvent::getEventEnabled, &MeshLoadedEvent::setEventEnabled);
				usertypeDefinition["mobileActivated"] = sol::property(&MobileActorActivatedEvent::getEventEnabled, &MobileActorActivatedEvent::setEventEnabled);
				usertypeDefinition["mobileDeactivated"] = sol::property(&MobileActorDeactivatedEvent::getEventEnabled, &MobileActorDeactivatedEvent::setEventEnabled);
				usertypeDefinition["mouseAxis"] = sol::property(&MouseAxisEvent::getEventEnabled, &MouseAxisEvent::setEventEnabled);
				usertypeDefinition["mouseButtonDown"] = sol::property(&MouseButtonDownEvent::getEventEnabled, &MouseButtonDownEvent::setEventEnabled);
				usertypeDefinition["mouseButtonUp"] = sol::property(&MouseButtonUpEvent::getEventEnabled, &MouseButtonUpEvent::setEventEnabled);
				usertypeDefinition["mouseWheel"] = sol::property(&MouseWheelEvent::getEventEnabled, &MouseWheelEvent::setEventEnabled);
				usertypeDefinition["musicSelectTrack"] = sol::property(&MusicSelectTrackEvent::getEventEnabled, &MusicSelectTrackEvent::setEventEnabled);
				usertypeDefinition["objectInvalidated"] = sol::property(&ObjectInvalidatedEvent::getEventEnabled, &ObjectInvalidatedEvent::setEventEnabled);
				usertypeDefinition["playGroup"] = sol::property(&PlayAnimationGroupEvent::getEventEnabled, &PlayAnimationGroupEvent::setEventEnabled);
				usertypeDefinition["playItemSound"] = sol::property(&PlayItemSoundEvent::getEventEnabled, &PlayItemSoundEvent::setEventEnabled);
				usertypeDefinition["postInfoResponse"] = sol::property(&PostInfoResponseEvent::getEventEnabled, &PostInfoResponseEvent::setEventEnabled);
				usertypeDefinition["potionBrewed"] = sol::property(&PotionBrewedEvent::getEventEnabled, &PotionBrewedEvent::setEventEnabled);
				usertypeDefinition["potionBrewFailed"] = sol::property(&PotionBrewFailedEvent::getEventEnabled, &PotionBrewFailedEvent::setEventEnabled);
				usertypeDefinition["powerRecharged"] = sol::property(&PowerRechargedEvent::getEventEnabled, &PowerRechargedEvent::setEventEnabled);
				usertypeDefinition["preLevelUp"] = sol::property(&PreLevelUpEvent::getEventEnabled, &PreLevelUpEvent::setEventEnabled);
				usertypeDefinition["preventRest"] = sol::property(&PreventRestEvent::getEventEnabled, &PreventRestEvent::setEventEnabled);
				usertypeDefinition["projectileExpire"] = sol::property(&ProjectileExpireEvent::getEventEnabled, &ProjectileExpireEvent::setEventEnabled);
				usertypeDefinition["projectileHitActor"] = sol::property(&MobileProjectileActorCollisionEvent::getEventEnabled, &MobileProjectileActorCollisionEvent::setEventEnabled);
				usertypeDefinition["projectileHitObject"] = sol::property(&MobileProjectileObjectCollisionEvent::getEventEnabled, &MobileProjectileObjectCollisionEvent::setEventEnabled);
				usertypeDefinition["projectileHitTerrain"] = sol::property(&MobileProjectileTerrainCollisionEvent::getEventEnabled, &MobileProjectileTerrainCollisionEvent::setEventEnabled);
				usertypeDefinition["referenceActivated"] = sol::property(&ReferenceActivatedEvent::getEventEnabled, &ReferenceActivatedEvent::setEventEnabled);
				usertypeDefinition["referenceDeactivated"] = sol::property(&ReferenceDeactivatedEvent::getEventEnabled, &ReferenceDeactivatedEvent::setEventEnabled);
				usertypeDefinition["referenceSceneNodeCreated"] = sol::property(&ReferenceSceneNodeCreatedEvent::getEventEnabled, &ReferenceSceneNodeCreatedEvent::setEventEnabled);
				usertypeDefinition["restInterrupt"] = sol::property(&RestInterruptEvent::getEventEnabled, &RestInterruptEvent::setEventEnabled);
				usertypeDefinition["save"] = sol::property(&SaveGameEvent::getEventEnabled, &SaveGameEvent::setEventEnabled);
				usertypeDefinition["saved"] = sol::property(&SavedGameEvent::getEventEnabled, &SavedGameEvent::setEventEnabled);
				usertypeDefinition["simulate"] = sol::property(&SimulateEvent::getEventEnabled, &SimulateEvent::setEventEnabled);
				usertypeDefinition["skillRaised"] = sol::property(&SkillRaisedEvent::getEventEnabled, &SkillRaisedEvent::setEventEnabled);
				usertypeDefinition["soundObjectPlay"] = sol::property(&SoundObjectPlayEvent::getEventEnabled, &SoundObjectPlayEvent::setEventEnabled);
				usertypeDefinition["spellCast"] = sol::property(&SpellCastEvent::getEventEnabled, &SpellCastEvent::setEventEnabled);
				usertypeDefinition["spellCasted"] = sol::property(&SpellCastedEvent::getEventEnabled, &SpellCastedEvent::setEventEnabled);
				usertypeDefinition["spellCastedFailure"] = sol::property(&SpellCastedEvent::getEventEnabled, &SpellCastedEvent::setEventEnabled);
				usertypeDefinition["spellCreated"] = sol::property(&SpellCreatedEvent::getEventEnabled, &SpellCreatedEvent::setEventEnabled);
				usertypeDefinition["spellResist"] = sol::property(&SpellResistEvent::getEventEnabled, &SpellResistEvent::setEventEnabled);
				usertypeDefinition["spellTick"] = sol::property(&SpellTickEvent::getEventEnabled, &SpellTickEvent::setEventEnabled);
				usertypeDefinition["topicAdded"] = sol::property(&AddTopicEvent::getEventEnabled, &AddTopicEvent::setEventEnabled);
				usertypeDefinition["trapDisarm"] = sol::property(&DisarmTrapEvent::getEventEnabled, &DisarmTrapEvent::setEventEnabled);
				usertypeDefinition["uiActivated"] = sol::property(&GenericUiActivatedEvent::getEventEnabled, &GenericUiActivatedEvent::setEventEnabled);
				usertypeDefinition["uiEvent"] = sol::property(&GenericUiPostEvent::getEventEnabled, &GenericUiPostEvent::setEventEnabled);
				usertypeDefinition["uiObjectTooltip"] = sol::property(&UiObjectTooltipEvent::getEventEnabled, &UiObjectTooltipEvent::setEventEnabled);
				usertypeDefinition["uiPreEvent"] = sol::property(&GenericUiPreEvent::getEventEnabled, &GenericUiPreEvent::setEventEnabled);
				usertypeDefinition["uiRefreshed"] = sol::property(&UiRefreshedEvent::getEventEnabled, &UiRefreshedEvent::setEventEnabled);
				usertypeDefinition["uiShowRestMenu"] = sol::property(&ShowRestWaitMenuEvent::getEventEnabled, &ShowRestWaitMenuEvent::setEventEnabled);
				usertypeDefinition["uiSpellTooltip"] = sol::property(&UiSpellTooltipEvent::getEventEnabled, &UiSpellTooltipEvent::setEventEnabled);
				usertypeDefinition["unequipped"] = sol::property(&UnequippedEvent::getEventEnabled, &UnequippedEvent::setEventEnabled);
				usertypeDefinition["weaponReadied"] = sol::property(&WeaponReadiedEvent::getEventEnabled, &WeaponReadiedEvent::setEventEnabled);
				usertypeDefinition["weaponUnreadied"] = sol::property(&WeaponUnreadiedEvent::getEventEnabled, &WeaponUnreadiedEvent::setEventEnabled);
				usertypeDefinition["weatherChangedImmediate"] = sol::property(&WeatherChangedImmediateEvent::getEventEnabled, &WeatherChangedImmediateEvent::setEventEnabled);
				usertypeDefinition["weatherCycled"] = sol::property(&WeatherCycledEvent::getEventEnabled, &WeatherCycledEvent::setEventEnabled);
				usertypeDefinition["weatherTransitionFinished"] = sol::property(&WeatherTransitionFinishedEvent::getEventEnabled, &WeatherTransitionFinishedEvent::setEventEnabled);
				usertypeDefinition["weatherTransitionStarted"] = sol::property(&WeatherTransitionStartedEvent::getEventEnabled, &WeatherTransitionStartedEvent::setEventEnabled);
			}
		}
	}
}
