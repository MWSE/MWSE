#include "PatchUtil.h"

#include "LuaUtil.h"
#include "MemoryUtil.h"
#include "TES3Util.h"

#include "NIPoint2.h"

#include "TES3Class.h"
#include "TES3MobilePlayer.h"
#include "TES3UIElement.h"
#include "TES3UIInventoryTile.h"
#include "TES3UIMenuController.h"
#include "TES3WorldController.h"

namespace mwse::patch::ui {
	//
	// Patch: Fix crash with paper doll equipping/unequipping.
	//
	// In this patch, the tile associated with the stack may have been deleted, but the property to the TES3::ItemData 
	// remains. If the memory is reallocated it will fill with garbage, and cause a crash. The tile property, however,
	// is properly deleted. So we look for that instead, and return its associated item data (which is the same value).
	//! TODO: Find out where it's being deleted, and also delete the right property.
	//
	TES3::UI::PropertyValue* __fastcall PatchPaperdollTooltipCrashFix(TES3::UI::Element* self, DWORD _UNUSUED_, TES3::UI::PropertyValue* propValue, TES3::UI::Property prop, TES3::UI::PropertyType propType, const TES3::UI::Element* element = nullptr, bool checkInherited = false) {
		auto tileProp = self->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(0x7D3A70));
		auto tile = reinterpret_cast<TES3::UI::InventoryTile*>(tileProp.ptrValue);

		if (tile == nullptr) {
			propValue->ptrValue = nullptr;
		}
		else {
			propValue->ptrValue = tile->itemData;
		}

		return propValue;
	}

	//
	// Patch: Fix crash when saving menu position if the derived key name is too long.
	//

	__declspec(naked) void PatchSaveMenuPositionRightPad() {
		__asm {
			// Clamp eax <= 32
			cmp eax, 32
			jbe clamped
			mov eax, 32
			clamped:
			// Null terminate padding so that key length+padding length is at least 32
			lea ecx, [esp + 4 + 0x64]
				sub ecx, eax
				mov byte ptr[ecx], 0
				ret
		}
	}

	//
	// Patch: Slight optimization to journal updating.
	//

	__declspec(naked) void PatchSwapJournalUpdateCheckForSpeakerOrder() {
		__asm {
			// Check speaker first.
			mov eax, [edi + 0x28]            // Size: 0x3
			test eax, eax                    // Size: 0x2
			jnz $ + 0xE5                     // Size: 0x6

			// Then bother to check to see if we have text.
			mov ecx, edi                     // Size: 0x2
			nop							     // Size: 0x5. Replaced with a call generation. Can't do so here, because offsets aren't accurate.
			nop							     // ^
			nop							     // ^
			nop							     // ^
			nop							     // ^
			test eax, eax                    // Size: 0x2
			jz $ + 0xD6                      // Size: 0x6
		}
	}
	constexpr auto PatchSwapJournalUpdateCheckForSpeakerOrder_size = (0x4B2FF1u - 0x4B2FD7u);

	//
	// Patch: Support custom class images.
	//

	__declspec(naked) void PatchAddCustomClassImageSupportSetup() {
		__asm {
			mov ecx, esi			// Size: 0x2. The Class*.
			mov edx, ebx			// Size: 0x2. The parent element pointer.
			nop						// Size: 0x5. Replaced with a call generation. Can't do so here, because offsets aren't accurate.
			nop						// ^
			nop						// ^
			nop						// ^
			nop						// ^
		}
	}
	constexpr auto PatchAddCustomClassImageSupport_size = 0x9;

	TES3::UI::Element* __fastcall PatchAddCustomClassImageSupport(const TES3::Class* charClass, TES3::UI::Element* parent) {
		auto result = charClass->getLevelUpImage();
		return parent->createImage(TES3::UI::ID_NULL, result.c_str(), false);
	}


	//
	// Patch: UI element image mirroring on negative image scale.
	// 

	// Mirror image texcoords with negative image scale.
	void __cdecl PatchUIElementTexcoordWrite(TES3::UI::Element* element, NI::Point2* texCoords) {
		float left = 0.0f, top = 0.0f, right = 1.0f, bottom = 1.0f;

		if (element->imageScaleX < 0) {
			std::swap(left, right);
		}
		if (element->imageScaleY < 0) {
			std::swap(top, bottom);
		}
		texCoords[0].x = left;
		texCoords[0].y = top;
		texCoords[1].x = left;
		texCoords[1].y = bottom;
		texCoords[2].x = right;
		texCoords[2].y = top;
		texCoords[3].x = right;
		texCoords[3].y = bottom;
	}

	// Change pixel width/height calculation to floor(abs(imageScale{X,Y} * texture{Width,Height}) + 0.5)
	const float f_half = 0.5f;
	__declspec(naked) void PatchUIUpdateLayoutImageContent1() {
		__asm {
			fmulp	st(1), st			// imageScale * textureDimension
			fabs						// abs
			fadd[f_half]			// + 0.5
			fstp	qword ptr[esp]		// double argument for floor
		}
	}
	const size_t PatchUIUpdateLayoutImageContent1_size = 0xD;

	// Replace texcoord writer.
	__declspec(naked) void PatchUIUpdateLayoutImageContent2() {
		__asm {
			push eax		// texcoord data pointer
			push esi		// UiElement pointer
			nop				// call to patch placeholder
			nop
			nop
			nop
			nop
			add esp, 8
			xor ecx, ecx
			jmp $ + 0x51
		}
	}
	const size_t PatchUIUpdateLayoutImageContent2_size = 0x11;

	//
	// Patch: Log stack traces of problematic UI pointer issues.
	//

	void __cdecl PatchLogUIMemoryPointerErrors(const char* message) {
		lua::logStackTrace("Lua traceback at time of invalid access:");
		tes3::logErrorAndSavePoint(message);
	}

	//
	// Patch: Fix MenuEnchant menu pointer on failed enchant
	//

	static TES3::UI::Element* __cdecl PatchEnchantingMenuPointer(TES3::UI::UI_ID id) {
		const auto menuInputController = TES3::WorldController::get()->menuController->menuInputController;
		if (!menuInputController->textInputFocus->isValid()) {
			menuInputController->textInputFocus = nullptr;
		}

		// Call original code.
		return TES3::UI::findMenu(id);
	}

	//
	// Patch: IDK something
	//

	static TES3::UI::InventoryTile* __fastcall PatchFindInventoryTileWithForcedRefreshForPlayer(TES3::InventoryData* inventoryData, DWORD _EDX_, const TES3::Item* item) {
		const auto player = TES3::WorldController::get()->getMobilePlayer()->reference;
		inventoryData->refreshForReference(player, 2);
		return inventoryData->findTile(item);
	}

	void install() {
		using se::memory::genCallEnforced;
		using se::memory::genCallUnprotected;
		using se::memory::genNOPUnprotected;
		using se::memory::writePatchCodeUnprotected;

		// Patch: Crash fix for help text for paperdolls.
		genCallEnforced(0x5CDFD0, 0x581440, reinterpret_cast<DWORD>(PatchPaperdollTooltipCrashFix));

		// Patch: Fix crash when saving menu position if the derived key name is too long.
		genCallUnprotected(0x597061, reinterpret_cast<DWORD>(PatchSaveMenuPositionRightPad), 0x6);
		genNOPUnprotected(0x59706C, 0x59706F - 0x59706C);

		// Patch: Slight journal update optimization.
		writePatchCodeUnprotected(0x4B2FD7, (BYTE*)&PatchSwapJournalUpdateCheckForSpeakerOrder, PatchSwapJournalUpdateCheckForSpeakerOrder_size);
		genCallUnprotected(0x4B2FD7 + 0xD, 0x4B1B80);

		// Patch: Support custom class images.
		genNOPUnprotected(0x5AF047, 0x5AF583 - 0x5AF047);
		writePatchCodeUnprotected(0x5AF047, (BYTE*)&PatchAddCustomClassImageSupportSetup, PatchAddCustomClassImageSupport_size);
		genCallUnprotected(0x5AF047 + 0x4, reinterpret_cast<DWORD>(PatchAddCustomClassImageSupport), 0x9);

		// Patch: UI element image mirroring on negative image scale.
		writePatchCodeUnprotected(0x57DE02, (BYTE*)&PatchUIUpdateLayoutImageContent1, PatchUIUpdateLayoutImageContent1_size);
		writePatchCodeUnprotected(0x57DE3F, (BYTE*)&PatchUIUpdateLayoutImageContent1, PatchUIUpdateLayoutImageContent1_size);
		writePatchCodeUnprotected(0x57E1E8, (BYTE*)&PatchUIUpdateLayoutImageContent2, PatchUIUpdateLayoutImageContent2_size);
		genCallUnprotected(0x57E1E8 + 0x2, reinterpret_cast<DWORD>(PatchUIElementTexcoordWrite));

		// Provide lua stack traces with invalid UI access.
		genCallEnforced(0x581484, 0x476E20, reinterpret_cast<DWORD>(PatchLogUIMemoryPointerErrors));
		genCallEnforced(0x582DFA, 0x476E20, reinterpret_cast<DWORD>(PatchLogUIMemoryPointerErrors));

		// Patch: Fix invalid UI memory pointer.
		genCallEnforced(0x5C48DB, 0x595370, reinterpret_cast<DWORD>(PatchEnchantingMenuPointer));

		// Patch: Prevent quickslot failures from stale inventory data.
		genCallEnforced(0x608608, 0x633E80, reinterpret_cast<DWORD>(PatchFindInventoryTileWithForcedRefreshForPlayer));
	}
}
