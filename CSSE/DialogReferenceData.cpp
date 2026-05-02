#include "DialogReferenceData.h"

#include "LogUtil.h"
#include "MemoryUtil.h"
#include "StringUtil.h"
#include "WinUIUtil.h"

#include "DialogProcContext.h"

#include "CSCell.h"
#include "CSDataHandler.h"
#include "CSFaction.h"
#include "CSGlobalVariable.h"
#include "CSNPC.h"
#include "CSRecordHandler.h"
#include "CSReference.h"
#include "CSWeapon.h"

namespace se::cs::dialog::reference_data {

	const auto GetClampedDialogItemInteger = reinterpret_cast<int(__cdecl*)(HWND, int, int, int)>(0x4044C6);

	//
	// Configuration
	//

	constexpr auto ENABLE_ALL_OPTIMIZATIONS = true;
	constexpr auto LOG_PERFORMANCE_RESULTS = false;

	double GetPackedScale(const double scale) {
		char buffer[32] = {};
		sprintf_s(buffer, "%.2f", scale);
		return atof(buffer);
	}
	
	//
	// Extended window messages.
	//

	std::chrono::high_resolution_clock::time_point initializationTimer;

	void setRedrawOnExpensiveWindows(HWND hWnd, bool redraw) {
		if constexpr (!ENABLE_ALL_OPTIMIZATIONS) {
			return;
		}

		const auto wParam = redraw ? TRUE : FALSE;
		SendDlgItemMessageA(hWnd, CONTROL_ID_KEY_COMBO, WM_SETREDRAW, wParam, NULL);
		SendDlgItemMessageA(hWnd, CONTROL_ID_LOAD_CELL_COMBO, WM_SETREDRAW, wParam, NULL);
		SendDlgItemMessageA(hWnd, CONTROL_ID_OWNER_COMBO, WM_SETREDRAW, wParam, NULL);
		SendDlgItemMessageA(hWnd, CONTROL_ID_OWNER_VARIABLE_RANK_COMBO, WM_SETREDRAW, wParam, NULL);
		SendDlgItemMessageA(hWnd, CONTROL_ID_SOUL_COMBO, WM_SETREDRAW, wParam, NULL);
		SendDlgItemMessageA(hWnd, CONTROL_ID_TRAP_COMBO, WM_SETREDRAW, wParam, NULL);
	}

	void PatchDialogProc_BeforeInitialize(DialogProcContext& context) {
		if constexpr (LOG_PERFORMANCE_RESULTS) {
			initializationTimer = std::chrono::high_resolution_clock::now();
		}

		// Optimize redraws.
		setRedrawOnExpensiveWindows(context.getWindowHandle(), false);
	}

	void PatchDialogProc_AfterInitialize(DialogProcContext& context) {
		// Restore redraws.
		setRedrawOnExpensiveWindows(context.getWindowHandle(), true);
		
		if constexpr (LOG_PERFORMANCE_RESULTS) {
			auto timeToInitialize = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - initializationTimer);
			log::stream << "Displaying reference data took " << timeToInitialize.count() << "ms" << std::endl;
		}
	}

	void PatchDialogProc_BeforeCommand_OwnerCombo_SelectionChanged(DialogProcContext& context) {
		using winui::ComboBox_SelectStringExact;

		const auto hWnd = context.getWindowHandle();
		const auto hOwnerComboxBox = GetDlgItem(hWnd, CONTROL_ID_OWNER_COMBO);
		const auto hOwnerVariableComboBox = GetDlgItem(hWnd, CONTROL_ID_OWNER_VARIABLE_RANK_COMBO);
		const auto selection = ComboBox_GetCurSel(hOwnerComboxBox);
		if (selection == CB_ERR) {
			return;
		}

		const auto selectedOwner =  (BaseObject*)ComboBox_GetItemData(hOwnerComboxBox, selection);
		if (selectedOwner == nullptr) {
			return;
		}

		// Make sure the selection was actually changed.
		const auto userData = context.getUserData<UserData>();
		if (!userData->itemData || userData->itemData->owner != selectedOwner) {
			return;
		}

		// Prevent vanilla logic.
		context.setResult(0);

		// We still need to populate the related combobox.
		ComboBox_ResetContent(hOwnerVariableComboBox);
		if (selectedOwner == nullptr) {
			EnableWindow(hOwnerVariableComboBox, FALSE);
		}
		else if (selectedOwner->objectType == ObjectType::NPC) {
			const auto noVariableIndex = ComboBox_AddString(hOwnerVariableComboBox, "");
			ComboBox_SetItemData(hOwnerVariableComboBox, noVariableIndex, nullptr);
			for (auto global : *DataHandler::get()->recordHandler->globals) {
				const auto index = ComboBox_AddString(hOwnerVariableComboBox, global->getObjectID());
				ComboBox_SetItemData(hOwnerVariableComboBox, index, global);
			}
			EnableWindow(hOwnerVariableComboBox, TRUE);
			if (userData->itemData->requiredVariable) {
				ComboBox_SelectStringExact(hOwnerVariableComboBox, userData->itemData->requiredVariable->getObjectID());
			}
			else {
				ComboBox_SetCurSel(hOwnerVariableComboBox, 0);
			}
			SendMessageA(hWnd, WM_COMMAND, 0x103FEu, (LPARAM)hOwnerVariableComboBox);
		}
		else if (selectedOwner->objectType == ObjectType::Faction) {
			const auto ownerAsFaction = static_cast<Faction*>(selectedOwner);
			const auto noVariableIndex = ComboBox_AddString(hOwnerVariableComboBox, "");
			ComboBox_SetItemData(hOwnerVariableComboBox, noVariableIndex, -1);
			EnableWindow(hOwnerVariableComboBox, TRUE);
			for (auto i = 0; i < 10; ++i) {
				const auto rankName = ownerAsFaction->rankNames[i];
				if (strnlen_s(rankName, 32) == 0) {
					break;
				}
				const auto index = ComboBox_AddString(hOwnerVariableComboBox, rankName);
				ComboBox_SetItemData(hOwnerVariableComboBox, index, i);
			}
			if (userData->itemData->requiredRank >= 0) {
				ComboBox_SelectStringExact(hOwnerVariableComboBox, ownerAsFaction->rankNames[userData->itemData->requiredRank]);
			}
			else {
				ComboBox_SetCurSel(hOwnerVariableComboBox, 0);
			}
			SendMessageA(hWnd, WM_COMMAND, 0x103FEu, (LPARAM)hOwnerVariableComboBox);
		}
	}

	void PatchDialogProc_BeforeCommand_OwnerVarCombo_SelectionChanged(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();
		const auto hOwnerComboxBox = GetDlgItem(hWnd, CONTROL_ID_OWNER_COMBO);
		const auto hOwnerVariableComboBox = GetDlgItem(hWnd, CONTROL_ID_OWNER_VARIABLE_RANK_COMBO);
		const auto userData = context.getUserData<UserData>();

		char buffer[32] = {};
		const auto selection = ComboBox_GetCurSel(hOwnerVariableComboBox);
		const auto selectionText = ComboBox_GetText(hOwnerVariableComboBox, buffer, sizeof(buffer));
		if (selection == CB_ERR) {
			return;
		}

		const auto selectedOwnerVar = ComboBox_GetItemData(hOwnerVariableComboBox, selection);
		if (userData->itemData == nullptr || userData->itemData->requiredRank == selectedOwnerVar) {
			return;
		}

		userData->lParam->reference->setModified(true);
	}

	void PatchDialogProc_BeforeCommand_LockLevelEdit_Change(DialogProcContext& context) {
		// Prevent vanilla logic.
		context.setResult(0);

		const auto hWnd = context.getWindowHandle();
		const auto userData = context.getUserData<UserData>();
		if (userData->unknown_0x2F) {
			return;
		}

		const auto newLockLevel = GetClampedDialogItemInteger(hWnd, CONTROL_ID_LOCK_LEVEL_EDIT, 0, 100);
		if (userData->securityAttachment->lockLevel == newLockLevel) {
			return;
		}

		userData->securityAttachment->lockLevel = newLockLevel;
		userData->lParam->reference->setModified(true);
	}

	void PatchDialogProc_BeforeCommand_KeyCombo_SelectionChanged(DialogProcContext& context) {
		// Prevent vanilla logic.
		context.setResult(0);

		const auto hWnd = context.getWindowHandle();
		const auto hKeyComboBox = GetDlgItem(hWnd, CONTROL_ID_KEY_COMBO);
		const auto userData = context.getUserData<UserData>();

		const auto selectedIndex = ComboBox_GetCurSel(hKeyComboBox);
		if (selectedIndex == CB_ERR) {
			return;
		}

		auto newKey = (Object*)ComboBox_GetItemData(hKeyComboBox, selectedIndex);
		if (newKey == (Object*)CB_ERR) {
			newKey = nullptr;
		}

		if (userData->securityAttachment->key == newKey) {
			return;
		}

		userData->securityAttachment->key = newKey;
		userData->lParam->reference->setModified(true);
	}

	void PatchDialogProc_BeforeCommand_TrapCombo_SelectionChanged(DialogProcContext& context) {
		// Prevent vanilla logic.
		context.setResult(0);

		const auto hWnd = context.getWindowHandle();
		const auto hTrapComboBox = GetDlgItem(hWnd, CONTROL_ID_TRAP_COMBO);
		const auto userData = context.getUserData<UserData>();

		const auto selectedIndex = ComboBox_GetCurSel(hTrapComboBox);
		if (selectedIndex == CB_ERR) {
			return;
		}

		auto newTrap = (Spell*)ComboBox_GetItemData(hTrapComboBox, selectedIndex);
		if (newTrap == (Spell*)CB_ERR) {
			newTrap = nullptr;
		}

		if (userData->securityAttachment->trap == newTrap) {
			return;
		}

		userData->securityAttachment->trap = newTrap;
		userData->lParam->reference->setModified(true);
	}

	void PatchDialogProc_BeforeCommand_ScaleEdit_Change(DialogProcContext& context) {
		// Prevent vanilla logic.
		context.setResult(0);

		const auto hWnd = context.getWindowHandle();
		const auto userData = context.getUserData<UserData>();
		if (userData->unknown_0x2F) {
			return;
		}

		char buffer[32] = {};
		GetDlgItemTextA(hWnd, CONTROL_ID_SCALE_EDIT, buffer, sizeof(buffer));
		const auto newScale = GetPackedScale(atof(buffer));

		const auto reference = userData->lParam->reference;
		const auto scaleBefore = reference->getScale();
		reference->setScale((float)newScale);
		if (scaleBefore == reference->getScale()) {
			return;
		}

		userData->lParam->reference->setModified(true);
	}

	void PatchDialogProc_BeforeCommand_LoadCell_SelectionChanged(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();
		const auto hLoadCellComboBox = GetDlgItem(hWnd, CONTROL_ID_LOAD_CELL_COMBO);
		const auto userData = context.getUserData<UserData>();

		const auto selectedIndex = ComboBox_GetCurSel(hLoadCellComboBox);
		if (selectedIndex == CB_ERR) {
			return;
		}

		auto cell = (Cell*)ComboBox_GetItemData(hLoadCellComboBox, selectedIndex);
		if (cell == (Cell*)CB_ERR) {
			cell = nullptr;
		}

		if (cell == userData->loadDoorAttachment->cell) {
			context.setResult(0);
			return;
		}
	}

	void PatchDialogProc_BeforeCommand_SoulCombo_SelectionChanged(DialogProcContext& context) {
		context.setResult(0);

		const auto hWnd = context.getWindowHandle();
		const auto hSoulComboBox = GetDlgItem(hWnd, CONTROL_ID_SOUL_COMBO);
		const auto userData = context.getUserData<UserData>();

		const auto selectedIndex = ComboBox_GetCurSel(hSoulComboBox);
		if (selectedIndex == CB_ERR) {
			return;
		}

		auto soul = (Actor*)ComboBox_GetItemData(hSoulComboBox, selectedIndex);
		if (soul == (Actor*)CB_ERR) {
			soul = nullptr;
		}

		if (userData->itemData->soul == soul) {
			return;
		}

		userData->itemData->soul = soul;
		userData->lParam->reference->setModified(true);
	}

	void PatchDialogProc_BeforeCommand_GenericSetModified(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();
		const auto userData = context.getUserData<UserData>();
		userData->lParam->reference->setModified(true);
	}

	void PatchDialogProc_BeforeCommand_HealthLeftEdit_Change(DialogProcContext& context) {
		// Prevent vanilla logic.
		context.setResult(0);

		const auto hWnd = context.getWindowHandle();
		const auto userData = context.getUserData<UserData>();
		if (userData->unknown_0x2F) {
			return;
		}

		const auto newValue = GetClampedDialogItemInteger(hWnd, CONTROL_ID_HEALTH_LEFT_EDIT, 0, std::numeric_limits<int>::max());
		const auto reference = userData->lParam->reference;
		const auto object = userData->lParam->object;

		auto modifiesCount = false;
		switch (object->objectType) {
		case ObjectType::Misc:
			modifiesCount = true;
			break;
		case ObjectType::Weapon:
			modifiesCount = static_cast<Weapon*>(object)->isProjectile();
			break;
		}

		if (modifiesCount) {
			if (userData->itemData->count == newValue) {
				return;
			}

			userData->itemData->count = newValue;
			reference->setModified(true);
		}
		else {
			if (userData->itemData->condition == newValue) {
				return;
			}

			userData->itemData->condition = newValue;
			reference->setModified(true);
		}

	}

	static NI::Vector3 lastUsedPosition = {};
	static NI::Vector3 lastUsedRotation = {};

	void PatchDialogProc_BeforeCommand_GenericStoreLastPosition(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();
		const auto userData = context.getUserData<UserData>();
		lastUsedPosition = userData->lParam->reference->referenceData.unknown_0x10;
	}

	void PatchDialogProc_BeforeCommand_GenericStoreLastRotation(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();
		const auto userData = context.getUserData<UserData>();
		lastUsedRotation = userData->lParam->reference->referenceData.orientationNonAttached;
	}

	void PatchDialogProc_BeforeCommand(DialogProcContext& context) {
		const auto commandNotificationCode = context.getCommandNotificationCode();

		switch (context.getCommandControlIdentifier()) {
		case CONTROL_ID_LOCK_LEVEL_EDIT:
			switch (commandNotificationCode) {
			case EN_CHANGE:
				PatchDialogProc_BeforeCommand_LockLevelEdit_Change(context);
				break;
			}
			break;
		case CONTROL_ID_KEY_COMBO:
			switch (commandNotificationCode) {
			case CBN_SELCHANGE:
				// Prevent the CS from dirtying an object if the selection didn't actually change.
				PatchDialogProc_BeforeCommand_KeyCombo_SelectionChanged(context);
				break;
			}
			break;
		case CONTROL_ID_TRAP_COMBO:
			switch (commandNotificationCode) {
			case CBN_SELCHANGE:
				// Prevent the CS from dirtying an object if the selection didn't actually change.
				PatchDialogProc_BeforeCommand_TrapCombo_SelectionChanged(context);
				break;
			}
			break;
		case CONTROL_ID_SOUL_COMBO:
			switch (commandNotificationCode) {
			case CBN_SELCHANGE:
				// Prevent the CS from dirtying an object if the selection didn't actually change.
				PatchDialogProc_BeforeCommand_SoulCombo_SelectionChanged(context);
				break;
			}
			break;
		case CONTROL_ID_SCALE_EDIT:
			switch (commandNotificationCode) {
			case EN_CHANGE:
				PatchDialogProc_BeforeCommand_ScaleEdit_Change(context);
			}
			break;
		case CONTROL_ID_OWNER_COMBO:
			switch (commandNotificationCode) {
			case CBN_SELCHANGE:
				// Prevent the CS from dirtying an object if the selection didn't actually change.
				PatchDialogProc_BeforeCommand_OwnerCombo_SelectionChanged(context);
				break;
			}
			break;
		case CONTROL_ID_LOAD_CELL_COMBO:
			switch (commandNotificationCode) {
			case CBN_SELCHANGE:
				// Prevent the CS from dirtying an object if the selection didn't actually change.
				PatchDialogProc_BeforeCommand_LoadCell_SelectionChanged(context);
				break;
			}
			break;
		case CONTROL_ID_OWNER_VARIABLE_RANK_COMBO:
			switch (commandNotificationCode) {
			case CBN_SELCHANGE:
				// DO dirty a reference if the ownership variable actually changes.
				PatchDialogProc_BeforeCommand_OwnerVarCombo_SelectionChanged(context);
				break;
			}
			break;
		case CONTROL_ID_EXTRA_DATA_BUTTON:
			switch (commandNotificationCode) {
			case BN_CLICKED:
				PatchDialogProc_BeforeCommand_GenericSetModified(context);
				break;
			}
			break;
		case CONTROL_ID_REFERENCE_BLOCKED_BUTTON:
			switch (commandNotificationCode) {
			case BN_CLICKED:
				PatchDialogProc_BeforeCommand_GenericSetModified(context);
				break;
			}
			break;
		case CONTROL_ID_HEALTH_LEFT_EDIT:
			switch (commandNotificationCode) {
			case EN_CHANGE:
				PatchDialogProc_BeforeCommand_HealthLeftEdit_Change(context);
				break;
			}
			break;
		case CONTROL_ID_POSITION_X_EDIT:
		case CONTROL_ID_POSITION_Y_EDIT:
		case CONTROL_ID_POSITION_Z_EDIT:
			switch (commandNotificationCode) {
			case EN_KILLFOCUS:
				PatchDialogProc_BeforeCommand_GenericStoreLastPosition(context);
				break;
			}
			break;
		case CONTROL_ID_ROTATION_X_EDIT:
		case CONTROL_ID_ROTATION_Y_EDIT:
		case CONTROL_ID_ROTATION_Z_EDIT:
			switch (commandNotificationCode) {
			case EN_KILLFOCUS:
				PatchDialogProc_BeforeCommand_GenericStoreLastRotation(context);
				break;
			}
			break;
		}
	}

	void PatchDialogProc_AfterCommand_GenericCompareLastPosition(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();
		const auto userData = context.getUserData<UserData>();
		const auto reference = userData->lParam->reference;
		if (reference->referenceData.unknown_0x10 != lastUsedPosition) {
			reference->setModified(true);
		}
	}

	void PatchDialogProc_AfterCommand_GenericCompareLastRotation(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();
		const auto userData = context.getUserData<UserData>();
		const auto reference = userData->lParam->reference;
		if (reference->referenceData.orientationNonAttached != lastUsedRotation) {
			reference->setModified(true);
		}
	}

	void PatchDialogProc_AfterCommand(DialogProcContext& context) {
		const auto commandNotificationCode = context.getCommandNotificationCode();
		switch (context.getCommandControlIdentifier()) {
			case CONTROL_ID_POSITION_X_EDIT:
			case CONTROL_ID_POSITION_Y_EDIT:
			case CONTROL_ID_POSITION_Z_EDIT:
				switch (commandNotificationCode) {
				case EN_KILLFOCUS:
					PatchDialogProc_AfterCommand_GenericCompareLastPosition(context);
					break;
				}
				break;
			case CONTROL_ID_ROTATION_X_EDIT:
			case CONTROL_ID_ROTATION_Y_EDIT:
			case CONTROL_ID_ROTATION_Z_EDIT:
				switch (commandNotificationCode) {
				case EN_KILLFOCUS:
					PatchDialogProc_AfterCommand_GenericCompareLastRotation(context);
					break;
				}
				break;
		}
	}

	LRESULT CALLBACK PatchDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		DialogProcContext context(hWnd, msg, wParam, lParam, 0x41EF80);

		switch (msg) {
		case WM_INITDIALOG:
			PatchDialogProc_BeforeInitialize(context);
			break;
		case WM_COMMAND:
			PatchDialogProc_BeforeCommand(context);
			break;
		}

		// Call original function, or return early if we already have a result.
		if (context.hasResult()) {
			return context.getResult();
		}
		else {
			context.callOriginalFunction();
		}

		switch (msg) {
		case WM_INITDIALOG:
			PatchDialogProc_AfterInitialize(context);
			break;
		case WM_COMMAND:
			PatchDialogProc_AfterCommand(context);
			break;
		}

		return context.getResult();
	}

	//
	//
	//

	void installPatches() {
		using memory::genJumpEnforced;

		// Patch: Extend handling.
		genJumpEnforced(0x40366B, 0x41EF80, reinterpret_cast<DWORD>(PatchDialogProc));
	}
}
