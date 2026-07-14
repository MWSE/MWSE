#include "IconOverride.h"

#include "DarkMode.h"
#include "LogUtil.h"
#include "MemoryUtil.h"
#include "PathUtil.h"

extern "C" IMAGE_DOS_HEADER __ImageBase;

namespace se::cs::iconoverride {
	namespace fs = std::filesystem;

	static bool overridesAvailable = false;
	static fs::path overrideDirectory;

	// Original import targets, saved before the IAT slots are redirected.
	static decltype(&LoadIconA) realLoadIconA = nullptr;
	static decltype(&LoadImageA) realLoadImageA = nullptr;
	static decltype(&ImageList_LoadImageA) realImageList_LoadImageA = nullptr;
	static decltype(&CreateToolbarEx) realCreateToolbarEx = nullptr;

	enum class Kind { bitmap, icon };

	struct NamedResource {
		UINT id;
		Kind kind;
		const char* name;
	};

	// Friendly file names for the CS exe resources that reach the hooks below.
	// scripts/extract_cs_icons.py extracts the original art as templates under
	// these names; csse_icon_overrides.md documents the required dimensions.
	static constexpr NamedResource exeResources[] = {
		// Toolbar strips, 16x16 cells, silver RGB(192,192,192) keyed.
		{ 152, Kind::bitmap, "toolbar_main" },		// Unused while PatchReplaceToolbarBitmap supplies CSSE's strip.
		{ 194, Kind::bitmap, "toolbar_render" },	// Render + Preview windows. 112x16.
		{ 219, Kind::bitmap, "toolbar_object" },	// Object Window + Script Editor. 112x16.

		// Record/spell type strip used by the Object Window, Cell View, and the
		// NPC/Creature/Race spell lists. 384x16, 24 cells, teal RGB(0,128,128) keyed.
		{ 178, Kind::bitmap, "object_icons" },

		// Show Scene Graph browser tiles, 16x15 (107/108 are 16x16), drawn opaque.
		{ 103, Kind::bitmap, "scenegraph_103" },
		{ 104, Kind::bitmap, "scenegraph_104" },
		{ 105, Kind::bitmap, "scenegraph_105" },
		{ 106, Kind::bitmap, "scenegraph_106" },
		{ 107, Kind::bitmap, "scenegraph_107" },
		{ 108, Kind::bitmap, "scenegraph_108" },
		{ 109, Kind::bitmap, "scenegraph_109" },
		{ 113, Kind::bitmap, "scenegraph_113" },

		// Window class icons (MDI child captions), 32x32.
		{ 128, Kind::icon, "window_main" },
		{ 129, Kind::icon, "window_cell_view" },	// Also the Preview window class.
		{ 130, Kind::icon, "window_plane" },
		{ 131, Kind::icon, "window_render" },
		{ 132, Kind::icon, "window_face" },
		{ 143, Kind::icon, "window_weapon" },
		{ 144, Kind::icon, "window_npc" },
		{ 145, Kind::icon, "window_armor" },
		{ 146, Kind::icon, "window_creature" },
		{ 149, Kind::icon, "window_lockpick" },
		{ 150, Kind::icon, "window_alchemy" },
		{ 151, Kind::icon, "window_activator" },
		{ 191, Kind::icon, "window_speaker" },		// Also the Land window class.

		// Data Files dialog state marks, 16x16. Overriding any of these
		// disables the black-stroke recolor hack, so supply all five.
		{ 133, Kind::icon, "datafiles_check_checked" },
		{ 134, Kind::icon, "datafiles_check_unchecked" },
	};

	static std::string getOverrideName(HINSTANCE hInstance, UINT resourceId, Kind kind) {
		if (hInstance == GetModuleHandleA(nullptr)) {
			for (const auto& resource : exeResources) {
				if (resource.id == resourceId && resource.kind == kind) {
					return resource.name;
				}
			}
			// Unmapped exe resources stay overridable by raw id.
			return (kind == Kind::bitmap ? "bitmap_" : "icon_") + std::to_string(resourceId);
		}
		if (hInstance == reinterpret_cast<HINSTANCE>(&__ImageBase) && kind == Kind::bitmap && resourceId == IDB_MAIN_TOOLBAR) {
			// CSSE's own replacement strip; see PatchReplaceToolbarBitmap.
			return "toolbar_main";
		}
		return {};
	}

	static std::optional<fs::path> findOverrideFile(HINSTANCE hInstance, UINT resourceId, Kind kind) {
		if (!overridesAvailable) {
			return {};
		}

		const auto name = getOverrideName(hInstance, resourceId, kind);
		if (name.empty()) {
			return {};
		}

		const char* extensions[] = { ".png", kind == Kind::bitmap ? ".bmp" : ".ico" };
		for (const auto extension : extensions) {
			auto file = overrideDirectory / (name + extension);
			std::error_code ec;
			if (fs::is_regular_file(file, ec)) {
				return file;
			}
		}
		return {};
	}

	// Each file is logged once on first use so artists can confirm pickup
	// (or load failures) in csse.log.
	static void logResult(const fs::path& file, bool loaded) {
		static std::set<std::string> logged;
		const auto name = file.filename().string();
		if (logged.insert(name).second) {
			log::stream << "Icon override: " << (loaded ? "applied " : "failed to load ") << name << "." << std::endl;
		}
	}

	//
	// Image loading.
	//

	static bool ensureGdiplus() {
		// Started on first use; deliberately never shut down.
		static const ULONG_PTR token = [] {
			ULONG_PTR value = 0;
			Gdiplus::GdiplusStartupInput input;
			if (Gdiplus::GdiplusStartup(&value, &input, nullptr) != Gdiplus::Ok) {
				value = 0;
			}
			return value;
		}();
		return token != 0;
	}

	static HBITMAP createDibSection32(int width, int height, void** bits) {
		BITMAPINFO info = {};
		info.bmiHeader.biSize = sizeof(info.bmiHeader);
		info.bmiHeader.biWidth = width;
		info.bmiHeader.biHeight = -height;
		info.bmiHeader.biPlanes = 1;
		info.bmiHeader.biBitCount = 32;
		info.bmiHeader.biCompression = BI_RGB;
		return CreateDIBSection(nullptr, &info, DIB_RGB_COLORS, bits, nullptr, 0);
	}

	// Loads a PNG as a 32bpp premultiplied-alpha DIB section, the format
	// comctl32 v6 image lists composite with per-pixel alpha.
	static HBITMAP loadPngAsDib(const fs::path& file) {
		if (!ensureGdiplus()) {
			return nullptr;
		}

		Gdiplus::Bitmap source(file.c_str());
		if (source.GetLastStatus() != Gdiplus::Ok) {
			return nullptr;
		}

		const int width = source.GetWidth();
		const int height = source.GetHeight();
		void* bits = nullptr;
		const auto dib = createDibSection32(width, height, &bits);
		if (dib == nullptr) {
			return nullptr;
		}

		Gdiplus::Rect rect(0, 0, width, height);
		Gdiplus::BitmapData locked = {};
		if (source.LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppPARGB, &locked) != Gdiplus::Ok) {
			DeleteObject(dib);
			return nullptr;
		}
		for (int y = 0; y < height; ++y) {
			memcpy(static_cast<BYTE*>(bits) + y * width * 4, static_cast<const BYTE*>(locked.Scan0) + y * locked.Stride, width * 4u);
		}
		source.UnlockBits(&locked);
		return dib;
	}

	static HICON loadPngAsIcon(const fs::path& file) {
		if (!ensureGdiplus()) {
			return nullptr;
		}

		Gdiplus::Bitmap source(file.c_str());
		if (source.GetLastStatus() != Gdiplus::Ok) {
			return nullptr;
		}

		HICON icon = nullptr;
		if (source.GetHICON(&icon) != Gdiplus::Ok) {
			return nullptr;
		}
		return icon;
	}

	// Returns a 32bpp copy of a bitmap with an opaque alpha channel, for use
	// in ILC_COLOR32 image lists where a zeroed alpha channel draws invisible.
	static HBITMAP toOpaque32bppDib(HBITMAP source) {
		BITMAP info = {};
		if (GetObjectA(source, sizeof(info), &info) != sizeof(info)) {
			return nullptr;
		}

		void* bits = nullptr;
		const auto dib = createDibSection32(info.bmWidth, info.bmHeight, &bits);
		if (dib == nullptr) {
			return nullptr;
		}

		BITMAPINFO dibInfo = {};
		dibInfo.bmiHeader.biSize = sizeof(dibInfo.bmiHeader);
		dibInfo.bmiHeader.biWidth = info.bmWidth;
		dibInfo.bmiHeader.biHeight = -info.bmHeight;
		dibInfo.bmiHeader.biPlanes = 1;
		dibInfo.bmiHeader.biBitCount = 32;
		dibInfo.bmiHeader.biCompression = BI_RGB;
		const auto hdc = GetDC(nullptr);
		const auto lines = GetDIBits(hdc, source, 0, info.bmHeight, bits, &dibInfo, DIB_RGB_COLORS);
		ReleaseDC(nullptr, hdc);
		if (lines != info.bmHeight) {
			DeleteObject(dib);
			return nullptr;
		}

		const auto pixels = static_cast<DWORD*>(bits);
		for (int i = 0; i < info.bmWidth * info.bmHeight; ++i) {
			pixels[i] |= 0xFF000000;
		}
		return dib;
	}

	HBITMAP loadBitmapOverride(HINSTANCE hInstance, UINT resourceId, bool& outHasAlpha) {
		outHasAlpha = false;

		const auto file = findOverrideFile(hInstance, resourceId, Kind::bitmap);
		if (!file) {
			return nullptr;
		}

		HBITMAP bitmap;
		if (file->extension() == L".png") {
			bitmap = loadPngAsDib(*file);
			outHasAlpha = bitmap != nullptr;
		}
		else {
			bitmap = reinterpret_cast<HBITMAP>(LoadImageW(nullptr, file->c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION));
		}

		logResult(*file, bitmap != nullptr);
		return bitmap;
	}

	bool hasDataFilesCheckOverrides() {
		const auto exe = GetModuleHandleA(nullptr);
		for (UINT id = 133; id <= 137; ++id) {
			if (findOverrideFile(exe, id, Kind::icon)) {
				return true;
			}
		}
		return false;
	}

	//
	// Import hooks. Resource ids only reach the override lookup for the CS exe
	// instance (and CSSE's toolbar strip); everything else passes through.
	//

	static HICON WINAPI hookLoadIconA(HINSTANCE hInstance, LPCSTR lpIconName) {
		if (IS_INTRESOURCE(lpIconName)) {
			const auto id = static_cast<UINT>(reinterpret_cast<ULONG_PTR>(lpIconName));
			if (const auto file = findOverrideFile(hInstance, id, Kind::icon)) {
				HICON icon;
				if (file->extension() == L".png") {
					icon = loadPngAsIcon(*file);
				}
				else {
					icon = reinterpret_cast<HICON>(LoadImageW(nullptr, file->c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE));
				}

				logResult(*file, icon != nullptr);
				if (icon) {
					// Unlike LoadIconA's shared icons this is caller-owned, but
					// every CS caller either keeps it for the process lifetime
					// (window class icons) or destroys it after copying it into
					// an image list (Data Files check marks).
					return icon;
				}
			}
		}
		return realLoadIconA(hInstance, lpIconName);
	}

	static HANDLE WINAPI hookLoadImageA(HINSTANCE hInst, LPCSTR name, UINT type, int cx, int cy, UINT fuLoad) {
		// The exe's only bitmap LoadImageA caller builds the Show Scene Graph
		// image list, whose creation flags are promoted to ILC_COLOR32.
		if (type != IMAGE_BITMAP || !IS_INTRESOURCE(name)) {
			return realLoadImageA(hInst, name, type, cx, cy, fuLoad);
		}

		const auto id = static_cast<UINT>(reinterpret_cast<ULONG_PTR>(name));
		bool hasAlpha = false;
		auto bitmap = loadBitmapOverride(hInst, id, hasAlpha);
		if (bitmap == nullptr) {
			bitmap = reinterpret_cast<HBITMAP>(realLoadImageA(hInst, name, type, cx, cy, fuLoad));
		}
		if (bitmap != nullptr && !hasAlpha) {
			if (const auto converted = toOpaque32bppDib(bitmap)) {
				DeleteObject(bitmap);
				bitmap = converted;
			}
		}
		return bitmap;
	}

	static HIMAGELIST WINAPI hookImageList_LoadImageA(HINSTANCE hi, LPCSTR lpbmp, int cx, int cGrow, COLORREF crMask, UINT uType, UINT uFlags) {
		if (uType == IMAGE_BITMAP && IS_INTRESOURCE(lpbmp)) {
			const auto id = static_cast<UINT>(reinterpret_cast<ULONG_PTR>(lpbmp));
			bool hasAlpha = false;
			if (const auto bitmap = loadBitmapOverride(hi, id, hasAlpha)) {
				BITMAP info = {};
				GetObjectA(bitmap, sizeof(info), &info);
				const int width = cx > 0 ? cx : static_cast<int>(info.bmWidth);
				const int count = std::max(1, static_cast<int>(info.bmWidth) / std::max(1, width));
				const auto imageList = darkmode::buildImageList(bitmap, width, info.bmHeight, count, cGrow, hasAlpha, crMask);
				DeleteObject(bitmap);
				if (imageList) {
					return imageList;
				}
			}
		}
		return realImageList_LoadImageA(hi, lpbmp, cx, cGrow, crMask, uType, uFlags);
	}

	static HWND WINAPI hookCreateToolbarEx(HWND hwnd, DWORD ws, UINT wID, int nBitmaps, HINSTANCE hBMInst, UINT_PTR wBMID, LPCTBBUTTON lpButtons, int iNumButtons, int dxButton, int dyButton, int dxBitmap, int dyBitmap, UINT uStructSize) {
		const auto toolbar = realCreateToolbarEx(hwnd, ws, wID, nBitmaps, hBMInst, wBMID, lpButtons, iNumButtons, dxButton, dyButton, dxBitmap, dyBitmap, uStructSize);

		// Composites the strip over the dark toolbar fill and picks up any
		// override. This reaches the Render/Preview and Object/Script Editor
		// toolbars; the main toolbar goes through PatchReplaceToolbarBitmap.
		darkmode::remapToolbarImages(toolbar, hBMInst, static_cast<UINT>(wBMID), dxBitmap, dyBitmap);
		return toolbar;
	}

	template <typename T>
	static void hookImportSlot(DWORD slotAddress, T& realFunction, T hookFunction) {
		realFunction = memory::MemAccess<T>::Get(slotAddress);
		memory::writeDoubleWordUnprotected(slotAddress, reinterpret_cast<DWORD>(hookFunction));
	}

	void initialize() {
		// Dark-only by design: light mode stays byte-identical to vanilla.
		if (!darkmode::isActive()) {
			return;
		}

		overrideDirectory = path::getDataFilesPath() / "MWSE" / "core" / "csse" / "icons" / "dark";
		std::error_code ec;
		overridesAvailable = fs::is_directory(overrideDirectory, ec);
		if (overridesAvailable) {
			log::stream << "Icon overrides: loading from " << overrideDirectory.string() << "." << std::endl;
		}

		hookImportSlot(0x6D9DA0, realLoadIconA, hookLoadIconA);
		hookImportSlot(0x6D9E38, realLoadImageA, hookLoadImageA);
		hookImportSlot(0x6D9918, realImageList_LoadImageA, hookImageList_LoadImageA);
		hookImportSlot(0x6D990C, realCreateToolbarEx, hookCreateToolbarEx);

		// Allow 32-bpp icons in a few lists.
		memory::genPushEnforced(0x411DBF, static_cast<BYTE>(ILC_COLOR32 | ILC_MASK)); // Data Files
		memory::genPushEnforced(0x493178, static_cast<BYTE>(ILC_COLOR32)); // Show Scene Graph
	}
}
