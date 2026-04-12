#include "DialogCSSESettings.h"

#include "CDataBoundPropertyGridProperty.h"

#include "Settings.h"
#include "ThemeEngine.h"

namespace {
	using namespace se::cs;

	COLORREF darkenColor(COLORREF color, double amount) {
		amount = std::clamp(amount, 0.0, 1.0);
		const auto scaleChannel = [amount](BYTE channel) {
			return static_cast<BYTE>(std::clamp(
				static_cast<int>(std::lround(channel * (1.0 - amount))),
				0,
				255
			));
		};

		return RGB(scaleChannel(GetRValue(color)), scaleChannel(GetGValue(color)), scaleChannel(GetBValue(color)));
	}

	CMFCPropertyGridProperty* addThemeColorProperty(CMFCPropertyGridProperty* parent, const CString& name, std::array<unsigned char, 3>* value, const CString& description) {
		auto* property = new CDataBoundPropertyGridColorProperty(name, value, description);
		parent->AddSubItem(property);
		return property;
	}
}

IMPLEMENT_DYNAMIC(DialogCSSESettings, CDialogEx);

DialogCSSESettings::DialogCSSESettings(CWnd* pParent /*=nullptr*/) :
	CDialogEx(IDD_CSSE_SETTINGS, pParent)
{

}

DialogCSSESettings::~DialogCSSESettings() {

}

COleVariant getOleBool(bool value) {
	return COleVariant(value ? VARIANT_TRUE : VARIANT_FALSE, VT_BOOL);
}

BOOL DialogCSSESettings::OnInitDialog() {
	CDialogEx::OnInitDialog();

	HDITEMA hdItem = {};
	hdItem.mask = HDI_WIDTH;
	hdItem.cxy = 200;
	m_PropertyGrid.GetHeaderCtrl().SetItem(0, &hdItem);

	BuildPropertyGrid();
	ApplyThemeToPropertyGrid();

	return TRUE;
}

void DialogCSSESettings::BuildPropertyGrid() {
	m_PropertyEnabled = new CDataBoundPropertyGridProperty("Enabled", &se::cs::settings.enabled, "This can be used to prevent CSSE from loading at startup. You will need to manually re-enable it in the config file.");
	m_PropertyGrid.AddProperty(m_PropertyEnabled);

	m_GroupTheme = new CMFCPropertyGridProperty("Theme");
	m_PropertyThemeEnabled = new CDataBoundPropertyGridProperty("Enable Custom Theme", &se::cs::settings.color_theme.enabled, "Turns the custom UI theme engine on or off.");
	m_PropertyThemePreset = new CDataBoundPropertyGridStringProperty("Preset", &se::cs::settings.color_theme.preset, "Choose a built-in theme preset or switch to custom colors.");
	m_PropertyThemeDarkTitleBar = new CDataBoundPropertyGridProperty("Dark Title Bar", &se::cs::settings.color_theme.use_dark_title_bar, "Uses the immersive dark title bar on supported Windows versions.");

	m_PropertyThemePreset->AddOption(_T("light"));
	m_PropertyThemePreset->AddOption(_T("dark"));
	m_PropertyThemePreset->AddOption(_T("custom"));
	m_PropertyThemePreset->AllowEdit(FALSE);

	m_GroupTheme->AddSubItem(m_PropertyThemeEnabled);
	m_GroupTheme->AddSubItem(m_PropertyThemePreset);
	m_GroupTheme->AddSubItem(m_PropertyThemeDarkTitleBar);
	addThemeColorProperty(m_GroupTheme, "Window Background", &se::cs::settings.color_theme.window_bg, "Main dialog and frame backgrounds.");
	addThemeColorProperty(m_GroupTheme, "Control Background", &se::cs::settings.color_theme.control_bg, "Shared background for static controls and standard control chrome.");
	addThemeColorProperty(m_GroupTheme, "Text", &se::cs::settings.color_theme.text_color, "Default text color.");
	addThemeColorProperty(m_GroupTheme, "Disabled Text", &se::cs::settings.color_theme.text_disabled, "Text used for disabled controls and menu items.");
	addThemeColorProperty(m_GroupTheme, "Selection Background", &se::cs::settings.color_theme.selection_bg, "Selection highlight color.");
	addThemeColorProperty(m_GroupTheme, "Selection Text", &se::cs::settings.color_theme.selection_text, "Text color drawn on selections.");
	addThemeColorProperty(m_GroupTheme, "Button Background", &se::cs::settings.color_theme.button_bg, "Owner-drawn button background color.");
	addThemeColorProperty(m_GroupTheme, "Button Text", &se::cs::settings.color_theme.button_text, "Owner-drawn button text and icon tint.");
	addThemeColorProperty(m_GroupTheme, "Edit Background", &se::cs::settings.color_theme.edit_bg, "Edit box and combo edit background.");
	addThemeColorProperty(m_GroupTheme, "Edit Text", &se::cs::settings.color_theme.edit_text, "Edit box text color.");
	addThemeColorProperty(m_GroupTheme, "List Background", &se::cs::settings.color_theme.listview_bg, "List view background.");
	addThemeColorProperty(m_GroupTheme, "List Text", &se::cs::settings.color_theme.listview_text, "List view text color.");
	addThemeColorProperty(m_GroupTheme, "List Header Background", &se::cs::settings.color_theme.listview_header_bg, "List view header background.");
	addThemeColorProperty(m_GroupTheme, "Tree Background", &se::cs::settings.color_theme.treeview_bg, "Tree view background.");
	addThemeColorProperty(m_GroupTheme, "Tree Text", &se::cs::settings.color_theme.treeview_text, "Tree view text color.");
	addThemeColorProperty(m_GroupTheme, "Tab Active Background", &se::cs::settings.color_theme.tab_active_bg, "Selected tab background.");
	addThemeColorProperty(m_GroupTheme, "Tab Inactive Background", &se::cs::settings.color_theme.tab_inactive_bg, "Unselected tab background.");
	addThemeColorProperty(m_GroupTheme, "Menu Background", &se::cs::settings.color_theme.menu_bg, "Menu and menu bar background.");
	addThemeColorProperty(m_GroupTheme, "Menu Text", &se::cs::settings.color_theme.menu_text, "Menu text color.");
	addThemeColorProperty(m_GroupTheme, "Toolbar Background", &se::cs::settings.color_theme.toolbar_bg, "Toolbar background.");
	addThemeColorProperty(m_GroupTheme, "Status Bar Background", &se::cs::settings.color_theme.statusbar_bg, "Status bar background.");
	addThemeColorProperty(m_GroupTheme, "Status Bar Text", &se::cs::settings.color_theme.statusbar_text, "Status bar text color.");
	addThemeColorProperty(m_GroupTheme, "Deleted Highlight", &se::cs::settings.color_theme.highlight_deleted_object_color, "Highlight for deleted records.");
	addThemeColorProperty(m_GroupTheme, "Modified Master Highlight", &se::cs::settings.color_theme.highlight_modified_from_master_color, "Highlight for changed records that come from a master file.");
	addThemeColorProperty(m_GroupTheme, "Modified New Highlight", &se::cs::settings.color_theme.highlight_modified_new_object_color, "Highlight for changed records created in the active file.");
	addThemeColorProperty(m_GroupTheme, "Deprecated Highlight", &se::cs::settings.color_theme.highlight_deprecated_object_color, "Highlight for deprecated records.");
	m_GroupTheme->Expand();
	m_PropertyGrid.AddProperty(m_GroupTheme);

	auto groupObjectsWindow = new CMFCPropertyGridProperty("Objects Window");
	groupObjectsWindow->AddSubItem(new CDataBoundPropertyGridProperty("Change Tab Style", &se::cs::settings.object_window.use_button_style_tabs, "If true, the tab control will use a more button-like style. This will prevent tab rows from jumping to the bottom of the stack when selected."));
	groupObjectsWindow->AddSubItem(new CDataBoundPropertyGridProperty("Highlight Modified", &se::cs::settings.object_window.highlight_modified_items, "If true, modified objects will have a background color."));
	groupObjectsWindow->AddSubItem(new CDataBoundPropertyGridProperty("Case Sensitive", &se::cs::settings.object_window.search_settings.case_sensitive, "If true, searching will be case sensitive."));
	groupObjectsWindow->AddSubItem(new CDataBoundPropertyGridProperty("Use Regex", &se::cs::settings.object_window.search_settings.use_regex, "If true, searching will be performed with regex. The case sensitive option is still used."));
	groupObjectsWindow->AddSubItem(new CDataBoundPropertyGridProperty("Clear Filter on Tab Switch", &se::cs::settings.object_window.clear_filter_on_tab_switch, "If true, the search bar will be cleared when changing tabs."));
	groupObjectsWindow->AddSubItem(new CDataBoundPropertyGridProperty("Hide Deprecated", &se::cs::settings.object_window.hide_deprecated, "If true, objects marked as deprecated will be hidden from the object window."));
	{
		auto filterByGroup = new CMFCPropertyGridProperty("Filter By");
		filterByGroup->AddSubItem(new CDataBoundPropertyGridProperty("ID", &se::cs::settings.object_window.search_settings.id, "If true, the object's ID will be searched when filtering."));
		filterByGroup->AddSubItem(new CDataBoundPropertyGridProperty("Name", &se::cs::settings.object_window.search_settings.name, "If true, the object's name will be searched when filtering."));
		filterByGroup->AddSubItem(new CDataBoundPropertyGridProperty("Script ID", &se::cs::settings.object_window.search_settings.script_id, "If true, the script's ID will be searched when filtering."));
		filterByGroup->AddSubItem(new CDataBoundPropertyGridProperty("Enchantment ID", &se::cs::settings.object_window.search_settings.enchantment_id, "If true, the enchantment's ID will be searched when filtering."));
		filterByGroup->AddSubItem(new CDataBoundPropertyGridProperty("Icon Path", &se::cs::settings.object_window.search_settings.icon_path, "If true, the object's icon path will be searched when filtering."));
		filterByGroup->AddSubItem(new CDataBoundPropertyGridProperty("Model Path", &se::cs::settings.object_window.search_settings.model_path, "If true, the object's model path will be searched when filtering."));
		filterByGroup->AddSubItem(new CDataBoundPropertyGridProperty("Book Text", &se::cs::settings.object_window.search_settings.book_text, "If true, book text will be searched when filtering."));
		filterByGroup->AddSubItem(new CDataBoundPropertyGridProperty("Faction", &se::cs::settings.object_window.search_settings.faction, "If true, faction IDs and rank names will be searched when filtering NPCs."));
		filterByGroup->AddSubItem(new CDataBoundPropertyGridProperty("Effect", &se::cs::settings.object_window.search_settings.effect, "If true, effects will be searched when filtering alchemy, spell, enchantment, or ingredients."));
		filterByGroup->AddSubItem(new CDataBoundPropertyGridProperty("Training Skills", &se::cs::settings.object_window.search_settings.training, "If true, NPC's training skills will be searched when filtering NPCs."));

		groupObjectsWindow->AddSubItem(filterByGroup);
	}
	m_PropertyGrid.AddProperty(groupObjectsWindow);

	auto groupDialogueWindow = new CMFCPropertyGridProperty("Dialogue Window");
	groupDialogueWindow->AddSubItem(new CDataBoundPropertyGridProperty("Highlight Modified", &se::cs::settings.dialogue_window.highlight_modified_items, "If true, modified INFO records will have a background color."));
	m_PropertyGrid.AddProperty(groupDialogueWindow);

	auto groupRenderWindow = new CMFCPropertyGridProperty("Render Window");
	groupRenderWindow->AddSubItem(new CDataBoundPropertyGridProperty("Field of View", &se::cs::settings.render_window.fov, "The field of view used by the construction set's render window. The default FOV is ~53."));
	groupRenderWindow->AddSubItem(new CDataBoundPropertyGridProperty("Multisamples", &se::cs::settings.render_window.multisamples, "The antialiasing level to use for the render window."));
	groupRenderWindow->AddSubItem(new CDataBoundPropertyGridProperty("FPS Limit", &se::cs::settings.render_window.fps_limit, "By default, the CS won't update more than 25 times per second. This setting overrides this limiter. Frame timings are not exact, and a value of 60 will actually result in a limit of ~62 FPS."));
	groupRenderWindow->AddSubItem(new CDataBoundPropertyGridProperty("Use Legacy Camera", &se::cs::settings.render_window.use_legacy_camera, "If true, the CSSE's new camera implementation will be disabled."));
	groupRenderWindow->AddSubItem(new CDataBoundPropertyGridProperty("Use Legacy Movement", &se::cs::settings.render_window.use_legacy_object_movement, "If true, the CSSE's new movement operations will be disabled."));
	groupRenderWindow->AddSubItem(new CDataBoundPropertyGridProperty("Use Legacy Grid Snap", &se::cs::settings.render_window.use_legacy_grid_snap, "When enabled, the default movement behavior while grid snapping will be to snap objects on all axes. When disabled, snapping only happens on the actual axis of movement (X/Y)."));
	groupRenderWindow->AddSubItem(new CDataBoundPropertyGridProperty("Use World Axis Rotations", &se::cs::settings.render_window.use_world_axis_rotations_by_default, "If true, world axis rotations are performed instead of local axis rotations."));
	groupRenderWindow->AddSubItem(new CDataBoundPropertyGridProperty("Use Group Scaling", &se::cs::settings.render_window.use_group_scaling, "If true, all selected objects will be scaled together, and move appropriately to keep consistent spacing. The CS by default scales items independently and keep them in-place."));
	m_PropertyGrid.AddProperty(groupRenderWindow);

	/*
	auto groupScriptEditor = new CMFCPropertyGridProperty("Script Editor");
	groupScriptEditor->AddSubItem(new CMFCPropertyGridFontProperty("Font", &se::cs::settings.script_editor.font_face, CF_SCREENFONTS, "The font to use."));
	groupScriptEditor->AddSubItem(new CDataBoundPropertyGridProperty("Font Size", &se::cs::settings.script_editor.font_size, "The size of the font to use."));
	m_PropertyGrid.AddProperty(groupScriptEditor);
	*/

	auto groupTextSearch = new CMFCPropertyGridProperty("TextSearch");
	groupTextSearch->AddSubItem(new CDataBoundPropertyGridProperty("Case Sensitive", &se::cs::settings.text_search.search_settings.case_sensitive, "If true, searching will be case sensitive."));
	groupTextSearch->AddSubItem(new CDataBoundPropertyGridProperty("Use Regex", &se::cs::settings.text_search.search_settings.use_regex, "If true, searching will be performed with regex. The case sensitive option is still used."));
	m_PropertyGrid.AddProperty(groupTextSearch);

	auto groupQuickStart = new CMFCPropertyGridProperty("QuickStart");
	groupQuickStart->AddSubItem(new CDataBoundPropertyGridProperty("Enabled", &se::cs::settings.quickstart.enabled, "Determines if the QuickStart feature is used on startup."));
	groupQuickStart->AddSubItem(new CDataBoundPropertyGridProperty("Load Cell", &se::cs::settings.quickstart.load_cell, "Should the CS automatically load a cell? If false, data files will still load."));
	m_PropertyGrid.AddProperty(groupQuickStart);
}

afx_msg void DialogCSSESettings::OnGetMinMaxInfo(MINMAXINFO* lpMMI) {
	lpMMI->ptMinTrackSize.x = 377;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void DialogCSSESettings::ApplyThemeToPropertyGrid() {
	if (theme::isEnabled()) {
		const auto& ct = se::cs::settings.color_theme;
		m_PropertyGrid.SetCustomColors(
			ct.packed_window_bg,
			ct.packed_text_color,
			ct.packed_control_bg,
			ct.packed_text_color,
			ct.packed_control_bg,
			ct.packed_text_color,
			darkenColor(ct.packed_control_bg, 0.20)
		);
	}
	else {
		m_PropertyGrid.SetCustomColors((COLORREF)-1, (COLORREF)-1, (COLORREF)-1, (COLORREF)-1, (COLORREF)-1, (COLORREF)-1, (COLORREF)-1);
	}

	m_PropertyGrid.RedrawWindow();
}

void DialogCSSESettings::SyncThemePropertyValues(CMFCPropertyGridProperty* property) {
	if (property == nullptr) {
		return;
	}

	if (property == m_PropertyThemePreset) {
		property->SetValue(COleVariant(CString(se::cs::settings.color_theme.preset.c_str())));
	}
	else if (property == m_PropertyThemeEnabled) {
		property->SetValue(COleVariant(se::cs::settings.color_theme.enabled ? VARIANT_TRUE : VARIANT_FALSE, VT_BOOL));
	}
	else if (property == m_PropertyThemeDarkTitleBar) {
		property->SetValue(COleVariant(se::cs::settings.color_theme.use_dark_title_bar ? VARIANT_TRUE : VARIANT_FALSE, VT_BOOL));
	}
	else if (auto* colorProperty = dynamic_cast<CDataBoundPropertyGridColorProperty*>(property)) {
		auto* value = reinterpret_cast<std::array<unsigned char, 3>*>(colorProperty->GetData());
		if (value) {
			colorProperty->SetColor(RGB((*value)[0], (*value)[1], (*value)[2]));
		}
	}

	for (auto index = 0; index < property->GetSubItemsCount(); ++index) {
		SyncThemePropertyValues(property->GetSubItem(index));
	}
}

bool DialogCSSESettings::IsThemeProperty(const CMFCPropertyGridProperty* property) const {
	for (auto* current = property; current != nullptr; current = current->GetParent()) {
		if (current == m_GroupTheme) {
			return true;
		}
	}

	return false;
}

afx_msg LRESULT DialogCSSESettings::OnPropertyChanged(WPARAM wParam, LPARAM lParam) {
	auto* changedProperty = reinterpret_cast<CMFCPropertyGridProperty*>(lParam);
	if (changedProperty == nullptr) {
		return 0;
	}

	if (changedProperty == m_PropertyThemePreset) {
		if (se::cs::settings.color_theme.preset == "light" || se::cs::settings.color_theme.preset == "dark") {
			se::cs::settings.color_theme.applyPreset(se::cs::settings.color_theme.preset);
		}
	}
	else if (IsThemeProperty(changedProperty)
		&& changedProperty != m_PropertyThemeEnabled
		&& changedProperty != m_PropertyThemeDarkTitleBar) {
		se::cs::settings.color_theme.preset = "custom";
	}

	if (IsThemeProperty(changedProperty)) {
		se::cs::settings.color_theme.packColors();
		SyncThemePropertyValues(m_GroupTheme);
		ApplyThemeToPropertyGrid();
		theme::refresh();
	}

	se::cs::settings.save();
	return 0;
}

void DialogCSSESettings::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SETTINGS_GRID, m_PropertyGrid);
}

BEGIN_MESSAGE_MAP(DialogCSSESettings, CDialogEx)
	ON_WM_GETMINMAXINFO()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, &DialogCSSESettings::OnPropertyChanged)
END_MESSAGE_MAP()
