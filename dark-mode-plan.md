# CSSE Dark Mode / Custom Theming System

## Context

The #1 most requested feature for CSSE is dark mode. The challenge is that CSSE extends a legacy Win32/MFC application (the Morrowind Construction Set) using runtime memory patching. Most dialogs use raw Win32 dialog procedures, not MFC message maps. There is currently **no WM_CTLCOLOR handling** and **no UxTheme usage** anywhere in the codebase — the only color work is NM_CUSTOMDRAW for list view row highlighting and one owner-drawn list in the layers window.

The goal is full, fine-tunable control over UI colors — not just "high contrast mode." Users should be able to pick presets ("dark", "light") and override individual colors in `csse.toml`.

## Architecture: Global CBT Hook + Per-Window Subclassing

**Why this approach?** CSSE currently hooks ~22 of the CS's dialog procs, but many vanilla dialogs (birthsign, class, faction, race, sound editors, etc.) are **not hooked**. Rather than finding and patching dozens more vanilla dialog proc addresses, we install a single **`WH_CBT` hook** (thread-local, zero perf cost) that catches every window creation in the process and applies a subclass proc via `SetWindowSubclass`. This gives us WM_CTLCOLOR interception on every dialog — both CSSE's and vanilla CS's — with one piece of code.

**Four layers:**
1. **Color palette** — semantic colors stored in Settings, loaded from TOML, with preset support
2. **ThemeEngine** — CBT hook, subclass proc, brush cache, message dispatch
3. **Per-control painters** — specialized NM_CUSTOMDRAW / owner-draw for ListView headers, tabs, menus
4. **Integration points** — existing dialog code updated to use theme colors

## Files to Create

### `CSSE/ThemeEngine.h` + `CSSE/ThemeEngine.cpp`

Central theming module containing:

- **`BrushCache`** — `std::unordered_map<COLORREF, HBRUSH>`, lazy-creates brushes, clears on theme change. Solves the WM_CTLCOLOR brush lifetime problem cleanly.
- **`theme::install()`** — Called from `CSSE::InstallPatches()` ([CSSE.cpp:601](CSSE/CSSE.cpp#L601)). Installs `WH_CBT` hook on the current thread via `SetWindowsHookExA`.
- **`theme::shutdown()`** — Unhooks and clears brush cache.
- **`CBTProc`** — On `HCBT_CREATEWND`, calls `SetWindowSubclass(hWnd, ThemeSubclassProc, 1, 0)` on the new window.
- **`ThemeSubclassProc`** — The heart of theming. Intercepts:
  - `WM_CTLCOLORDLG/STATIC/EDIT/LISTBOX/BTN` → sets text/bg color on the HDC, returns themed HBRUSH
  - `WM_ERASEBKGND` → fills dialog background with theme color (skips render viewport)
  - `WM_INITDIALOG` → calls `applyDarkTitleBar()` and `themeChildControls()`
  - `WM_NOTIFY` / `NM_CUSTOMDRAW` → fallback theming for list views not already handled by CSSE dialog procs
  - `WM_NCDESTROY` → `RemoveWindowSubclass` cleanup
- **`handleCtlColor()`** — Maps each WM_CTLCOLOR* message to the appropriate semantic palette color.
- **`applyDarkTitleBar()`** — Calls `DwmSetWindowAttribute(hWnd, 20 /*DWMWA_USE_IMMERSIVE_DARK_MODE*/, ...)` on top-level windows. Runtime feature-detected via `GetProcAddress` for Windows 10 1903+ compat.
- **`themeChildControls()`** — `EnumChildWindows` to:
  - `SetWindowTheme(hwnd, L"", L"")` on ListViews/TreeViews/Tabs to strip visual styles
  - `ListView_SetBkColor/SetTextColor/SetTextBkColor` with theme colors
  - `TreeView_SetBkColor/SetTextColor` with theme colors

## Files to Modify

### [Settings.h](CSSE/Settings.h) — Expand `ColorTheme` struct

Add full semantic palette to the existing `ColorTheme` (lines 175-189):

```cpp
struct ColorTheme {
    // === Existing (preserved) ===
    std::array<unsigned char, 3> highlight_deleted_object_color = { 255, 235, 235 };
    std::array<unsigned char, 3> highlight_modified_from_master_color = { 235, 255, 235 };
    std::array<unsigned char, 3> highlight_modified_new_object_color = { 215, 240, 255 };
    std::array<unsigned char, 3> highlight_deprecated_object_color = { 225, 225, 225 };
    unsigned int highlight_*_packed_color fields...

    // === NEW ===
    std::string preset = "light";    // "light" | "dark" | "custom"
    bool enabled = false;            // master switch for full UI theming
    bool use_dark_title_bar = false;

    // Semantic palette (defaults shown are light-mode; applyPreset("dark") overrides)
    std::array<unsigned char, 3> window_bg        = { 255, 255, 255 };
    std::array<unsigned char, 3> control_bg        = { 255, 255, 255 };
    std::array<unsigned char, 3> text_color         = { 0, 0, 0 };
    std::array<unsigned char, 3> text_disabled      = { 128, 128, 128 };
    std::array<unsigned char, 3> selection_bg       = { 0, 120, 215 };
    std::array<unsigned char, 3> selection_text     = { 255, 255, 255 };
    std::array<unsigned char, 3> button_bg          = { 225, 225, 225 };
    std::array<unsigned char, 3> button_text        = { 0, 0, 0 };
    std::array<unsigned char, 3> edit_bg            = { 255, 255, 255 };
    std::array<unsigned char, 3> edit_text          = { 0, 0, 0 };
    std::array<unsigned char, 3> listview_bg        = { 255, 255, 255 };
    std::array<unsigned char, 3> listview_text      = { 0, 0, 0 };
    std::array<unsigned char, 3> listview_header_bg = { 240, 240, 240 };
    std::array<unsigned char, 3> treeview_bg        = { 255, 255, 255 };
    std::array<unsigned char, 3> treeview_text      = { 0, 0, 0 };
    std::array<unsigned char, 3> tab_active_bg      = { 255, 255, 255 };
    std::array<unsigned char, 3> tab_inactive_bg    = { 240, 240, 240 };
    std::array<unsigned char, 3> menu_bg            = { 255, 255, 255 };
    std::array<unsigned char, 3> menu_text          = { 0, 0, 0 };
    std::array<unsigned char, 3> toolbar_bg         = { 240, 240, 240 };
    std::array<unsigned char, 3> statusbar_bg       = { 240, 240, 240 };
    std::array<unsigned char, 3> statusbar_text     = { 0, 0, 0 };

    // Packed COLORREF cache for all above (computed by packColors)
    unsigned int packed_window_bg, packed_control_bg, packed_text_color, ...;

    void applyPreset(const std::string& name);  // populates palette from preset
    void packColors();  // packs all RGB arrays to COLORREF
    void from_toml(const toml::value& v);
    toml::value into_toml() const;
};
```

### [Settings.cpp](CSSE/Settings.cpp) — TOML serialization + dark preset

- `applyPreset("dark")` fills the palette with curated dark colors (e.g., window_bg={30,30,30}, text={220,220,220}, edit_bg={40,40,40})
- `from_toml` loads preset first, then applies any per-color overrides from TOML
- `packColors()` extended to pack all new fields

### [CSSE.cpp](CSSE/CSSE.cpp) — Hook installation

In `InstallPatches()` (line 601), add `theme::install()` call early (before dialog patches so the CBT hook is in place before any windows are created). Add `theme::shutdown()` in `ExitInstance()`.

### [DialogLayersWindow.cpp](CSSE/DialogLayersWindow.cpp) — Update owner-draw (lines 872-919)

Replace `GetSysColorBrush(COLOR_HIGHLIGHT)` / `GetSysColor(COLOR_WINDOW)` etc. with theme-aware equivalents:
```cpp
if (theme::isEnabled()) {
    FillRect(hDC, &rc, theme::brushCache.get(settings.color_theme.packed_selection_bg));
    SetTextColor(hDC, settings.color_theme.packed_selection_text);
} else {
    FillRect(hDC, &rc, GetSysColorBrush(COLOR_HIGHLIGHT));
    SetTextColor(hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
}
```

### [DialogObjectWindow.cpp](CSSE/DialogObjectWindow.cpp) (line 305), [DialogCellWindow.cpp](CSSE/DialogCellWindow.cpp), [DialogDialogueWindow.cpp](CSSE/DialogDialogueWindow.cpp) — Update NM_CUSTOMDRAW

When theming is active and no special highlight applies, set `clrTextBk` and `clrText` to the theme's listview colors as the default, so un-highlighted rows also get themed.

## Hard Problems & Pragmatic Solutions

| Problem | Solution | Phase |
|---------|----------|-------|
| **Scrollbars** | Skip in v1. System scrollbars are acceptable. Win10/11 dark system theme helps. | Future |
| **Menu bar** | Phase 1: Dark title bar via DwmSetWindowAttribute. Phase 3: Owner-drawn menus via MF_OWNERDRAW + WM_DRAWITEM/WM_MEASUREITEM in main window proc. | 1 + 3 |
| **Title bar** | `DwmSetWindowAttribute(hwnd, 20, TRUE)` — single call per top-level window, Windows 10 1903+. Feature-detect with GetProcAddress. | 1 |
| **MFC Settings dialog** | `CMFCPropertyGridCtrl::SetCustomColors()` for the property grid. CDialogEx backgrounds via WM_CTLCOLORDLG (handled by the global subclass). | 2 |
| **Render viewport** | Exclude from WM_ERASEBKGND. Identify the D3D child window and skip it in ThemeSubclassProc. | 1 |
| **Buttons** | WM_CTLCOLORBTN only changes background behind text, not the button face. For full control: BS_OWNERDRAW + WM_DRAWITEM, or accept partial theming in Phase 1. | 2 |
| **Brush lifecycle** | BrushCache (map COLORREF→HBRUSH). Lazy create, bulk destroy on theme change. ~30-40 brushes max. | 1 |

## Implementation Phases

### Phase 1: Core Infrastructure + Quick Visual Wins
1. Create `ThemeEngine.h/.cpp` with CBT hook, subclass proc, brush cache
2. Expand `ColorTheme` in Settings with full palette + "dark" preset + TOML serialization
3. Implement WM_CTLCOLOR* handling (edit, static, dialog, listbox, button)
4. Implement WM_ERASEBKGND for dialog backgrounds
5. `SetWindowTheme(L"", L"")` + `ListView_SetBkColor/TextColor` on list/tree views
6. `DwmSetWindowAttribute` for dark title bars
7. Call `theme::install()` from `CSSE::InstallPatches()`

**Result: ~80% of UI surface is dark** — dialog backgrounds, text, edit controls, lists, tree views, title bars.

### Phase 2: Control Refinements
1. ListView header NM_CUSTOMDRAW
2. Tab control owner-draw (TCS_OWNERDRAWFIXED + WM_DRAWITEM)
3. ComboBox dropdown theming
4. Button owner-draw for full control
5. Update existing highlight code in ObjectWindow/CellWindow/DialogueWindow
6. Update LayersWindow owner-draw
7. MFC property grid theming

**Result: ~95% visual coverage.**

### Phase 3: Polish
1. Owner-drawn menus (main menu bar)
2. Toolbar theming (dark icon variants)
3. StatusBar owner-draw
4. Script editor (RichEdit) dark background
5. Theme selector in CSSE settings UI
6. Hot-reload support (change TOML → apply without restart)

## Verification Plan

1. **Build**: `msbuild MWSE.sln /p:Configuration=Release /p:Platform=Win32` — must compile clean
2. **Smoke test**: Launch CS with `[color_theme] enabled = true, preset = "dark"` in csse.toml
3. **Visual check each dialog**: Object Window, Cell Window, Dialogue Window, Layers Window, Script Editor, Render Window (verify viewport is NOT affected), all vanilla edit dialogs (NPC, spell, enchantment, etc.)
4. **Light mode check**: With `enabled = false`, verify everything looks exactly as before (no regressions)
5. **Custom color override**: Override a single color in TOML, verify it takes effect
6. **Windows version compat**: Test on Windows 10 and Windows 11 (dark title bar feature detection)
