#include "DialogCSSESettings.h"

#include "CDataBoundPropertyGridProperty.h"

#include "Settings.h"

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

	m_PropertyGrid.AddProperty(new CDataBoundPropertyGridProperty("Enabled", &se::cs::settings.enabled, "This can be used to prevent CSSE from loading at startup. You will need to manually re-enable it in the config file."));

	auto groupObjectsWindow = new CMFCPropertyGridProperty("Objects Window");
	groupObjectsWindow->AddSubItem(new CDataBoundPropertyGridProperty("Change Tab Style", &se::cs::settings.object_window.use_button_style_tabs, "If true, the tab control will use a more button-like style. This will prevent tab rows from jumping to the bottom of the stack when selected."));
	groupObjectsWindow->AddSubItem(new CDataBoundPropertyGridProperty("Highlight Modified", &se::cs::settings.object_window.highlight_modified_items, "If true, modified objects will have a background color."));
	groupObjectsWindow->AddSubItem(new CDataBoundPropertyGridProperty("Case Sensitive", &se::cs::settings.object_window.search_settings.case_sensitive, "If true, searching will be case sensitive."));
	groupObjectsWindow->AddSubItem(new CDataBoundPropertyGridProperty("Use Regex", &se::cs::settings.object_window.search_settings.use_regex, "If true, searching will be performed with regex. The case sensitive option is still used."));
	groupObjectsWindow->AddSubItem(new CDataBoundPropertyGridProperty("Clear Filter on Tab Switch", &se::cs::settings.object_window.clear_filter_on_tab_switch, "If true, the search bar will be cleared when changing tabs."));
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

	return TRUE;
}

afx_msg void DialogCSSESettings::OnGetMinMaxInfo(MINMAXINFO* lpMMI) {
	lpMMI->ptMinTrackSize.x = 377;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void DialogCSSESettings::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SETTINGS_GRID, m_PropertyGrid);
}

BEGIN_MESSAGE_MAP(DialogCSSESettings, CDialogEx)
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()
