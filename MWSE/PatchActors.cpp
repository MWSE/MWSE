#include "PatchActors.h"

#include "MemoryUtil.h"
#include "Log.h"

#include "TES3ActionData.h"
#include "TES3ActorAnimationController.h"
#include "TES3BodyPartManager.h"
#include "TES3Cell.h"
#include "TES3GameSetting.h"
#include "TES3MobManager.h"
#include "TES3MobilePlayer.h"
#include "TES3MobileProjectile.h"
#include "TES3DataHandler.h"
#include "TES3Reference.h"
#include "TES3WorldController.h"

namespace mwse::patch::actors {
	//
	// Patch: Unify athletics training.
	//

	void PatchUnifyAthleticsTraining() {
		auto worldController = TES3::WorldController::get();
		auto mobilePlayer = worldController->getMobilePlayer();

		auto athletics = &TES3::DataHandler::get()->nonDynamicData->skills[TES3::SkillID::Athletics];

		// If we're running, use the first progress.
		if (mobilePlayer->getMovementFlagRunning()) {
			mobilePlayer->exerciseSkill(TES3::SkillID::Athletics, athletics->progressActions[0] * worldController->deltaTime);
		}

		// If we're swimming, use the second progress.
		if (mobilePlayer->getMovementFlagSwimming()) {
			mobilePlayer->exerciseSkill(TES3::SkillID::Athletics, athletics->progressActions[1] * worldController->deltaTime);
		}
	}

	//
	// Patch: Unify sneak training.
	//

	void PatchUnifySneakTraining() {
		auto nonDynamicData = TES3::DataHandler::get()->nonDynamicData;

		// Decrement sneak use delay counter.
		*reinterpret_cast<float*>(0x7D16E0) = *reinterpret_cast<float*>(0x7D16E0) - nonDynamicData->GMSTs[TES3::GMST::fSneakUseDelay]->value.asFloat;

		// Excercise sneak.
		TES3::WorldController::get()->getMobilePlayer()->exerciseSkill(TES3::SkillID::Sneak, nonDynamicData->skills[TES3::SkillID::Sneak].progressActions[0]);
	}

	//
	// Patch: Player animation idles.
	//
	// Update animations for third person and first person player reference when idle mode is flagged.
	//

	const auto TES3_DataHandler_UpdateAllIdles = reinterpret_cast<void(__thiscall*)(TES3::DataHandler*)>(0x48AED0);
	const auto TES3_Reference_AnimIdleUpdate = reinterpret_cast<void(__thiscall*)(TES3::Reference*)>(0x4E6E20);
	void __stdcall PatchUpdateAllIdles() {
		TES3_DataHandler_UpdateAllIdles(TES3::DataHandler::get());

		auto worldController = TES3::WorldController::get();
		auto mobilePlayer = worldController->getMobilePlayer();
		if (mobilePlayer->actorFlags & TES3::MobileActorFlag::IdleAnim) {
			TES3_Reference_AnimIdleUpdate(mobilePlayer->reference);
			TES3_Reference_AnimIdleUpdate(mobilePlayer->firstPersonReference);
		}
	}

	//
	// Patch: Correctly initialize MobileProjectile tag/objectType
	// 
	// The copy constructor for MobileProjectiles fails to correctly set the object type correctly. This
	// ensures that it is set to the right value, instead of 0.
	//

	void __fastcall PatchInitializeMobileProjectileType(TES3::ObjectType::ObjectType* type) {
		*type = TES3::ObjectType::MobileProjectile;
	}

	//
	// Patch: Fall back to reference rotation values when initializing animation controllers without a scene node.
	// 
	// Leaving here since the reporter couldn't reproduce the crash on a new save, but we'll want information next time this happens.
	//

	const char* SafeGetObjectId(const TES3::BaseObject* object) {
		__try {
			return object->getObjectID();
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			return nullptr;
		}
	}

	const char* SafeGetSourceFile(const TES3::BaseObject* object) {
		__try {
			return object->getSourceFilename();
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			return nullptr;
		}
	}

	template <typename T>
	void safePrintObject(const char* title, const T* object, std::ostream& ss) {
		if (object) {
			auto id = SafeGetObjectId(object);
			auto source = SafeGetSourceFile(object);
			ss << "  " << title << ": " << (id ? id : "<memory corrupted>") << " (" << (source ? source : "<memory corrupted>") << ")\n";
			if (id) {
				log::prettyDump(object, ss);
			}
		}
		else {
			ss << "  " << title << ": nullptr\n";
		}
	}

	template <typename T>
	void safePrintObjectToLog(const char* title, const T* object) {
		safePrintObject(title, object, log::getLog());
	}

	void __fastcall PatchSetAnimControllerMobile(TES3::ActorAnimationController* animController, DWORD _EDX_, TES3::MobileActor* mobile) {
		if (mobile == nullptr) {
			return;
		}

		// Try to get more information about this crash.
		if (mobile->reference->getSceneGraphNode() == nullptr) {
			log::getLog() << "[MWSE] No scene graph found when attempting to add animation controller to reference. Doing what we can with the reference. Please report this to the #mwse channel in the Morrowind Modding Community discord." << std::endl;
			safePrintObjectToLog("Reference", mobile->reference);
		}

		// Perform overwritten code, but use getRotationMatrix to fall back to reference rotation values.
		animController->mobileActor = mobile;
		animController->animationData = mobile->getAnimationData();
		animController->groundPlaneRotation = mobile->reference->getRotationMatrix();
	}

	//
	// Patch: Fix crash when updating lights for a reference that has had a light unassigned.
	//

	static TES3::LightAttachmentNode* __fastcall PatchGetLightAttachmentIfItHasALight(TES3::Reference* reference) {
		const auto result = reference->getAttachedDynamicLight();
		if (result == nullptr) {
			return nullptr;
		}

		if (result->light == nullptr) {
			reference->deleteDynamicLightAttachment();
			return nullptr;
		}

		return result;
	}

	//
	// Patch: Guard against invalid light flicker/pulse updates.
	//

	const auto TES3_Light_UpdateFlickerPulse = reinterpret_cast<void(__thiscall*)(TES3::Light*, NI::Node*, float*, TES3::ItemData*)>(0x4D33D0);
	void __fastcall PatchEntityLightFlickerPulseUpdate(TES3::Light* light, DWORD _EDX_, NI::Node* sgNode, float* flickerPhase, TES3::ItemData* itemData) {
		if (sgNode == nullptr) {
#if _DEBUG
			log::getLog() << "[MWSE] Warning: Light '" << light->getObjectID() << "' attempting to update update flicker/phase without a scene graph node." << std::endl;
#endif
			return;
		}

		TES3_Light_UpdateFlickerPulse(light, sgNode, flickerPhase, itemData);
	}

	//
	// Patch: Modify proximity movement speed matching of AI followers to limit the speed match from going to zero on immobilized follow targets.
	//

	float __stdcall PatchGetAnimDataMovementSpeedCapped(TES3::AnimationData* animData) {
		// Restrict speed matching to be at least 60% of base animation speed.
		return std::max(0.6f, animData->movementSpeed);
	}

	__declspec(naked) void PatchMovementAnimSpeedMatching() {
		__asm {
			push eax
			call $					// Replace with call PatchGetAnimDataMovementSpeedCapped
			fstp[esp + 0x14]		// fst [targetMoveSpeed]
			fld[esp + 0x10]		// fld [finalMovementSpeed]
		}
	}
	const size_t PatchMovementAnimSpeedMatching_size = 0xE;

	//
	// Patch: Fix crash when trying to unequip a nocked projectile item while still using an item index for its position.
	// 
	// Before serializing, the nocked projectile is converted into an item index. Some mods may try to unequip the index
	// before it is resolved back into an actual projectile. This will prevent the crash.
	//

	static void __fastcall PatchUnequipIndexedProjectile(TES3::MobileActor* mobile) {
		auto& actionData = mobile->actionData;

		// Only call the destructor if the value can reasonably be a pointer.
		if (size_t(actionData.nockedProjectile) > 0x70000u) {
			actionData.nockedProjectile->vTable.mobileObject->destructor(actionData.nockedProjectile, true);
		}

		actionData.nockedProjectile = nullptr;
	}

	__declspec(naked) void PatchUnequipIndexedProjectileSetup() {
		__asm {
			mov ecx, ebx // Size: 0x2
		}
	}
	constexpr size_t PatchUnequipIndexedProjectileSetup_size = 0x2;


	//
	// Patch: Unsummoned actor cleanup
	//

	TES3::MobileActor* __fastcall cleanupUnsummonedActor(TES3::Reference* reference) {
		TES3::MobileActor* mobileActor = reference->getAttachedMobileActor();
		auto worldController = TES3::WorldController::get();
		worldController->mobManager->removeMob(reference);
		return mobileActor;
	}

	//
	// Patch: Fix crash when releasing a clone of a light with no reference.
	//        Also fix crash when the attachment scenegraph light pointer has been cleared.
	//
	// The first fix is mostly useful for creating VFXs using a light object as a base.
	// The second fix is to prevent a crash and try to identify the cause of the cleared pointer.
	//

	TES3::Attachment* __fastcall PatchReleaseLightEntityForReference(const TES3::Reference* reference) {
		if (reference == nullptr) {
			return nullptr;
		}

		auto attachment = static_cast<TES3::LightAttachment*>(reference->getAttachment(TES3::AttachmentType::Light));

		if (attachment && attachment->data->light == nullptr) {
			log::getLog() << "[MWSE] Crash prevented while cleaning up light reference to object '" <<
				reference->baseObject->objectID << "' in cell '" << reference->getCell()->getEditorName() << "'. " <<
				"Please report this to the #mwse channel in the Morrowind Modding Community discord." << std::endl;
			return nullptr;
		}

		return attachment;
	}

	void install() {
		using se::memory::genCallEnforced;
		using se::memory::genCallUnprotected;
		using se::memory::genNOPUnprotected;
		using se::memory::overrideVirtualTableEnforced;
		using se::memory::writePatchCodeUnprotected;

		// Patch: Unify athletics and sneak training.
		genCallUnprotected(0x569EE7, reinterpret_cast<DWORD>(PatchUnifyAthleticsTraining), 0xC6);
		genCallUnprotected(0x5683D0, reinterpret_cast<DWORD>(PatchUnifySneakTraining), 0x65);

		// Patch: Try to catch bogus collisions.
		auto MobileObject_Collision_clone = &TES3::MobileObject::Collision::clone;
		genCallEnforced(0x537107, 0x405450, *reinterpret_cast<DWORD*>(&MobileObject_Collision_clone));
		genCallEnforced(0x55F7C4, 0x405450, *reinterpret_cast<DWORD*>(&MobileObject_Collision_clone));
		genCallEnforced(0x55F818, 0x405450, *reinterpret_cast<DWORD*>(&MobileObject_Collision_clone));

		// Patch: Update player first and third person animations when the idle flag is pausing the controller.
		genCallUnprotected(0x41B836, reinterpret_cast<DWORD>(PatchUpdateAllIdles));

		// Patch: Correctly initialize MobileProjectile tag/objectType
		genCallEnforced(0x572444, 0x4EE8A0, reinterpret_cast<DWORD>(PatchInitializeMobileProjectileType));

		// Patch: Fall back to reference rotation values when initializing animation controllers without a scene node.
		genCallEnforced(0x521773, 0x53DE70, reinterpret_cast<DWORD>(PatchSetAnimControllerMobile));

		// Patch: Stop updating an actor animation controller if its mobile is detached during a state-machine update.
		auto ActorAnimationController_update = &TES3::ActorAnimationController::update;
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::ActorAnimController, offsetof(TES3::ActorAnimationController_VirtualTable, update), 0x53E070, *reinterpret_cast<DWORD*>(&ActorAnimationController_update));
		genCallEnforced(0x543A2B, 0x53E070, *reinterpret_cast<DWORD*>(&ActorAnimationController_update));

		// Patch: Fix up transparency.
		auto BodyPartManager_updateForReference = &TES3::BodyPartManager::updateForReference;
		genCallEnforced(0x46444C, 0x473EA0, *reinterpret_cast<DWORD*>(&BodyPartManager_updateForReference));
		genCallEnforced(0x4DA07C, 0x473EA0, *reinterpret_cast<DWORD*>(&BodyPartManager_updateForReference));

		// Patch: Guard against updating dynamic light attachments that have no actual light.
		genCallEnforced(0x485DA4, 0x4E5170, reinterpret_cast<DWORD>(PatchGetLightAttachmentIfItHasALight));
		genCallEnforced(0x485E87, 0x4E5170, reinterpret_cast<DWORD>(PatchGetLightAttachmentIfItHasALight));
		genCallEnforced(0x4D260C, 0x4E5170, reinterpret_cast<DWORD>(PatchGetLightAttachmentIfItHasALight));
		genCallEnforced(0x5243D6, 0x4E5170, reinterpret_cast<DWORD>(PatchGetLightAttachmentIfItHasALight));

		// Patch: Guard against invalid light flicker/pulse updates.
		genCallEnforced(0x49B75E, 0x4D33D0, reinterpret_cast<DWORD>(PatchEntityLightFlickerPulseUpdate));
		genCallEnforced(0x4D33BF, 0x4D33D0, reinterpret_cast<DWORD>(PatchEntityLightFlickerPulseUpdate));

		// Patch: Modify proximity movement speed matching of AI followers to limit the speed match from going to zero on immobilized follow targets.
		writePatchCodeUnprotected(0x540DBA, (BYTE*)&PatchMovementAnimSpeedMatching, PatchMovementAnimSpeedMatching_size);
		genCallUnprotected(0x540DBA + 1, reinterpret_cast<DWORD>(PatchGetAnimDataMovementSpeedCapped));

		// Patch: Fix crash when trying to unequip a nocked projectile item while still using an item index for its position.
		genNOPUnprotected(0x4968E1, 0x4968FB - 0x4968E1);
		writePatchCodeUnprotected(0x4968E1, (BYTE*)&PatchUnequipIndexedProjectileSetup, PatchUnequipIndexedProjectileSetup_size);
		genCallUnprotected(0x4968E1 + 0x2, reinterpret_cast<DWORD>(PatchUnequipIndexedProjectile));

		// Patch: Clean up unsummoned actors.
		genCallEnforced(0x466858, 0x4E5750, reinterpret_cast<DWORD>(cleanupUnsummonedActor));

		// Patch: Fix crash when releasing a clone of a light with no reference. Also fix crash when the attachment scenegraph light pointer has been cleared.
		genCallEnforced(0x4D260C, 0x4E5170, reinterpret_cast<DWORD>(PatchReleaseLightEntityForReference));

		// Patch: Clean up mobile collision data when a mobile is destroyed. Fixes probably a Todd-typo.
		genNOPUnprotected(0x55E55B, 0x55E55F - 0x55E55B);

		// Patch: Fix bound calculation.
		auto PhysicalObject_createBoundingBox = &TES3::PhysicalObject::createBoundingBox;
		genCallEnforced(0x49572E, 0x4EEFC0, *reinterpret_cast<DWORD*>(&PhysicalObject_createBoundingBox));
		genCallEnforced(0x495785, 0x4EEFC0, *reinterpret_cast<DWORD*>(&PhysicalObject_createBoundingBox));
		genCallEnforced(0x4D2324, 0x4EEFC0, *reinterpret_cast<DWORD*>(&PhysicalObject_createBoundingBox));
		genCallEnforced(0x4EF99F, 0x4EEFC0, *reinterpret_cast<DWORD*>(&PhysicalObject_createBoundingBox));
		genCallEnforced(0x4EFE70, 0x4EEFC0, *reinterpret_cast<DWORD*>(&PhysicalObject_createBoundingBox));
	}
}
