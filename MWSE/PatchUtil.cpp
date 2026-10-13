#include "PatchUtil.h"

#include "PatchActors.h"
#include "PatchAudio.h"
#include "PatchDialogue.h"
#include "PatchEngine.h"
#include "PatchInput.h"
#include "PatchIO.h"
#include "PatchMagic.h"
#include "PatchMWScript.h"
#include "PatchNI.h"
#include "PatchUI.h"

#include "MemoryUtil.h"
#include "mwOffsets.h"

#include "TES3AIData.h"
#include "TES3DataHandler.h"
#include "TES3GameFile.h"
#include "TES3MobileActor.h"
#include "TES3VoiceStreamer.h"
#include "TES3WorldController.h"

#include "NIAVObject.h"
#include "NICollisionGroup.h"
#include "NIPoint3.h"

#include "BuildDate.h"
#include "MWSEConfig.h"
#include "MWSEDefs.h"

namespace mwse::patch {

	//
	// Patch: Improve error reporting by including the source mod next to object IDs in load error messages.
	//

	static char tempErrorMessageObjectID[256];

	const char* __fastcall PatchGetImprovedObjectIdentifier(TES3::Object* object) {
		const auto id = object->getObjectID();
		const auto source = object->sourceMod ? object->sourceMod->filename : "no source";
		std::snprintf(tempErrorMessageObjectID, sizeof(tempErrorMessageObjectID), "%s' (%s)", id, source);
		return tempErrorMessageObjectID;
	}

	//
	// Patch: Make checking the object type of nullptr objects return an invalid type instead of crashing.
	//

	TES3::ObjectType::ObjectType __fastcall PatchSafeGetObjectType(TES3::ObjectType::ObjectType* objectType) {
		if (objectType == nullptr || size_t(objectType) == offsetof(TES3::BaseObject, objectType)) {
			return TES3::ObjectType::Invalid;
		}
		return *objectType;
	}

	void DoPatchSafeGetObjectType() {
		constexpr DWORD patchAddresses[] = { 0x41106E, 0x41CB71, 0x41CBA8, 0x41CBBA, 0x41CC93, 0x41CD68, 0x41CD80, 0x41D7A3, 0x41F59F, 0x42103B, 0x421373, 0x455165, 0x45517A, 0x4551B9, 0x45FA0E, 0x45FA22, 0x45FA3A, 0x45FAA9, 0x45FBC9, 0x45FBDD, 0x45FBF5, 0x45FC62, 0x45FC99, 0x45FD79, 0x45FE80, 0x4607A0, 0x4607B4, 0x4608E1, 0x4608F5, 0x460941, 0x463416, 0x464D25, 0x464D67, 0x464DC0, 0x464E4D, 0x464EB4, 0x4657D6, 0x465802, 0x465907, 0x465E2D, 0x465E45, 0x465FA2, 0x465FAF, 0x4666E7, 0x4667EB, 0x46F59C, 0x46FBA1, 0x46FBD4, 0x472F15, 0x47319A, 0x473263, 0x473334, 0x47367C, 0x47369B, 0x473787, 0x47393F, 0x4739A0, 0x473CCE, 0x473D84, 0x473DC5, 0x473E21, 0x473F7B, 0x474021, 0x474174, 0x474238, 0x484C62, 0x484DC2, 0x484ED0, 0x485FFD, 0x486365, 0x486433, 0x488B2F, 0x48B16E, 0x48B20C, 0x48B233, 0x48B253, 0x48B2E8, 0x48B4B1, 0x48B548, 0x48B712, 0x48B73A, 0x48B764, 0x48B86B, 0x48BA9C, 0x48BCBF, 0x48BD64, 0x48BFB3, 0x48C089, 0x48C2D4, 0x48C3D2, 0x48C463, 0x4951E5, 0x49526F, 0x495335, 0x495380, 0x4954BE, 0x4954F5, 0x495561, 0x4955A6, 0x4956AE, 0x495811, 0x495854, 0x4958F1, 0x49595C, 0x4959B7, 0x495A50, 0x495AF0, 0x495B6D, 0x495BC9, 0x495C0B, 0x495C67, 0x495CB6, 0x495D22, 0x495DE2, 0x495E9C, 0x495F20, 0x495F97, 0x495FCE, 0x496096, 0x4960FB, 0x496123, 0x49617B, 0x4961D7, 0x496219, 0x4962C8, 0x496314, 0x4964ED, 0x496515, 0x49655B, 0x49672A, 0x4968C1, 0x496E1D, 0x496E7D, 0x496EC8, 0x496F18, 0x496F58, 0x497030, 0x497BEB, 0x497D37, 0x497E72, 0x49817A, 0x498598, 0x498764, 0x499182, 0x49938C, 0x4999CE, 0x499ABF, 0x499AD1, 0x499AE3, 0x499CB3, 0x499D7D, 0x49A1BD, 0x49A5A2, 0x49A5F9, 0x49A758, 0x49A7D8, 0x49A7EA, 0x49A8AD, 0x49A8BA, 0x49AA36, 0x49AA48, 0x49ABE8, 0x49ABFA, 0x49ACAF, 0x49ACC1, 0x49B51C, 0x49B595, 0x49B640, 0x49B6BD, 0x49B793, 0x49D1BB, 0x49DC83, 0x49E8BE, 0x49E8CC, 0x49EA7E, 0x49EB4E, 0x49EC91, 0x49EFA8, 0x49FD2B, 0x4A01EB, 0x4A0BFE, 0x4A171B, 0x4A24EB, 0x4A359E, 0x4A407B, 0x4A4A8B, 0x4A525E, 0x4A526C, 0x4A56BB, 0x4A5C7B, 0x4A61BB, 0x4A67AB, 0x4A6C7B, 0x4A716B, 0x4A74DB, 0x4AB36B, 0x4AC78E, 0x4AC79C, 0x4AC88B, 0x4ACAD6, 0x4B01BD, 0x4B01DC, 0x4B02FE, 0x4B0851, 0x4B0C16, 0x4B0C5D, 0x4B0CA4, 0x4B0CE9, 0x4B0ED0, 0x4B1076, 0x4B119F, 0x4B15D0, 0x4B1636, 0x4B166B, 0x4B172E, 0x4B1E68, 0x4B5B44, 0x4B5BD4, 0x4B5CF9, 0x4B5D50, 0x4B5DC6, 0x4B5DEF, 0x4B5E22, 0x4B5E49, 0x4B606D, 0x4B6076, 0x4B8880, 0x4B8ADD, 0x4B9007, 0x4B92E2, 0x4B9520, 0x4B9A9D, 0x4B9D79, 0x4B9D98, 0x4B9E0A, 0x4B9FEC, 0x4BA03C, 0x4BA0B8, 0x4BA108, 0x4BA183, 0x4BA1A6, 0x4BA489, 0x4BA946, 0x4BA95A, 0x4BAEAB, 0x4BAF53, 0x4BB7E8, 0x4BB9F1, 0x4BBDFE, 0x4BBE1E, 0x4BBE2B, 0x4BBE3B, 0x4C0BE7, 0x4C0BF1, 0x4C0C49, 0x4C0C70, 0x4C1188, 0x4C18FB, 0x4C1A4A, 0x4C1E6F, 0x4C2096, 0x4C21A5, 0x4C37ED, 0x4C3B28, 0x4C3BEB, 0x4C3CCF, 0x4C3D29, 0x4C55CE, 0x4C5744, 0x4C5960, 0x4C5B68, 0x4C604C, 0x4C6408, 0x4C6817, 0x4C6C0C, 0x4C71E1, 0x4C722B, 0x4C73DD, 0x4C74F3, 0x4C79E7, 0x4C7E83, 0x4C848B, 0x4C84A6, 0x4CF9A0, 0x4D0DC3, 0x4D212B, 0x4D280E, 0x4D2847, 0x4D2C7D, 0x4D2CE5, 0x4D2D80, 0x4D332E, 0x4D734B, 0x4D8A27, 0x4D987E, 0x4D988C, 0x4D9AE4, 0x4D9CF2, 0x4D9DA5, 0x4D9DDE, 0x4DA00F, 0x4DA0A2, 0x4DBBE6, 0x4DBD77, 0x4DBF2C, 0x4DBF82, 0x4DBFE7, 0x4DC046, 0x4DC1B2, 0x4DC2E9, 0x4DC81B, 0x4DDD7B, 0x4DDE1C, 0x4DDEAA, 0x4DDEF2, 0x4DE077, 0x4DE081, 0x4DE08B, 0x4DE944, 0x4DEB23, 0x4DEC21, 0x4DF596, 0x4DF84F, 0x4E0D48, 0x4E12FC, 0x4E1646, 0x4E1683, 0x4E173A, 0x4E17E4, 0x4E19C1, 0x4E1A46, 0x4E2B0F, 0x4E491A, 0x4E4928, 0x4E4936, 0x4E497F, 0x4E4AD9, 0x4E4DCC, 0x4E5687, 0x4E5786, 0x4E633D, 0x4E6E83, 0x4E7136, 0x4E7199, 0x4E77A3, 0x4E79CF, 0x4E7E0F, 0x4E7E21, 0x4E7E33, 0x4E7E9F, 0x4E7EFC, 0x4E7F60, 0x4E813B, 0x4E839A, 0x4E8503, 0x4E85C5, 0x4E85DB, 0x4E86CA, 0x4E8BC6, 0x4E8C94, 0x4E8D62, 0x4E8F9C, 0x4E90B9, 0x4E91D2, 0x4E9452, 0x4E9710, 0x4E9754, 0x4E994A, 0x4E9981, 0x4E9A8A, 0x4EAF5E, 0x4EB0ED, 0x4EB16D, 0x4EB183, 0x4EB199, 0x4EB1CD, 0x4EB62D, 0x4EB816, 0x4EBB16, 0x4EBB28, 0x4EBE4F, 0x4EBEA5, 0x4F260B, 0x4F27BF, 0x4F7229, 0x4F7468, 0x4F9FF1, 0x4FA0A6, 0x4FA0B4, 0x4FA0C2, 0x4FA22C, 0x4FA2DF, 0x4FE0BE, 0x4FE0CC, 0x4FE0E8, 0x4FE0F6, 0x4FE110, 0x4FE252, 0x4FE2AD, 0x4FE2BB, 0x4FE2C9, 0x4FE346, 0x4FE39B, 0x4FECD6, 0x4FECE4, 0x4FED04, 0x4FED12, 0x4FED30, 0x5057F2, 0x506753, 0x507303, 0x507323, 0x50733F, 0x507764, 0x5077BA, 0x508054, 0x5080B0, 0x50856E, 0x5085D5, 0x5086B6, 0x508741, 0x50893D, 0x50899D, 0x508A1A, 0x508A75, 0x508ACB, 0x508B23, 0x508CB5, 0x508CC8, 0x508D5A, 0x50904B, 0x509627, 0x509664, 0x509707, 0x5098EF, 0x509A52, 0x50A4CF, 0x50A5FD, 0x50A722, 0x50A858, 0x50A910, 0x50A9C8, 0x50AA32, 0x50AAD3, 0x50ABD0, 0x50BA30, 0x50BC8B, 0x50BE1C, 0x50BE88, 0x50BF8C, 0x50C196, 0x50C233, 0x50C248, 0x50C256, 0x50C643, 0x50EC97, 0x50ECC4, 0x50ED38, 0x50EFC6, 0x50F03E, 0x5116C0, 0x51248A, 0x512498, 0x51385A, 0x51415C, 0x514A22, 0x514D7B, 0x514FD1, 0x515052, 0x51522F, 0x5155D0, 0x51572B, 0x515DDB, 0x5165F7, 0x516E41, 0x517890, 0x51793C, 0x5179A9, 0x517C13, 0x518ADE, 0x518B6B, 0x518BC1, 0x5206EA, 0x520744, 0x520896, 0x5208F0, 0x5234C9, 0x525092, 0x527F5F, 0x528082, 0x52B241, 0x52B333, 0x52C078, 0x52C0AF, 0x52C1A5, 0x52C1B9, 0x52C844, 0x52CCD7, 0x52CDB4, 0x52CDC2, 0x52CDD0, 0x52CDDE, 0x52CDEC, 0x52CEE8, 0x533691, 0x533F61, 0x53753A, 0x53767E, 0x537ACF, 0x53836F, 0x53843E, 0x53AFF7, 0x53DEEA, 0x54C9D7, 0x54CB05, 0x54D342, 0x54FF4C, 0x550EB6, 0x551B06, 0x55A52B, 0x55F120, 0x55F219, 0x55F4E3, 0x55F5D8, 0x5634FD, 0x563755, 0x563819, 0x565FC4, 0x5677C9, 0x5699DF, 0x569A96, 0x569ACD, 0x569B9F, 0x56B1C2, 0x56B1D0, 0x56B1DE, 0x56B1EC, 0x56B233, 0x56B241, 0x56B2B6, 0x56B2C8, 0x56B2D6, 0x56B2E4, 0x56B32F, 0x56B33D, 0x56B3FF, 0x56B40D, 0x56B41B, 0x56B429, 0x56B470, 0x56B47E, 0x56B4F1, 0x56B503, 0x56B511, 0x56B51F, 0x56B56A, 0x56B578, 0x56C149, 0x56C208, 0x56C375, 0x56C42C, 0x56F05D, 0x573435, 0x5738F1, 0x5743B7, 0x574C92, 0x58FD49, 0x58FEA7, 0x590449, 0x590DFB, 0x590E5E, 0x590E73, 0x590E85, 0x590E97, 0x590EA9, 0x590EBB, 0x590EC9, 0x590ED7, 0x590EE5, 0x590EF3, 0x590F01, 0x590F0F, 0x590F1D, 0x590F2B, 0x590F3D, 0x590FE0, 0x5910B6, 0x591637, 0x5916C9, 0x5917FE, 0x591EB1, 0x592B54, 0x595749, 0x59575A, 0x59A16F, 0x59A19F, 0x59A1CF, 0x59A1FF, 0x59A22F, 0x59ACA8, 0x59D1A9, 0x5A3C98, 0x5A3CBF, 0x5A3CE6, 0x5A4704, 0x5A5492, 0x5A5F10, 0x5A63FF, 0x5A6414, 0x5B447B, 0x5B4489, 0x5B44DA, 0x5B4719, 0x5B5002, 0x5B502C, 0x5B5067, 0x5B51DD, 0x5B5409, 0x5B54CD, 0x5B56E8, 0x5B59DF, 0x5B5C6D, 0x5B5E06, 0x5B6012, 0x5B6028, 0x5B7224, 0x5B76C5, 0x5B7A8E, 0x5B7CA5, 0x5B7CD6, 0x5B7DCE, 0x5B7DE7, 0x5BF3CC, 0x5C47FD, 0x5C497A, 0x5C4A1E, 0x5C4AC5, 0x5C55DA, 0x5C615A, 0x5C6171, 0x5C61B2, 0x5C6B4E, 0x5CA998, 0x5CA9BF, 0x5CA9E6, 0x5CB836, 0x5CC782, 0x5CD901, 0x5CE309, 0x5D095E, 0x5D098F, 0x5D142B, 0x5D36B9, 0x5D36C7, 0x5D36D5, 0x5D3790, 0x5D3865, 0x5D3A5E, 0x5D3C61, 0x5D4069, 0x5D4505, 0x5D450F, 0x5D461F, 0x5D4628, 0x5D463B, 0x5D4644, 0x5E0090, 0x5E00D5, 0x5E0738, 0x5E212A, 0x5E2198, 0x5E31AA, 0x5E31EF, 0x5E38BE, 0x5E4355, 0x5ED061, 0x5ED073, 0x5ED0A0, 0x5ED0E4, 0x5ED0F6, 0x5ED123, 0x5ED157, 0x5F72E9, 0x5F72FA, 0x5FE2DE, 0x608759, 0x608AA7, 0x60AFF8, 0x60D7A7, 0x60D8F8, 0x60E365, 0x61527E, 0x615290, 0x631A50, 0x631A5C, 0x631B70, 0x631B7A, 0x631B91, 0x631B9B, 0x631CB8, 0x631CC4, 0x631DF8, 0x631E02, 0x631E1B, 0x631E25, 0x63261D, 0x633413, 0x6335FA, 0x63365D, 0x633740, 0x63384E, 0x633888, 0x633914, 0x6339A5, 0x633C98, 0x633CC1, 0x633D53, 0x633DB6, 0x635373, 0x6EA072, 0x6EA08C, 0x6EA097, 0x6EA0BA, 0x6EA1B3, 0x6EA721, 0x6EA794, 0x6EA7A0, 0x6EA7AB, 0x6EA7B7, 0x6EA7CB, 0x6EA7D7, 0x6EA7DF, 0x6EA8D8 };
		for (auto address : patchAddresses) {
			se::memory::genCallEnforced(address, 0x4EE8B0, reinterpret_cast<DWORD>(PatchSafeGetObjectType));
		}
	}

	//
	// Patch: Optimize map rendering when crossing cell borders
	// 
	// renderCellMapTile re-runs UpdateProperties/UpdateEffects on the same map render-target root and Update on the
	// same nodeLand once per ring tile; a batch bracketed around updateCellThreadLoader runs each traversal once per
	// cross instead.
	//

	// Bracket the local-map compositor so the fog cache is active while it runs.
	static const auto TES3_ui_MenuMap_updateMapRender = reinterpret_cast<void(__cdecl*)()>(0x5E99C0);
	static void __cdecl PatchOptimizeMapUpdates_UpdateMapRenderCached() {
		TES3::WorldControllerRenderTarget::beginFogCache();
		TES3_ui_MenuMap_updateMapRender();
		TES3::WorldControllerRenderTarget::endFogCache();
	}

	static bool sHoistBatchActive = false;
	static bool sHoistDoneProps = false;
	static bool sHoistDoneFx = false;
	static bool sHoistDoneLand = false;

	static int __fastcall PatchOptimizeMapUpdates_WrapCellThreadLoader(TES3::DataHandler* dataHandler, DWORD _EDX_, NI::Point3* position) {
		sHoistBatchActive = true;
		sHoistDoneProps = false;
		sHoistDoneFx = false;
		sHoistDoneLand = false;
		const int result = dataHandler->updateCellThreadLoader(position);
		sHoistBatchActive = false;
		return result;
	}

	static void __fastcall PatchOptimizeMapUpdates_WrapUpdateProperties(NI::AVObject* node) {
		if (sHoistBatchActive && sHoistDoneProps) {
			return;
		}
		node->updateProperties();
		sHoistDoneProps = true;
	}

	static void __fastcall PatchOptimizeMapUpdates_WrapUpdateEffects(NI::AVObject* node) {
		if (sHoistBatchActive && sHoistDoneFx) {
			return;
		}
		node->updateEffects();
		sHoistDoneFx = true;
	}

	static void __fastcall PatchOptimizeMapUpdates_WrapUpdate(NI::AVObject* node, DWORD _EDX_, float fTime, int updateControllers, int updateChildren) {
		if (sHoistBatchActive && sHoistDoneLand) {
			return;
		}
		node->update(fTime, updateControllers, updateChildren);
		sHoistDoneLand = true;
	}

	//
	// Patch: Optimize relighting of actors during cell transition.
	// 
	// Actor teardown: skip markActorCorpse's redundant AIPlanner::enterLeaveSimulation when the actor has
	// already left simulation, and memoize resetCollisionGroup's loop-invariant root-collision-node search.
	//

	static NI::AVObject* sRootSearchArg = nullptr;
	static NI::Node* sRootSearchResult = nullptr;

	static void __fastcall ActorTeardownDedupLeaveSimulation(TES3::AIPlanner* aiPlanner, DWORD _EDX_, bool active) {
		if (!active && aiPlanner) {
			TES3::MobileActor* mobileActor = aiPlanner->mobileActor;
			if (mobileActor && !mobileActor->getMobileActorFlag(TES3::MobileActorFlag::ActiveInSimulation)) {
				return;
			}
		}
		aiPlanner->enterLeaveSimulation(active);
	}

	static void __fastcall ActorTeardownResetRootSearchCache(NI::CollisionGroup* group) {
		sRootSearchArg = nullptr;
		group->removeAll();
	}

	static NI::Node* __cdecl ActorTeardownMemoizedRootSearch(NI::AVObject* sceneNode) {
		if (sceneNode == sRootSearchArg) {
			return sRootSearchResult;
		}
		sRootSearchResult = sceneNode->findRootCollisionNode();
		sRootSearchArg = sceneNode;
		return sRootSearchResult;
	}

	//
	// Install all the patches.
	//

	void installPatches() {
		using se::memory::genCallEnforced;
		using se::memory::genCallUnprotected;
		using se::memory::genJumpEnforced;
		using se::memory::writeBytesUnprotected;
		using se::memory::writeValueEnforced;
		
		actors::install();
		audio::install();
		dialogue::install();
		engine::install();
		input::install();
		io::install();
		magic::install();
		mwscript::install();
		ni::install();
		ui::install();

		// Patch: Improve error reporting by including the source mod next to object IDs in load error messages.
		// In Cell::loadReference:
		const BYTE patchImprovedErrorIDArgs0[] = { 0x8B, 0xCE };
		const BYTE patchImprovedErrorIDArgs1[] = { 0x8B, 0xCD, 0x90 };
		const BYTE patchImprovedErrorIDArgs2[] = { 0x50, 0x8B, 0xCE };
		const BYTE patchImprovedErrorIDArgs3[] = { 0x51, 0x8B, 0xCA };
		const BYTE patchImprovedErrorIDArgs4[] = { 0x8B, 0x32 };
		const BYTE patchImprovedErrorIDArgs5[] = { 0x8B, 0xCA };
		const BYTE patchImprovedErrorIDArgs6[] = { 0x51, 0x8B, 0xCA };
		genCallUnprotected(0x4DE71A, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DE78D, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DE98C, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4DEA86, patchImprovedErrorIDArgs0, sizeof(patchImprovedErrorIDArgs0));
		genCallUnprotected(0x4DEA88, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4DED73, patchImprovedErrorIDArgs1, sizeof(patchImprovedErrorIDArgs1));
		genCallUnprotected(0x4DED76, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DF1A5, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DF21B, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4DF42C, patchImprovedErrorIDArgs2, sizeof(patchImprovedErrorIDArgs2));
		genCallUnprotected(0x4DF42F, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4DF454, patchImprovedErrorIDArgs3, sizeof(patchImprovedErrorIDArgs3));
		genCallUnprotected(0x4DF457, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4DF551, patchImprovedErrorIDArgs4, sizeof(patchImprovedErrorIDArgs4));
		genCallUnprotected(0x4DF553, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DF5FD, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4DF7A4, patchImprovedErrorIDArgs5, sizeof(patchImprovedErrorIDArgs5));
		genCallUnprotected(0x4DF7A6, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4DF7DB, patchImprovedErrorIDArgs6, sizeof(patchImprovedErrorIDArgs6));
		genCallUnprotected(0x4DF7DE, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DF89A, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DF922, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DF9E4, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DFAA3, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DFB7C, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DFC0E, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DFCB5, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DFDB3, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DFEAA, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DFF50, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DFFC1, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4E035E, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4E037C, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4E08E7, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		// In link-resolve functions:
		const BYTE patchImprovedErrorIDArgs7[] = { 0x8B, 0xCE };
		const BYTE patchImprovedErrorIDArgs8[] = { 0x8B, 0xCF };
		const BYTE patchImprovedErrorIDArgs9[] = { 0x8B, 0xCE };
		const BYTE patchImprovedErrorIDArgs10[] = { 0x8B, 0xCD, 0x90 };
		const BYTE patchImprovedErrorIDArgs11[] = { 0x8B, 0xCB };
		const BYTE patchImprovedErrorIDArgs12[] = { 0x8B, 0xCD, 0x90 };
		const BYTE patchImprovedErrorIDArgs13[] = { 0x8B, 0xCE };
		const BYTE patchImprovedErrorIDArgs14[] = { 0x8B, 0xCE };
		const BYTE patchImprovedErrorIDArgs15[] = { 0x8B, 0xCE };
		const BYTE patchImprovedErrorIDArgs16[] = { 0x8B, 0xCE };
		const BYTE patchImprovedErrorIDArgs17[] = { 0x8B, 0xCF };
		const BYTE patchImprovedErrorIDArgs18[] = { 0x8B, 0xCE };
		const BYTE patchImprovedErrorIDArgs19[] = { 0x8B, 0xCE };
		const BYTE patchImprovedErrorIDArgs20[] = { 0x8B, 0xCB };
		genCallUnprotected(0x40FCA5, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x480DDF, patchImprovedErrorIDArgs7, sizeof(patchImprovedErrorIDArgs7));
		genCallUnprotected(0x480DE1, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x49D121, patchImprovedErrorIDArgs8, sizeof(patchImprovedErrorIDArgs8));
		genCallUnprotected(0x49D123, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x49FAEF, patchImprovedErrorIDArgs9, sizeof(patchImprovedErrorIDArgs9));
		genCallUnprotected(0x49FAF1, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4A0686, patchImprovedErrorIDArgs10, sizeof(patchImprovedErrorIDArgs10));
		genCallUnprotected(0x4A0689, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4A212E, patchImprovedErrorIDArgs11, sizeof(patchImprovedErrorIDArgs11));
		genCallUnprotected(0x4A2130, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4A2F96, patchImprovedErrorIDArgs12, sizeof(patchImprovedErrorIDArgs12));
		genCallUnprotected(0x4A2F99, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4A53EF, patchImprovedErrorIDArgs13, sizeof(patchImprovedErrorIDArgs13));
		genCallUnprotected(0x4A53F1, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4A5995, patchImprovedErrorIDArgs14, sizeof(patchImprovedErrorIDArgs14));
		genCallUnprotected(0x4A5997, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4A5F2F, patchImprovedErrorIDArgs15, sizeof(patchImprovedErrorIDArgs15));
		genCallUnprotected(0x4A5F31, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4A64BF, patchImprovedErrorIDArgs16, sizeof(patchImprovedErrorIDArgs16));
		genCallUnprotected(0x4A64C1, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4AC3FA, patchImprovedErrorIDArgs17, sizeof(patchImprovedErrorIDArgs17));
		genCallUnprotected(0x4AC3FC, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4CF800, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4D0B60, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4D1D56, patchImprovedErrorIDArgs18, sizeof(patchImprovedErrorIDArgs18));
		genCallUnprotected(0x4D1D58, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4D7284, patchImprovedErrorIDArgs19, sizeof(patchImprovedErrorIDArgs19));
		genCallUnprotected(0x4D7286, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4E6C0C, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4F2132, patchImprovedErrorIDArgs20, sizeof(patchImprovedErrorIDArgs20));
		genCallUnprotected(0x4F2134, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));

		// Patch: Fix missing nullptr check when determining object types.
		DoPatchSafeGetObjectType();

		// Patch: Fix crash when trying to draw cell markers that don't fit on the map.
		// Compatible with MCP map expansion.
		writeValueEnforced<DWORD>(0x4C85BC + 2, 0x200, 0x1F7);
		writeValueEnforced<DWORD>(0x4C85CC + 1, 0x200, 0x1F7);

		// Patch: Optimize map rendering when crossing cell borders.
		genJumpEnforced(0x420415, 0x5E99C0, reinterpret_cast<DWORD>(&PatchOptimizeMapUpdates_UpdateMapRenderCached)); // MapController::updateMapRender tail-jmp
		genCallEnforced(0x5E9757, 0x5E99C0, reinterpret_cast<DWORD>(&PatchOptimizeMapUpdates_UpdateMapRenderCached)); // ui_showMapMenu
		genCallEnforced(0x5F0BF2, 0x5E99C0, reinterpret_cast<DWORD>(&PatchOptimizeMapUpdates_UpdateMapRenderCached)); // ui_MenuMapNoteEdit_onOK
		genCallEnforced(0x5F0D32, 0x5E99C0, reinterpret_cast<DWORD>(&PatchOptimizeMapUpdates_UpdateMapRenderCached)); // ui_MenuMapNoteEdit_onDeleteNote
		auto WorldControllerRenderTarget_getFogOfWarPixelCached = &TES3::WorldControllerRenderTarget::getFogOfWarPixelCached;
		genCallEnforced(0x5EE104, 0x42FE20, *reinterpret_cast<DWORD*>(&WorldControllerRenderTarget_getFogOfWarPixelCached)); // isPositionUncoveredByFogOfWar
		genCallEnforced(0x5EE286, 0x42FE20, *reinterpret_cast<DWORD*>(&WorldControllerRenderTarget_getFogOfWarPixelCached));
		genCallEnforced(0x41B771, 0x486620, reinterpret_cast<DWORD>(&PatchOptimizeMapUpdates_WrapCellThreadLoader)); // mainLoop
		genCallEnforced(0x485731, 0x486620, reinterpret_cast<DWORD>(&PatchOptimizeMapUpdates_WrapCellThreadLoader)); // sub_485680
		genCallEnforced(0x48915D, 0x486620, reinterpret_cast<DWORD>(&PatchOptimizeMapUpdates_WrapCellThreadLoader)); // cellChangeToInterior
		genCallEnforced(0x420089, 0x6EB0E0, reinterpret_cast<DWORD>(&PatchOptimizeMapUpdates_WrapUpdateProperties));
		genCallEnforced(0x420090, 0x6EB380, reinterpret_cast<DWORD>(&PatchOptimizeMapUpdates_WrapUpdateEffects));
		genCallEnforced(0x42009E, 0x6EB000, reinterpret_cast<DWORD>(&PatchOptimizeMapUpdates_WrapUpdate));

		// Patch: Optimize relighting of actors during cell transition.
		auto DataHandler_relightExteriorCellsAfterCross = &TES3::DataHandler::relightExteriorCellsAfterCross;
		genCallEnforced(0x486FBE, 0x485C50, *reinterpret_cast<DWORD*>(&DataHandler_relightExteriorCellsAfterCross)); // updateCellThreadLoader -> updateAllLights
		genCallEnforced(0x56F0AF, 0x565350, reinterpret_cast<DWORD>(&ActorTeardownDedupLeaveSimulation));
		genCallEnforced(0x55ECC0, 0x6FD680, reinterpret_cast<DWORD>(&ActorTeardownResetRootSearchCache));
		genCallEnforced(0x55ED1B, 0x6A3080, reinterpret_cast<DWORD>(&ActorTeardownMemoizedRootSearch));
	}

	void installPostLuaPatches() {
		audio::installPostLua();
		input::installPostLua();
		ni::installPostLua();
	}

	void installPostInitializationPatches() {
		// Patch: Async voiceover loading. Eliminates the main-thread MP3 decode
		// spike when NPCs greet the player. Installed here (rather than in
		// installPatches) so DataHandler::get() is valid when the worker thread
		// first acquires criticalSectionAudioEvents.
		voice::install();
	}

	void uninstallPatches() {
		// Patch: Async voiceover loading — stop the worker before the engine
		// starts tearing down DSound / DataHandler.
		voice::shutdown();
	}
}
