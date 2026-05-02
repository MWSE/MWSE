#pragma once

namespace se::cs {
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
		NMLVDISPINFOA* getNotificationListViewDisplayInfo() const;
		NMLVCUSTOMDRAW* getNotificationCustomDraw() const;
		NMITEMACTIVATE* getNotificationItemActivateData() const;
		NMTTDISPINFOA* getNotificationTooltipGetDisplayInfo() const;


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
