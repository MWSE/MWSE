#pragma once

#include "LuaExecutor.h"

namespace mwse::lua {
	// These are only set once on initialized in LuaManager.
	extern int64_t performanceFrequency;
	extern int64_t startTimestamp;

	double getHighPrecisionClock();

	size_t getVirtualMemoryUsage();

	std::optional<std::string> getClipboardText();
	bool setClipboardText(std::optional<std::string> text);

	sol::table getCommandLine(sol::this_state ts);

	sol::optional<std::shared_ptr<LuaExecutor>> createProcess(sol::table params);
	
	void openURL(const std::string& url);

	bool getIsVirtualKeyPressed(int VK_key);
}

namespace mwse::windows {
	std::optional<std::wstring> GetThreadDescription(HANDLE thread);
	bool SetThreadDescription(HANDLE thread, const std::wstring_view& description);

	class DialogProcContext {
	public:
		DialogProcContext(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, DWORD originalAddress);

		HWND getWindowHandle() const;
		UINT getMessage() const;
		WPARAM getWParam() const;
		LPARAM getLParam() const;

		void callOriginalFunction();

		bool hasResult() const;
		LRESULT getResult() const;
		void setResult(LRESULT result);

		//
		// Convenience functions for reinterpretting WPARAM/LPARAM values.
		//

		WORD getLOWParam() const;
		WORD getHIWParam() const;
		WORD getLOLParam() const;
		WORD getHILParam() const;

		template <typename T>
		T* getUserData() const {
			return (T*)GetWindowLongA(m_WindowHandle, GWL_USERDATA);
		}

		// WM_INITDIALOG
		HWND getDefaultFocus() const;

		// WM_NOTIFY
		WPARAM getNotificationControlIdentifier() const;
		NMHDR* getNotificationData() const;

		// WM_COMMAND
		WORD getCommandNotificationCode() const;
		WORD getCommandControlIdentifier() const;

		// WM_GETMINMAXINFO
		MINMAXINFO* getMinMaxInfo() const;

		// WM_SIZE
		WORD getSizeX() const;
		WORD getSizeY() const;

		// WM_MOVE
		int getMovedX() const;
		int getMovedY() const;

		// WM_MOUSEMOVE
		int getMouseMoveX() const;
		int getMouseMoveY() const;

		// WM_MOUSEWHEEL
		short getMouseWheelDelta() const;

		// WM_KEYUP
		WPARAM getKeyVirtualCode() const;
		WORD getKeyFlags() const;
		WORD getKeyScanCode() const;
		bool getKeyIsExtended() const;
		bool getKeyWasDown() const;
		bool getKeyIsReleased() const;
		WORD getKeyRepeatCount() const;

	private:
		WNDPROC m_OriginalFunction;
		HWND m_WindowHandle;
		UINT m_Message;
		WPARAM m_WParam;
		LPARAM m_LParam;
		std::optional<LRESULT> m_Result;
	};
}
