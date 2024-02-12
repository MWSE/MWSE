#include "TES3DialogueFilterContext.h"

#include "TES3Actor.h"
#include "TES3Cell.h"
#include "TES3DataHandler.h"
#include "TES3DialogueInfo.h"
#include "TES3Faction.h"
#include "TES3GlobalVariable.h"
#include "TES3MobilePlayer.h"
#include "TES3MobManager.h"
#include "TES3NPC.h"
#include "TES3Reference.h"
#include "TES3Script.h"
#include "TES3UIElement.h"
#include "TES3UIManager.h"
#include "TES3WeatherController.h"
#include "TES3WorldController.h"

#include "StringUtil.h"
#include "CodePatchUtil.h"

namespace TES3 {
	DialogueFilterContext::DialogueFilterContext(Object* _speaker, Reference* _reference, DialogueInfo::FilterSource _source, Dialogue* _dialogue, const DialogueInfo* _dialogueInfo) {
		// Store the paramaters passed to DialogueInfo::filter
		speaker = _speaker;
		speakerReference = _reference;
		speakerMobile = speakerReference ? speakerReference->getAttachedMobileActor() : nullptr;
		source = _source;
		dialogue = _dialogue;
		dialogueInfo = _dialogueInfo;

		// Store the base actor.
		speakerBaseActor = static_cast<Actor*>(speaker->getBaseObject());

		// Parse out the linked list of conditions into our context.
		auto condition = dialogueInfo->conditions;
		while (condition) {
			switch (condition->tag) {
			case DialogueInfoFilterType::Actor:
				filterActor = condition->actor;
				break;
			case DialogueInfoFilterType::Race:
				filterRace = condition->npcRace;
				break;
			case DialogueInfoFilterType::Class:
				filterClass = condition->npcClass;
				break;
			case DialogueInfoFilterType::NPCFaction:
				filterFaction = condition->npcFaction;
				break;
			case DialogueInfoFilterType::Cell:
				filterCell = condition->cell;
				break;
			case DialogueInfoFilterType::PCFaction:
				filterPlayerFaction = condition->pcFaction;
				break;
			case DialogueInfoFilterType::SoundPath:
				filterResultScript = condition->soundPath;
				break;
			case DialogueInfoFilterType::Conditional0:
				setConditional(0, condition->conditional);
				break;
			case DialogueInfoFilterType::Conditional1:
				setConditional(1, condition->conditional);
				break;
			case DialogueInfoFilterType::Conditional2:
				setConditional(2, condition->conditional);
				break;
			case DialogueInfoFilterType::Conditional3:
				setConditional(3, condition->conditional);
				break;
			case DialogueInfoFilterType::Conditional4:
				setConditional(4, condition->conditional);
				break;
			case DialogueInfoFilterType::Conditional5:
				setConditional(5, condition->conditional);
				break;
			}
			condition = condition->next;
		}
	}

	void DialogueFilterContext::setConditional(size_t index, DialogueConditional* conditional) {
		auto& conditionalContext = conditionalContexts[index];
		conditionalContext.parentContext = this;
		conditionalContext.conditional = conditional;
		conditionalContext.compareOperator = conditional->compareOperator;
	}

	//
	// DialogueFilterContext::ConditionalContext
	//

	using ConditionalLoadFunction = std::function<void(DialogueFilterContext::ConditionalContext*)>;

	void loadFunctionReactionLow(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto speakerBase = context->parentContext->speakerBaseActor;
		if (speakerBase->objectType != ObjectType::NPC) {
			return;
		}

		const auto faction = speakerBase->getFaction();
		if (faction == nullptr) {
			context->compareValue = 0.0f;
			return;
		}

		context->compareValue = faction->getLowestJoinedReaction();
	}

	void loadFunctionReactionHigh(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto speakerBase = context->parentContext->speakerBaseActor;
		if (speakerBase->objectType != ObjectType::NPC) {
			return;
		}

		const auto faction = speakerBase->getFaction();
		if (faction == nullptr) {
			context->compareValue = 0.0f;
			return;
		}

		context->compareValue = faction->getHighestJoinedReaction();
	}

	void loadFunctionRankRequirement(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto speakerBase = context->parentContext->speakerBaseActor;
		if (speakerBase->objectType != ObjectType::NPC) {
			return;
		}

		const auto faction = speakerBase->getFaction();
		if (faction == nullptr) {
			return;
		}

		context->compareValue = (float)faction->getAdvancementPotential();
	}

	void loadFunctionReputation(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto speaker = context->parentContext->speaker;
		if (speaker->objectType != ObjectType::NPC) {
			return;
		}

		context->compareValue = speaker->getReputation();
	}

	void loadFunctionHealthPercent(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto mobile = context->parentContext->speakerMobile;
		if (mobile == nullptr || !mobile->isActor()) {
			return;
		}
		context->compareValue = mobile->health.getNormalized() * 100.0f;
	}

	void loadFunctionPCReputation(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto macp = WorldController::get()->getMobilePlayer();
		context->compareValue = macp->npcInstance->getReputation();
	}

	void loadFunctionPCLevel(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto macp = WorldController::get()->getMobilePlayer();
		context->compareValue = macp->npcInstance->getLevel();
	}

	void loadFunctionPCHealthPercent(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto macp = WorldController::get()->getMobilePlayer();
		context->compareValue = macp->health.getNormalized() * 100.0f;
	}

	void loadFunctionPCMagicka(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto macp = WorldController::get()->getMobilePlayer();
		context->compareValue = macp->magicka.getCurrent();
	}

	void loadFunctionPCFatigue(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto macp = WorldController::get()->getMobilePlayer();
		context->compareValue = macp->fatigue.getCurrent();
	}

	void loadFunctionPCStrength(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto macp = WorldController::get()->getMobilePlayer();
		context->compareValue = macp->getAttributeStrength()->getCurrent();
	}

	void loadFunctionPCSkill(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto macp = WorldController::get()->getMobilePlayer();
		const auto skill = (int)context->conditional->function - (int)DialogueConditionalFunction::PCBlock;
		context->compareValue = macp->getSkillStatistic(skill)->getCurrent();
	}

	void loadFunctionPCSex(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		auto macp = WorldController::get()->getMobilePlayer();
		context->compareValue = macp->npcInstance->isFemale() ? 1.0f : 0.0f;
	}

	void loadFunctionPCExpelled(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto speakerBase = context->parentContext->speakerBaseActor;
		if (speakerBase->objectType != ObjectType::NPC) {
			return;
		}

		const auto faction = speakerBase->getFaction();
		if (faction == nullptr) {
			return;
		}

		context->compareValue = faction->getPlayerExpelled() ? 1.0f : 0.0f;
	}

	void loadFunctionPCCommonDisease(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto macp = WorldController::get()->getMobilePlayer();
		context->compareValue = macp->hasCommonDisease() ? 1.0f : 0.0f;
	}

	void loadFunctionPCBlightDisease(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto macp = WorldController::get()->getMobilePlayer();
		context->compareValue = macp->hasBlightDisease() ? 1.0f : 0.0f;
	}

	void loadFunctionPCClothingModifier(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto macp = WorldController::get()->getMobilePlayer();
		context->compareValue = macp->npcInstance->getEquipmentValue(false);
	}

	void loadFunctionPCCrimeLevel(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto macp = WorldController::get()->getMobilePlayer();
		context->compareValue = macp->getBounty();
	}

	void loadFunctionSameSex(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto macp = WorldController::get()->getMobilePlayer();
		const auto match = macp->npcInstance->isFemale() == context->parentContext->speakerBaseActor->isFemale();
		context->compareValue = match ? 1.0f : 0.0f;
		context->compareOperator = DialogueConditionalComparator::Equal;
	}

	void loadFunctionSameRace(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto macp = WorldController::get()->getMobilePlayer();
		const auto match = macp->npcInstance->getRace() == context->parentContext->speakerBaseActor->getRace();
		context->compareValue = match ? 1.0f : 0.0f;
		context->compareOperator = DialogueConditionalComparator::Equal;
	}

	void loadFunctionSameFaction(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto speakerBase = context->parentContext->speakerBaseActor;
		if (speakerBase->objectType != ObjectType::NPC) {
			return;
		}

		const auto faction = speakerBase->getFaction();
		context->compareValue = faction && faction->getPlayerJoined() ? 1.0f : 0.0f;
		context->compareOperator = DialogueConditionalComparator::Equal;
	}

	void loadFunctionFactionRankDifference(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto speakerBase = context->parentContext->speakerBaseActor;
		if (speakerBase->objectType != ObjectType::NPC) {
			return;
		}

		const auto faction = speakerBase->getFaction();
		if (faction == nullptr) {
			context->compareValue = 0.0f;
			return;
		}

		context->compareValue = faction->getEffectivePlayerRank() - speakerBase->getFactionRank();
	}

	void loadFunctionDetected(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto worldController = WorldController::get();
		const auto mobile = context->parentContext->speakerMobile;
		const auto macp = worldController->getMobilePlayer();
		const auto detected = worldController->mobManager->processManager->detectByActor(mobile, macp);
		context->compareValue = detected ? 1.0f : 0.0f;
	}

	void loadFunctionAlarmed(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto isAlarmed = context->parentContext->speakerMobile->actionData.aiBehaviorState == AIBehavior::Alarmed;
		context->compareValue = isAlarmed ? 1.0f : 0.0f;
	}

	void loadFunctionChoice(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto uiidMenuDialogue = *reinterpret_cast<UI::UI_ID*>(0x7D3442);
		const auto menuDialog = UI::findMenu(uiidMenuDialogue);
		if (menuDialog == nullptr) {
			return;
		}

		// Handle support for MCP's choice patch.
		if (mwse::mcp::getFeatureEnabled(mwse::mcp::feature::ServiceRefusalFiltering)) {
			const auto source = context->parentContext->source;
			if (source > DialogueInfo::FilterSource::MCP_Offset) {
				context->compareValue = int(source) - int(DialogueInfo::FilterSource::MCP_Offset);
				return;
			}
		}

		const auto uiidPartHyperText_dialog = *reinterpret_cast<UI::Property*>(0x7D7C80);
		const auto currentDialogue = static_cast<Dialogue*>(menuDialog->getProperty(UI::PropertyType::Pointer, uiidPartHyperText_dialog).ptrValue);
		if (currentDialogue != context->parentContext->dialogue) {
			return;
		}

		const auto uiidMenuDialogue_answer = *reinterpret_cast<UI::Property*>(0x7D3420);
		const auto answer = menuDialog->getProperty(UI::PropertyType::Integer, uiidMenuDialogue_answer).integerValue;
		if (answer < 0) {
			return;
		}

		context->compareValue = answer;
	}

	void loadFunctionPCIntelligence(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		auto macp = WorldController::get()->getMobilePlayer();
		context->compareValue = macp->getAttributeIntelligence()->getCurrent();
	}

	void loadFunctionPCWillpower(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		auto macp = WorldController::get()->getMobilePlayer();
		context->compareValue = macp->getAttributeWillpower()->getCurrent();
	}

	void loadFunctionPCAgility(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		auto macp = WorldController::get()->getMobilePlayer();
		context->compareValue = macp->getAttributeAgility()->getCurrent();
	}

	void loadFunctionPCSpeed(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		auto macp = WorldController::get()->getMobilePlayer();
		context->compareValue = macp->getAttributeSpeed()->getCurrent();
	}

	void loadFunctionPCEndurance(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		auto macp = WorldController::get()->getMobilePlayer();
		context->compareValue = macp->getAttributeEndurance()->getCurrent();
	}

	void loadFunctionPCPersonality(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		auto macp = WorldController::get()->getMobilePlayer();
		context->compareValue = macp->getAttributePersonality()->getCurrent();
	}

	void loadFunctionPCLuck(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		auto macp = WorldController::get()->getMobilePlayer();
		context->compareValue = macp->getAttributeLuck()->getCurrent();
	}

	void loadFunctionPCCorprus(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto macp = WorldController::get()->getMobilePlayer();
		context->compareValue = macp->hasCorprusDisease() ? 1.0f : 0.0f;
	}

	void loadFunctionWeather(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		// Ignore if we are indoors.
		if (DataHandler::get()->currentInteriorCell) {
			return;
		}

		context->compareValue = WorldController::get()->weatherController->getCurrentWeatherIndex();
	}

	void loadFunctionPCVampire(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto macp = WorldController::get()->getMobilePlayer();
		context->compareValue = macp->hasVampirism() ? 1.0f : 0.0f;
	}

	void loadFunctionLevel(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		context->compareValue = context->parentContext->speakerBaseActor->getLevel();
	}

	void loadFunctionAttacked(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto attacked = context->parentContext->speakerMobile->getFlagAttacked();
		context->compareValue = attacked ? 1.0f : 0.0f;
	}

	void loadFunctionTalkedToPC(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto attacked = context->parentContext->speakerMobile->getFlagTalkedTo();
		context->compareValue = attacked ? 1.0f : 0.0f;
	}

	void loadFunctionPCHealth(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto macp = WorldController::get()->getMobilePlayer();
		context->compareValue = macp->health.getCurrent();
	}

	void loadFunctionCreatureTarget(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto target = context->parentContext->speakerMobile->actionData.target;
		if (target == nullptr) {
			return;
		}

		if (target->actorType == MobileActorType::Creature) {
			context->compareValue = 1;
		}
		else if (target->getIsWerewolf()) {
			context->compareValue = 2;
		}
		else {
			context->compareValue = 0;
		}
	}

	void loadFunctionFriendHit(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		context->compareValue = context->parentContext->speakerMobile->friendlyFireHitCount;
	}

	void loadFunctionFight(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		context->compareValue = context->parentContext->speakerMobile->fight;
	}

	void loadFunctionHello(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		context->compareValue = context->parentContext->speakerMobile->hello;
	}

	void loadFunctionAlarm(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		context->compareValue = context->parentContext->speakerMobile->alarm;
	}

	void loadFunctionFlee(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		context->compareValue = context->parentContext->speakerMobile->flee;
	}

	void loadFunctionShouldAttack(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto macp = WorldController::get()->getMobilePlayer();
		const auto fightWeight = context->parentContext->speakerMobile->getActionWeightFight(macp);
		context->compareValue = fightWeight < 100.0f ? 0.0f : 1.0f;
	}

	void loadFunctionWerewolf(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto isWerewolf = context->parentContext->speakerMobile->getIsWerewolf();
		context->compareValue = isWerewolf ? 1.0f : 0.0f;
	}

	void loadFunctionPCWerewolfKills(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		context->compareValue = WorldController::get()->playerKills->werewolfKills;
	}

	ConditionalLoadFunction loadFunctionFunction(DialogueConditional* conditional) {
		auto worldController = WorldController::get();
		auto macp = worldController->getMobilePlayer();

		switch (conditional->function) {
		case DialogueConditionalFunction::ReactionLow: return loadFunctionReactionLow;
		case DialogueConditionalFunction::ReactionHigh: return loadFunctionReactionHigh;
		case DialogueConditionalFunction::RankRequirement: return loadFunctionRankRequirement;
		case DialogueConditionalFunction::Reputation: return loadFunctionReputation;
		case DialogueConditionalFunction::HealthPercent: return loadFunctionHealthPercent;
		case DialogueConditionalFunction::PCReputation: return loadFunctionPCReputation;
		case DialogueConditionalFunction::PCLevel: return loadFunctionPCLevel;
		case DialogueConditionalFunction::PCHealthPercent: return loadFunctionPCHealthPercent;
		case DialogueConditionalFunction::PCMagicka: return loadFunctionPCMagicka;
		case DialogueConditionalFunction::PCFatigue: return loadFunctionPCFatigue;
		case DialogueConditionalFunction::PCStrength: return loadFunctionPCStrength;
		case DialogueConditionalFunction::PCBlock:
		case DialogueConditionalFunction::PCArmorer:
		case DialogueConditionalFunction::PCMediumArmor:
		case DialogueConditionalFunction::PCHeavyArmor:
		case DialogueConditionalFunction::PCBluntWeapon:
		case DialogueConditionalFunction::PCLongBlade:
		case DialogueConditionalFunction::PCAxe:
		case DialogueConditionalFunction::PCSpear:
		case DialogueConditionalFunction::PCAthletics:
		case DialogueConditionalFunction::PCEnchant:
		case DialogueConditionalFunction::PCDestruction:
		case DialogueConditionalFunction::PCAlteration:
		case DialogueConditionalFunction::PCIllusion:
		case DialogueConditionalFunction::PCConjuration:
		case DialogueConditionalFunction::PCMysticism:
		case DialogueConditionalFunction::PCRestoration:
		case DialogueConditionalFunction::PCAlchemy:
		case DialogueConditionalFunction::PCUnarmored:
		case DialogueConditionalFunction::PCSecurity:
		case DialogueConditionalFunction::PCSneak:
		case DialogueConditionalFunction::PCAcrobatics:
		case DialogueConditionalFunction::PCLightArmor:
		case DialogueConditionalFunction::PCShortBlade:
		case DialogueConditionalFunction::PCMarksman:
		case DialogueConditionalFunction::PCMercantile:
		case DialogueConditionalFunction::PCSpeechcraft:
		case DialogueConditionalFunction::PCHandToHand:
			return loadFunctionPCSkill;
		case DialogueConditionalFunction::PCSex: return loadFunctionPCSex;
		case DialogueConditionalFunction::PCExpelled: return loadFunctionPCExpelled;
		case DialogueConditionalFunction::PCCommonDisease: return loadFunctionPCCommonDisease;
		case DialogueConditionalFunction::PCBlightDisease: return loadFunctionPCBlightDisease;
		case DialogueConditionalFunction::PCClothingModifier: return loadFunctionPCClothingModifier;
		case DialogueConditionalFunction::PCCrimeLevel: return loadFunctionPCCrimeLevel;
		case DialogueConditionalFunction::SameSex: return loadFunctionSameSex;
		case DialogueConditionalFunction::SameRace: return loadFunctionSameRace;
		case DialogueConditionalFunction::SameFaction: return loadFunctionSameFaction;
		case DialogueConditionalFunction::FactionRankDifference: return loadFunctionFactionRankDifference;
		case DialogueConditionalFunction::Detected: return loadFunctionDetected;
		case DialogueConditionalFunction::Alarmed: return loadFunctionAlarmed;
		case DialogueConditionalFunction::Choice: return loadFunctionChoice;
		case DialogueConditionalFunction::PCIntelligence: return loadFunctionPCIntelligence;
		case DialogueConditionalFunction::PCWillpower: return loadFunctionPCWillpower;
		case DialogueConditionalFunction::PCAgility: return loadFunctionPCAgility;
		case DialogueConditionalFunction::PCSpeed: return loadFunctionPCSpeed;
		case DialogueConditionalFunction::PCEndurance: return loadFunctionPCEndurance;
		case DialogueConditionalFunction::PCPersonality: return loadFunctionPCPersonality;
		case DialogueConditionalFunction::PCLuck: return loadFunctionPCLuck;
		case DialogueConditionalFunction::PCCorprus: return loadFunctionPCCorprus;
		case DialogueConditionalFunction::Weather: return loadFunctionWeather;
		case DialogueConditionalFunction::PCVampire: return loadFunctionPCVampire;
		case DialogueConditionalFunction::Level: return loadFunctionLevel;
		case DialogueConditionalFunction::Attacked: return loadFunctionAttacked;
		case DialogueConditionalFunction::TalkedToPC: return loadFunctionTalkedToPC;
		case DialogueConditionalFunction::PCHealth: return loadFunctionPCHealth;
		case DialogueConditionalFunction::CreatureTarget: return loadFunctionCreatureTarget;
		case DialogueConditionalFunction::FriendHit: return loadFunctionFriendHit;
		case DialogueConditionalFunction::Fight: return loadFunctionFight;
		case DialogueConditionalFunction::Hello: return loadFunctionHello;
		case DialogueConditionalFunction::Alarm: return loadFunctionAlarm;
		case DialogueConditionalFunction::Flee: return loadFunctionFlee;
		case DialogueConditionalFunction::ShouldAttack: return loadFunctionShouldAttack;
		case DialogueConditionalFunction::Werewolf: return loadFunctionWerewolf;
		case DialogueConditionalFunction::PCWerewolfKills: return loadFunctionPCWerewolfKills;
		}

		throw std::runtime_error("Invalid function encountered.");
	}

	void loadGlobalValue(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		context->compareValue = context->conditional->globalVariable->getValue();
	}

	void loadLocalVariableValue(DialogueFilterContext::ConditionalContext* context) {
		const auto script = context->parentContext->speakerBaseActor->getScript();
		if (script == nullptr) {
			context->resultOverride = false;
			return;
		}

		unsigned int index = 0;
		const auto varType = script->getLocalVarIndexAndType(context->conditional->localVarName, &index);
		if (varType == 0) {
			context->resultOverride = false;
			return;
		}

		auto variables = &script->varValues;
		if (context->parentContext->speakerReference) {
			variables = context->parentContext->speakerReference->getScriptVariables();
		}
		else {
			context->resultOverride = true;
		}

		switch (varType) {
		case 'f':
			context->compareValue = variables->floatVarValues[index];
			break;
		case 'l':
			context->compareValue = variables->longVarValues[index];
			break;
		case 's':
			context->compareValue = variables->shortVarValues[index];
			break;
#if _DEBUG
		default:
			throw std::runtime_error("Invalid local variable type.");
#endif
		}
	}

	void loadJournalIndex(DialogueFilterContext::ConditionalContext* context) {
		if (context->parentContext->speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}
		context->compareValue = context->conditional->journal->journalIndex;
	}

	void loadItemCount(DialogueFilterContext::ConditionalContext* context) {
		const auto speakerReference = context->parentContext->speakerReference;
		if (speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		const auto item = context->conditional->item;
		if (item == nullptr) {
			return;
		}

		const auto macp = WorldController::get()->getMobilePlayer();
		const auto playerInventory = macp->reference->getInventory();
		if (playerInventory == nullptr) {
			return;
		}

		context->compareValue = std::abs(playerInventory->getItemCount(item));
	}

	void loadDeadActorCount(DialogueFilterContext::ConditionalContext* context) {
		const auto speakerReference = context->parentContext->speakerReference;
		if (speakerReference == nullptr) {
			context->resultOverride = true;
			return;
		}

		context->compareValue = WorldController::get()->playerKills->getKillCount(context->conditional->actor);
	}

	void loadNotID(DialogueFilterContext::ConditionalContext* context) {
		// This conditional bypasses compareValue comparisons.
		if (context->conditional->actor) {
			context->resultOverride = context->conditional->actor != context->parentContext->speakerBaseActor;
		}
		else {
			context->resultOverride = true;
		}
	}

	void loadNotFaction(DialogueFilterContext::ConditionalContext* context) {
		// This conditional bypasses compareValue comparisons.
		if (context->conditional->faction) {
			context->resultOverride = context->conditional->faction != context->parentContext->speakerBaseActor->getFaction();
		}
		else {
			context->resultOverride = true;
		}
	}

	void loadNotClass(DialogueFilterContext::ConditionalContext* context) {
		// This conditional bypasses compareValue comparisons.
		if (context->conditional->class_) {
			context->resultOverride = context->conditional->class_ != context->parentContext->speakerBaseActor->getClass();
		}
		else {
			context->resultOverride = true;
		}
	}

	void loadNotRace(DialogueFilterContext::ConditionalContext* context) {
		// This conditional bypasses compareValue comparisons.
		if (context->conditional->race) {
			context->resultOverride = context->conditional->race != context->parentContext->speakerBaseActor->getRace();
		}
		else {
			context->resultOverride = true;
		}
	}

	void loadNotCell(DialogueFilterContext::ConditionalContext* context) {
		// This conditional bypasses compareValue comparisons.
		auto playerCell = DataHandler::get()->currentCell;
		if (playerCell == nullptr) {
			context->resultOverride = false;
			return;
		}

		// Prefix comparison of cell id.
		const auto filterCell = context->conditional->cell;
		if (filterCell) {
			std::string_view cellId = filterCell->getObjectID();
			context->resultOverride = _strnicmp(playerCell->getObjectID(), cellId.data(), cellId.size()) != 0;
		}
		else {
			context->resultOverride = true;
		}
	}

	void loadNotLocal(DialogueFilterContext::ConditionalContext* context) {
		loadLocalVariableValue(context);
		context->flipCompareOperator();

		// If we have no value, because we don't have a script or the like, this is considered a pass.
		if (!context->compareValue) {
			context->resultOverride = true;
		}
	}

	ConditionalLoadFunction getLoadFunctionForType(DialogueConditional* conditional) {
		switch (conditional->type) {
		case DialogueConditionalType::Function: return loadFunctionFunction(conditional);
		case DialogueConditionalType::GlobalVar: return loadGlobalValue;
		case DialogueConditionalType::LocalVar: return loadLocalVariableValue;
		case DialogueConditionalType::JournalIndex: return loadJournalIndex;
		case DialogueConditionalType::ItemCount: return loadItemCount;
		case DialogueConditionalType::DeadActor: return loadDeadActorCount;
		case DialogueConditionalType::NotID: return loadNotID;
		case DialogueConditionalType::NotFaction: return loadNotFaction;
		case DialogueConditionalType::NotClass: return loadNotClass;
		case DialogueConditionalType::NotRace: return loadNotRace;
		case DialogueConditionalType::NotCell: return loadNotCell;
		case DialogueConditionalType::NotLocal: return loadNotLocal;
		}

		return nullptr;
	}

	void DialogueFilterContext::ConditionalContext::load() {
		auto fn = getLoadFunctionForType(conditional);
		if (fn) {
			fn(this);
		}
#if _DEBUG
		else {
			throw std::runtime_error("No load function could be found for dialogue filter context.");
		}
#endif
	}

	void DialogueFilterContext::ConditionalContext::flipCompareOperator() {
		switch (compareOperator) {
		case DialogueConditionalComparator::Equal:
			compareOperator = DialogueConditionalComparator::NotEqual;
			break;
		case DialogueConditionalComparator::NotEqual:
			compareOperator = DialogueConditionalComparator::Equal;
			break;
		case DialogueConditionalComparator::Greater:
			compareOperator = DialogueConditionalComparator::LessEqual;
			break;
		case DialogueConditionalComparator::GreaterEqual:
			compareOperator = DialogueConditionalComparator::Less;
			break;
		case DialogueConditionalComparator::Less:
			compareOperator = DialogueConditionalComparator::GreaterEqual;
			break;
		case DialogueConditionalComparator::LessEqual:
			compareOperator = DialogueConditionalComparator::Greater;
			break;
		}
	}
}
