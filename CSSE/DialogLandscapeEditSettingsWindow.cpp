#include "DialogLandscapeEditSettingsWindow.h"

#include "CSLandTexture.h"

#include "WinUIUtil.h"

#include "Settings.h"
#include "CSDataHandler.h"
#include "CSRecordHandler.h"
#include "CSRegion.h"
#include "CSLand.h"
#include "CSCell.h"

#include "DialogProcContext.h"

#include "DialogRenderWindow.h"

namespace se::cs::dialog::landscape_edit_settings_window {
	constexpr auto MIN_WIDTH = 416u + 17u;
	constexpr auto MIN_HEIGHT = 474u + 14u;

	namespace LandscapeEditFlag {
		enum LandscapeEditFlag : unsigned int {
			ShowEditRadius = 0x1,
			FlattenVertices = 0x2,
			EditColors = 0x4,
			SoftenVertices = 0x8,
		};
	}

	using gLandscapeEditFlags = memory::ExternalGlobal<unsigned int, 0x6CE9C8>;

	bool getLandscapeEditFlag(LandscapeEditFlag::LandscapeEditFlag flag) {
		return gLandscapeEditFlags::get() & flag;
	}

	void setLandscapeEditFlag(LandscapeEditFlag::LandscapeEditFlag flag, bool set) {
		auto& flags = gLandscapeEditFlags::get();
		if (set) {
			flags |= flag;
		}
		else {
			flags &= ~flag;
		}
	}

	bool getEditLandscapeColor() {
		return getLandscapeEditFlag(LandscapeEditFlag::EditColors);
	}

	void setEditLandscapeColor(bool set) {
		auto hWnd = gWindowHandle::get();

		setLandscapeEditFlag(LandscapeEditFlag::EditColors, set);
		CheckDlgButton(hWnd, CONTROL_ID_EDIT_COLORS_CHECKBOX, set ? BST_CHECKED : BST_UNCHECKED);

		if (set) {
			setFlattenLandscapeVertices(false);
			EnableWindow(GetDlgItem(hWnd, CONTROL_ID_FLATTEN_VERTICES_CHECKBOX), FALSE);
			setSoftenLandscapeVertices(false);
			EnableWindow(GetDlgItem(hWnd, CONTROL_ID_SOFTEN_VERTICES_CHECKBOX), FALSE);
		}
		else {
			EnableWindow(GetDlgItem(hWnd, CONTROL_ID_FLATTEN_VERTICES_CHECKBOX), TRUE);
			EnableWindow(GetDlgItem(hWnd, CONTROL_ID_SOFTEN_VERTICES_CHECKBOX), TRUE);
		}

		render_window::updateLandscapeCircleWidget();
	}

	bool getFlattenLandscapeVertices() {
		return getLandscapeEditFlag(LandscapeEditFlag::FlattenVertices);
	}

	void setFlattenLandscapeVertices(bool set) {
		auto hWnd = gWindowHandle::get();

		setLandscapeEditFlag(LandscapeEditFlag::FlattenVertices, set);
		CheckDlgButton(hWnd, CONTROL_ID_FLATTEN_VERTICES_CHECKBOX, set ? BST_CHECKED : BST_UNCHECKED);

		if (set) {
			setSoftenLandscapeVertices(false);
			setEditLandscapeColor(false);
		}

		render_window::updateLandscapeCircleWidget();
	}

	bool getSoftenLandscapeVertices() {
		return getLandscapeEditFlag(LandscapeEditFlag::SoftenVertices);
	}

	void setSoftenLandscapeVertices(bool set) {
		auto hWnd = gWindowHandle::get();

		setLandscapeEditFlag(LandscapeEditFlag::SoftenVertices, set);
		CheckDlgButton(hWnd, CONTROL_ID_SOFTEN_VERTICES_CHECKBOX, set ? BST_CHECKED : BST_UNCHECKED);

		if (set) {
			setFlattenLandscapeVertices(false);
			setEditLandscapeColor(false);
		}

		render_window::updateLandscapeCircleWidget();
	}

	LandTexture* getSelectedTexture() {
		auto hWnd = gWindowHandle::get();
		if (hWnd == NULL) {
			return nullptr;
		}

		auto textureList = GetDlgItem(hWnd, CONTROL_ID_TEXTURE_LIST);
		auto selected = ListView_GetNextItem(textureList, -1, LVNI_SELECTED);
		if (selected == -1) {
			return nullptr;
		}

		LVITEMA queryData = {};
		queryData.mask = LVIF_PARAM;
		queryData.iItem = selected;
		if (!ListView_GetItem(textureList, &queryData)) {
			return nullptr;
		}

		return reinterpret_cast<LandTexture*>(queryData.lParam);
	}

	bool setSelectTexture(LandTexture* texture) {
		if (texture == nullptr) {
			return false;
		}

		auto hWnd = gWindowHandle::get();
		if (hWnd == NULL) {
			return false;
		}

		LVITEMA queryData = {};
		queryData.mask = LVIF_PARAM;
		auto textureList = GetDlgItem(hWnd, CONTROL_ID_TEXTURE_LIST);
		auto textureCount = ListView_GetItemCount(textureList);
		for (auto row = 0; row < textureCount; ++row) {
			queryData.iItem = row;
			if (!ListView_GetItem(textureList, &queryData)) {
				continue;
			}

			auto landTexture = reinterpret_cast<LandTexture*>(queryData.lParam);
			if (!landTexture) {
				continue;
			}

			if (landTexture == texture) {
				ListView_SetItemState(textureList, row, LVIS_SELECTED, LVIS_SELECTED);
				ListView_EnsureVisible(textureList, row, TRUE);
				return true;
			}
		}

		return false;
	}

	bool incrementEditRadius() {
		auto hWnd = gWindowHandle::get();
		if (hWnd == NULL) {
			return false;
		}

		auto radius = winui::GetDlgItemSignedInt(hWnd, CONTROL_ID_EDIT_RADIUS_EDIT).value_or(1);
		radius = std::min(radius + 1, 30);
		SetDlgItemInt(hWnd, CONTROL_ID_EDIT_RADIUS_EDIT, radius, FALSE);

		return true;
	}

	bool decrementEditRadius() {
		auto hWnd = gWindowHandle::get();
		if (hWnd == NULL) {
			return false;
		}

		auto radius = winui::GetDlgItemSignedInt(hWnd, CONTROL_ID_EDIT_RADIUS_EDIT).value_or(1);
		radius = std::max(radius - 1, 1);
		SetDlgItemInt(hWnd, CONTROL_ID_EDIT_RADIUS_EDIT, radius, FALSE);

		return true;
	}

	bool getLandscapeEditingEnabled() {
		return gLandscapeEditingEnabled::get();
	}

	void setLandscapeEditingEnabled(bool enabled, bool ifWindowOpen) {
		if (ifWindowOpen) {
			enabled = gWindowHandle::get() != NULL;
		}
		gLandscapeEditingEnabled::set(enabled);
	}

	void restoreTextureListColomnWidths(HWND hWnd) {
		const auto textureList = GetDlgItem(hWnd, CONTROL_ID_TEXTURE_LIST);

		ListView_SetColumnWidth(textureList, 0, settings.landscape_window.column_id.width);
		ListView_SetColumnWidth(textureList, 1, settings.landscape_window.column_used.width);
		ListView_SetColumnWidth(textureList, 2, settings.landscape_window.column_filename.width);

	}

	void saveTextureListColomnWidths(HWND hWnd) {
		const auto textureList = GetDlgItem(hWnd, CONTROL_ID_TEXTURE_LIST);

		settings.landscape_window.column_id.width = ListView_GetColumnWidth(textureList, 0);
		settings.landscape_window.column_used.width = ListView_GetColumnWidth(textureList, 1);
		settings.landscape_window.column_filename.width = ListView_GetColumnWidth(textureList, 2);
	}

	void togglePreviewTextureShown(HWND hWnd, int id) {
		settings.landscape_window.show_preview_enabled = !settings.landscape_window.show_preview_enabled;

		auto hDlgSelectedTextureStatic = GetDlgItem(hWnd, CONTROL_ID_SELECTED_TEXTURE_STATIC);
		auto hDlgPreviewTextureFrameStatic = GetDlgItem(hWnd, CONTROL_ID_PREVIEW_TEXTURE_FRAME_STATIC);
		auto hDlgPreviewTextureImage = GetDlgItem(hWnd, CONTROL_ID_PREVIEW_TEXTURE_BUTTON);

		int width = settings.landscape_window.size.width;
		bool isVisible;

		if (settings.landscape_window.show_preview_enabled) {
			width += 270;
			isVisible = TRUE;
		}
		else {
			width -= 270;
			isVisible = FALSE;
		}
		
		// hide/unhide the preview texture controls
		ShowWindow(hDlgSelectedTextureStatic, isVisible);
		ShowWindow(hDlgPreviewTextureFrameStatic, isVisible);
		ShowWindow(hDlgPreviewTextureImage, isVisible);

		SetWindowPos(hWnd, NULL, 0, 0, width, settings.landscape_window.size.height, SWP_NOMOVE);		

		RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_ERASENOW | RDW_INVALIDATE | RDW_ALLCHILDREN);	
	}

	//
	// Patch: Extend Render Window message handling.
	//

	void PatchDialogProc_BeforeDestroy(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();
		saveTextureListColomnWidths(hWnd);
		settings.landscape_window.size.width -= IsDlgButtonChecked(hWnd, CONTROL_ID_SHOW_PREVIEW_TEXTURE_BUTTON) ? 270 : 0;
	}

	void PatchDialogProc_AfterDestroy(DialogProcContext& context) {
		// Cleanup global handle address so we can rely on it being NULL.
		gWindowHandle::set(0x0);
	}

	void PatchDialogProc_BeforeCommand(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();
		const auto code = context.getCommandNotificationCode();
		const auto id = context.getCommandControlIdentifier();
		switch (id) {
		case CONTROL_ID_SHOW_PREVIEW_TEXTURE_BUTTON:
			switch (code) {
			case BN_CLICKED:
				togglePreviewTextureShown(hWnd, id);
				break;
			}
			break;
		}
	}

	void PatchDialogProc_AfterCommand(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();
		const auto code = context.getCommandNotificationCode();
		const auto id = context.getCommandControlIdentifier();
		switch (id) {
		case CONTROL_ID_FLATTEN_VERTICES_CHECKBOX:
		case CONTROL_ID_SOFTEN_VERTICES_CHECKBOX:
		case CONTROL_ID_EDIT_COLORS_CHECKBOX:
			switch (code) {
			case BN_CLICKED:
				render_window::updateLandscapeCircleWidget();
				break;
			}
			break;
		}
	}

	void PatchDialogProc_AfterInitialize(DialogProcContext& context) {
		using se::cs::winui::AddStyles;
		using se::cs::winui::RemoveStyles;
		using se::cs::winui::SetWindowIdByValue;

		const auto hWnd = context.getWindowHandle();
		restoreTextureListColomnWidths(hWnd);

		// Give IDs to controls that don't normally have one.
		SetWindowIdByValue(hWnd, "Height", CONTROL_ID_HEIGHT_GROUPBOX);
		SetWindowIdByValue(hWnd, "Edit Radius:", CONTROL_ID_EDIT_RADIUS_STATIC);
		SetWindowIdByValue(hWnd, "Edit Falloff %:", CONTROL_ID_EDIT_FALLOFF_STATIC);
		SetWindowIdByValue(hWnd, "Texture", CONTROL_ID_TEXTURE_GROUPBOX);
		SetWindowIdByValue(hWnd, "Selected\nTexture:", CONTROL_ID_SELECTED_TEXTURE_STATIC);
		SetWindowIdByValue(hWnd, "Vertex Color", CONTROL_ID_VERTEX_COLOR_GROUPBOX);
		SetWindowIdByValue(hWnd, "R", CONTROL_ID_PRIMARY_COLOR_RED_STATIC);
		SetWindowIdByValue(hWnd, "G", CONTROL_ID_PRIMARY_COLOR_GREEN_STATIC);
		SetWindowIdByValue(hWnd, "B", CONTROL_ID_PRIMARY_COLOR_BLUE_STATIC);
		SetWindowIdByValue(hWnd, "R", CONTROL_ID_SECONDARY_COLOR_RED_STATIC);
		SetWindowIdByValue(hWnd, "G", CONTROL_ID_SECONDARY_COLOR_GREEN_STATIC);
		SetWindowIdByValue(hWnd, "B", CONTROL_ID_SECONDARY_COLOR_BLUE_STATIC);

		RemoveStyles(hWnd, DS_MODALFRAME | WS_SYSMENU);
		AddStyles(hWnd, WS_POPUP | WS_CAPTION | WS_SIZEBOX);
		
		auto hInstance = (HINSTANCE)GetWindowLongA(hWnd, GWLP_HINSTANCE);
		auto font = SendMessageA(hWnd, WM_GETFONT, FALSE, FALSE);

		auto hDlgShowPreviewTextureButton = CreateWindowExA(NULL, WC_BUTTON, "Show Preview", BS_AUTOCHECKBOX | BS_PUSHLIKE | WS_CHILD | WS_VISIBLE | WS_GROUP, 0, 0, 0, 0, hWnd, (HMENU)CONTROL_ID_SHOW_PREVIEW_TEXTURE_BUTTON, hInstance, NULL);
		SendMessageA(hDlgShowPreviewTextureButton, WM_SETFONT, font, MAKELPARAM(TRUE, FALSE));

		// Hide the preview texture controls
		if (!settings.landscape_window.show_preview_enabled) {
			auto hDlgSelectedTextureStatic = GetDlgItem(hWnd, CONTROL_ID_SELECTED_TEXTURE_STATIC);
			ShowWindow(hDlgSelectedTextureStatic, FALSE);

			auto hDlgPreviewTextureFrameStatic = GetDlgItem(hWnd, CONTROL_ID_PREVIEW_TEXTURE_FRAME_STATIC);
			ShowWindow(hDlgPreviewTextureFrameStatic, FALSE);

			auto hDlgPreviewTextureImage = GetDlgItem(hWnd, CONTROL_ID_PREVIEW_TEXTURE_BUTTON);
			ShowWindow(hDlgPreviewTextureImage, FALSE);
		}
		else {
			Button_SetCheck(GetDlgItem(hWnd, CONTROL_ID_SHOW_PREVIEW_TEXTURE_BUTTON), settings.landscape_window.show_preview_enabled);
		}

		//Change the selected texture static to be a single line
		SetDlgItemText(hWnd, CONTROL_ID_SELECTED_TEXTURE_STATIC, "Selected Texture:");
		
		// Restore size and position
		const auto& settingsSize = settings.landscape_window.size;
		const auto width = std::max(settingsSize.width, MIN_WIDTH);
		const auto height = std::max(settingsSize.height, MIN_HEIGHT);
		const auto x = settings.landscape_window.x_position;
		const auto y = settings.landscape_window.y_position;

		MoveWindow(hWnd, x, y, width, height, FALSE);
		RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
	}

	void PatchDialogProc_GetMinMaxInfo(DialogProcContext& context) {
		const auto showPreviewImagePadding = IsDlgButtonChecked(context.getWindowHandle(), CONTROL_ID_SHOW_PREVIEW_TEXTURE_BUTTON) ? 270 : 0;
		const auto info = context.getMinMaxInfo();
		info->ptMinTrackSize.x = MIN_WIDTH + showPreviewImagePadding;
		info->ptMinTrackSize.y = MIN_HEIGHT;

		context.setResult(0);
	}

	void PatchDialogProc_AfterMove(DialogProcContext& context) {
		settings.landscape_window.x_position = context.getMovedX() - 8;
		settings.landscape_window.y_position = context.getMovedY() - 31;
	}

	namespace ResizeConstants {
		constexpr auto BASIC_PADDING = 2;
		constexpr auto BIG_PADDING = 6;
		constexpr auto WINDOW_EDGE_PADDING = 10;

		constexpr auto SECTIONS_MIN_WIDTH = 396;
		constexpr auto HEIGHT_SECTION_HEIGHT = 80;
		constexpr auto VERTEX_COLOR_SECTION_HEIGHT = 147;

		constexpr auto BUTTON_HEIGHT = 23;
		constexpr auto BUTTON_WIDTH = 80;

		constexpr auto HEIGHT_STATIC_WIDTH = 64;
		constexpr auto HEIGHT_STATIC_HEIGHT = 16;

		constexpr auto HEIGHT_EDIT_WIDTH = 68;
		constexpr auto HEIGHT_EDIT_HEIGHT = 20;

		constexpr auto HEIGHT_CHECKBOX_WIDTH = 93;
		constexpr auto HEIGHT_CHECKBOX_HEIGHT = 13;

		// Extra padding needed to keep the controls within the window
		constexpr auto BOTTOM_GHOST_HEIGHT = 99;

		constexpr auto PREVIEW_TEXTURE_WIDTH = 110;
		constexpr auto PREVIEW_TEXTURE_NAME_WIDTH = 186;
		constexpr auto PREVIEW_TEXTURE_NAME_HEIGHT = 14;

		constexpr auto STATIC_WIDTH = 12;
		constexpr auto STATIC_HEIGHT = 16;
		constexpr auto STATIC_EDIT_OFFSET = 4;

		constexpr auto VERTEX_EDIT_WIDTH = 26;
		constexpr auto VERTEX_EDIT_HEIGHT = 20;

		constexpr auto SPIN_WIDTH = 18;
		constexpr auto SPIN_HEIGHT = 20;

		constexpr auto STATIC_EDIT_SPIN_OFFSET = 24;

		constexpr auto COLOR_PREVIEW_WIDTH = 58;
		constexpr auto COLOR_PREVIEW_HEIGHT = 42;

		constexpr auto CUSTOM_COLOR_WIDTH = 20;
		constexpr auto CUSTOM_COLOR_HEIGHT = 15;
		constexpr auto CUSTOM_COLOR_PADDING = 22;
	}

	void ResizeCustomColorStatic(HWND hParent, int iDlgStatic, int& x, int y) {
		using namespace ResizeConstants;

		auto hDlgStatic = GetDlgItem(hParent, iDlgStatic);
		MoveWindow(hDlgStatic, x, y, CUSTOM_COLOR_WIDTH, CUSTOM_COLOR_HEIGHT, FALSE);
		x += CUSTOM_COLOR_WIDTH + CUSTOM_COLOR_PADDING;
	}

	void ResizeStaticEditSpin(HWND hParent, int iDlgStatic, int iDlgEdit, int iDlgSpin, int x, int& y) {
		using namespace ResizeConstants;

		auto hDlgStatic = GetDlgItem(hParent, iDlgStatic);
		MoveWindow(hDlgStatic, x, y + STATIC_EDIT_OFFSET, STATIC_WIDTH, STATIC_HEIGHT, FALSE);

		auto hDlgEdit = GetDlgItem(hParent, iDlgEdit);
		MoveWindow(hDlgEdit, x + STATIC_WIDTH + 4, y, VERTEX_EDIT_WIDTH, VERTEX_EDIT_HEIGHT, FALSE);

		auto hDlgSpin = GetDlgItem(hParent, iDlgSpin);
		MoveWindow(hDlgSpin, x + STATIC_WIDTH + VERTEX_EDIT_WIDTH + 2, y, SPIN_WIDTH, SPIN_HEIGHT, FALSE);

		y += STATIC_EDIT_SPIN_OFFSET;
	}

#define ResizeStaticEditSpinHelper(hParent, primaryOrSecondary, color, x, y) ResizeStaticEditSpin(hParent, CONTROL_ID_##primaryOrSecondary##_COLOR_##color##_STATIC, CONTROL_ID_##primaryOrSecondary##_COLOR_##color##_EDIT, CONTROL_ID_##primaryOrSecondary##_COLOR_##color##_SPIN, x, y)

	void PatchDialogProc_BeforeSize(DialogProcContext& context) {
		using namespace ResizeConstants;

		const auto hWnd = context.getWindowHandle();
		const auto showPreviewImagePadding = IsDlgButtonChecked(hWnd, CONTROL_ID_SHOW_PREVIEW_TEXTURE_BUTTON) ? 270 : 0;
	
		const auto clientWidth = context.getSizeX();
		const auto clientHeight = context.getSizeY();
		const auto sectionWidth = clientWidth - WINDOW_EDGE_PADDING * 2 - showPreviewImagePadding;


		// Height Groupbox
		{
			auto currentX = WINDOW_EDGE_PADDING;
			auto currentY = WINDOW_EDGE_PADDING;

			auto heightGroupbox = GetDlgItem(hWnd, CONTROL_ID_HEIGHT_GROUPBOX);
			MoveWindow(heightGroupbox, currentX, currentY, sectionWidth, HEIGHT_SECTION_HEIGHT, FALSE);
			currentY += 18;
			currentX += 20;

			auto editRadiusStatic = GetDlgItem(hWnd, CONTROL_ID_EDIT_RADIUS_STATIC);
			MoveWindow(editRadiusStatic, currentX, currentY + 4, HEIGHT_STATIC_WIDTH, HEIGHT_STATIC_HEIGHT, FALSE);
			currentX += 69;

			auto editRadiusEdit = GetDlgItem(hWnd, CONTROL_ID_EDIT_RADIUS_EDIT);
			MoveWindow(editRadiusEdit, currentX, currentY, HEIGHT_EDIT_WIDTH, HEIGHT_EDIT_HEIGHT, FALSE);
			currentX += 88;

			auto flattenVerticesCheckbox = GetDlgItem(hWnd, CONTROL_ID_FLATTEN_VERTICES_CHECKBOX);
			MoveWindow(flattenVerticesCheckbox, currentX, currentY + 5, HEIGHT_CHECKBOX_WIDTH, HEIGHT_CHECKBOX_HEIGHT, FALSE);
			currentX += 104;

			auto softenVerticesCheckbox = GetDlgItem(hWnd, CONTROL_ID_SOFTEN_VERTICES_CHECKBOX);
			MoveWindow(softenVerticesCheckbox, currentX, currentY + 5, HEIGHT_CHECKBOX_WIDTH, HEIGHT_CHECKBOX_HEIGHT, FALSE);

			currentX = WINDOW_EDGE_PADDING + 20;
			currentY += 31;

			auto editFalloffStatic = GetDlgItem(hWnd, CONTROL_ID_EDIT_FALLOFF_STATIC);
			MoveWindow(editFalloffStatic, currentX, currentY + 3, HEIGHT_STATIC_WIDTH, HEIGHT_STATIC_HEIGHT, FALSE);
			currentX += 68;

			auto editFalloffEdit = GetDlgItem(hWnd, CONTROL_ID_EDIT_FALLOFF_EDIT);
			MoveWindow(editFalloffEdit, currentX, currentY, HEIGHT_EDIT_WIDTH, HEIGHT_EDIT_HEIGHT, FALSE);
			currentX += 133;

			auto showEditRadius = GetDlgItem(hWnd, CONTROL_ID_SHOW_EDIT_RADIUS_CHECKBOX);
			MoveWindow(showEditRadius, currentX, currentY + 4, HEIGHT_CHECKBOX_WIDTH + 10, HEIGHT_CHECKBOX_HEIGHT, FALSE);
		}

		// Texture Section
		{
			auto currentX = WINDOW_EDGE_PADDING;
			auto currentY = WINDOW_EDGE_PADDING + HEIGHT_SECTION_HEIGHT + BASIC_PADDING * 2;
			const auto TEXTURE_SECTION_HEIGHT = clientHeight - BOTTOM_GHOST_HEIGHT - VERTEX_COLOR_SECTION_HEIGHT - WINDOW_EDGE_PADDING - BUTTON_HEIGHT - BASIC_PADDING * 2;

			auto textureGroupbox = GetDlgItem(hWnd, CONTROL_ID_TEXTURE_GROUPBOX);
			MoveWindow(textureGroupbox, currentX, currentY, sectionWidth, TEXTURE_SECTION_HEIGHT, FALSE);

			currentX += BASIC_PADDING * 4;
			currentY += 16;

			auto TEXTURE_LIST_WIDTH = sectionWidth - WINDOW_EDGE_PADDING - BASIC_PADDING * 3;//clientWidth - WINDOW_EDGE_PADDING - PREVIEW_TEXTURE_WIDTH;
			const auto TEXTURE_LIST_HEIGHT = TEXTURE_SECTION_HEIGHT - BUTTON_HEIGHT - BASIC_PADDING * 10 - 1;

			auto textureList = GetDlgItem(hWnd, CONTROL_ID_TEXTURE_LIST);
			MoveWindow(textureList, currentX, currentY, TEXTURE_LIST_WIDTH, TEXTURE_LIST_HEIGHT, FALSE);
			currentX += TEXTURE_LIST_WIDTH;

			auto selectedTextureStatic = GetDlgItem(hWnd, CONTROL_ID_SELECTED_TEXTURE_STATIC);
			MoveWindow(selectedTextureStatic, currentX + 9, currentY - 54, 100, 12, FALSE);
			currentY += BASIC_PADDING + 25 + 4;

			auto previewTextureImage = GetDlgItem(hWnd, CONTROL_ID_PREVIEW_TEXTURE_BUTTON);
			MoveWindow(previewTextureImage, currentX + 20, currentY - 68, 256, 256, FALSE);

			auto previewTextureFrameStatic = GetDlgItem(hWnd, CONTROL_ID_PREVIEW_TEXTURE_FRAME_STATIC);
			MoveWindow(previewTextureFrameStatic, currentX + 18, currentY - 69, 260, 259, FALSE);

			currentX -= TEXTURE_LIST_WIDTH;
			currentY -= BASIC_PADDING + 25 + 4 + 1;
			currentY += TEXTURE_LIST_HEIGHT;

			auto addTextureButton = GetDlgItem(hWnd, CONTROL_ID_ADD_TEXTURE_BUTTON);
			MoveWindow(addTextureButton, currentX + 17, currentY + BASIC_PADDING, BUTTON_WIDTH, BUTTON_HEIGHT, FALSE);

			auto previewTextureNameStatic = GetDlgItem(hWnd, CONTROL_ID_PREVIEW_TEXTURE_NAME_STATIC);
			MoveWindow(previewTextureNameStatic, currentX + 110, currentY + 8, PREVIEW_TEXTURE_NAME_WIDTH, PREVIEW_TEXTURE_NAME_HEIGHT, FALSE);
				
			auto showPreviewTextureButton = GetDlgItem(hWnd, CONTROL_ID_SHOW_PREVIEW_TEXTURE_BUTTON);
			MoveWindow(showPreviewTextureButton, currentX + 301, currentY + BASIC_PADDING, BUTTON_WIDTH, BUTTON_HEIGHT, FALSE);
		}

		// Vertex Color Section
		{
			auto currentX = WINDOW_EDGE_PADDING;
			auto currentY = clientHeight - WINDOW_EDGE_PADDING - BUTTON_HEIGHT - 10 - VERTEX_COLOR_SECTION_HEIGHT + BASIC_PADDING * 2;

			auto vertexColorGroupbox = GetDlgItem(hWnd, CONTROL_ID_VERTEX_COLOR_GROUPBOX);
			MoveWindow(vertexColorGroupbox, currentX, currentY, sectionWidth, VERTEX_COLOR_SECTION_HEIGHT, FALSE);

			currentX += 9;
			currentY += 21;

			auto primaryColorPreviewStatic = GetDlgItem(hWnd, CONTROL_ID_PRIMARY_COLOR_PREVIEW_STATIC);
			MoveWindow(primaryColorPreviewStatic, currentX + 76, currentY, COLOR_PREVIEW_WIDTH, COLOR_PREVIEW_HEIGHT, FALSE);

			ResizeStaticEditSpinHelper(hWnd, PRIMARY, RED, currentX, currentY);
			ResizeStaticEditSpinHelper(hWnd, PRIMARY, GREEN, currentX, currentY);
			ResizeStaticEditSpinHelper(hWnd, PRIMARY, BLUE, currentX, currentY);

			auto primaryColorSelectColorButton = GetDlgItem(hWnd, CONTROL_ID_PRIMARY_COLOR_SELECT_COLOR_BUTTON);
			MoveWindow(primaryColorSelectColorButton, currentX + 65, currentY - 24, BUTTON_WIDTH, BUTTON_HEIGHT, FALSE);

			currentX += 160;
			currentY = clientHeight - WINDOW_EDGE_PADDING - BUTTON_HEIGHT - 11 - VERTEX_COLOR_SECTION_HEIGHT + BASIC_PADDING * 2;
			currentY += 21;

			auto secondaryColorPreviewStatic = GetDlgItem(hWnd, CONTROL_ID_SECONDARY_COLOR_PREVIEW_STATIC);
			MoveWindow(secondaryColorPreviewStatic, currentX + 76, currentY, COLOR_PREVIEW_WIDTH, COLOR_PREVIEW_HEIGHT, FALSE);

			ResizeStaticEditSpinHelper(hWnd, SECONDARY, RED, currentX, currentY);
			ResizeStaticEditSpinHelper(hWnd, SECONDARY, GREEN, currentX, currentY);
			ResizeStaticEditSpinHelper(hWnd, SECONDARY, BLUE, currentX, currentY);

			auto secondaryColorSelectColorButton = GetDlgItem(hWnd, CONTROL_ID_SECONDARY_COLOR_SELECT_COLOR_BUTTON);
			MoveWindow(secondaryColorSelectColorButton, currentX + 65, currentY - 24, BUTTON_WIDTH, BUTTON_HEIGHT, FALSE);

			auto editColorsButton = GetDlgItem(hWnd, CONTROL_ID_EDIT_COLORS_CHECKBOX);
			MoveWindow(editColorsButton, currentX + 153, currentY - 67, 72, 13, FALSE);

			currentX = WINDOW_EDGE_PADDING + 37;
			currentY += 11;

			ResizeCustomColorStatic(hWnd, CONTROL_ID_CUSTOM_COLOR_ONE_STATIC, currentX, currentY);
			ResizeCustomColorStatic(hWnd, CONTROL_ID_CUSTOM_COLOR_TWO_STATIC, currentX, currentY);
			ResizeCustomColorStatic(hWnd, CONTROL_ID_CUSTOM_COLOR_THREE_STATIC, currentX, currentY);
			ResizeCustomColorStatic(hWnd, CONTROL_ID_CUSTOM_COLOR_FOUR_STATIC, currentX, currentY);
			ResizeCustomColorStatic(hWnd, CONTROL_ID_CUSTOM_COLOR_FIVE_STATIC, currentX, currentY);
			ResizeCustomColorStatic(hWnd, CONTROL_ID_CUSTOM_COLOR_SIX_STATIC, currentX, currentY);
			ResizeCustomColorStatic(hWnd, CONTROL_ID_CUSTOM_COLOR_SEVEN_STATIC, currentX, currentY);
			ResizeCustomColorStatic(hWnd, CONTROL_ID_CUSTOM_COLOR_EIGHT_STATIC, currentX, currentY);

			currentX = WINDOW_EDGE_PADDING + 37;
			currentY += 6 + 15;

			ResizeCustomColorStatic(hWnd, CONTROL_ID_CUSTOM_COLOR_NINE_STATIC, currentX, currentY);
			ResizeCustomColorStatic(hWnd, CONTROL_ID_CUSTOM_COLOR_TEN_STATIC, currentX, currentY);
			ResizeCustomColorStatic(hWnd, CONTROL_ID_CUSTOM_COLOR_ELEVEN_STATIC, currentX, currentY);
			ResizeCustomColorStatic(hWnd, CONTROL_ID_CUSTOM_COLOR_TWELVE_STATIC, currentX, currentY);
			ResizeCustomColorStatic(hWnd, CONTROL_ID_CUSTOM_COLOR_THIRTEEN_STATIC, currentX, currentY);
			ResizeCustomColorStatic(hWnd, CONTROL_ID_CUSTOM_COLOR_FOURTEEN_STATIC, currentX, currentY);
			ResizeCustomColorStatic(hWnd, CONTROL_ID_CUSTOM_COLOR_FIFTEEN_STATIC, currentX, currentY);
			ResizeCustomColorStatic(hWnd, CONTROL_ID_CUSTOM_COLOR_SIXTEEN_STATIC, currentX, currentY);

			auto exitEditingButton = GetDlgItem(hWnd, CONTROL_ID_EXIT_EDITING_BUTTON);
			MoveWindow(exitEditingButton, 127, currentY + 31, 164, BUTTON_HEIGHT, FALSE);
		}

		RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_ERASENOW | RDW_INVALIDATE | RDW_ALLCHILDREN);

		// Store window size for later restoration.
		SIZE winSize = {};
		if (winui::GetWindowSize(hWnd, winSize)) {
			settings.landscape_window.size = winSize;
		}

		context.setResult(TRUE);
	}

	LRESULT CALLBACK PatchDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		DialogProcContext context(hWnd, msg, wParam, lParam, 0x44D470);

		switch (msg) {
		case WM_SIZE:
			PatchDialogProc_BeforeSize(context);
			break;
		case WM_DESTROY:
			PatchDialogProc_BeforeDestroy(context);
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
		case WM_COMMAND:
			PatchDialogProc_AfterCommand(context);
			break;
		case WM_DESTROY:
			PatchDialogProc_AfterDestroy(context);
			break;
		case WM_INITDIALOG:
			PatchDialogProc_AfterInitialize(context);
			break;
		case WM_GETMINMAXINFO:
			PatchDialogProc_GetMinMaxInfo(context);
			break;
		case WM_MOVE:
			PatchDialogProc_AfterMove(context);
			break;
		}

		return context.getResult();
	}

	//
	//
	//

	void installPatches() {
		using memory::genJumpEnforced;

		// Patch: Extend Render Window message handling.
		genJumpEnforced(0x4036A7, 0x44D470, reinterpret_cast<DWORD>(PatchDialogProc));
	}

}
