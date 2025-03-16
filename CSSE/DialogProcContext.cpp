#include "DialogProcContext.h"

namespace se::cs {
	DialogProcContext::DialogProcContext(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, DWORD originalAddress) :
		m_WindowHandle(hWnd),
		m_Message(msg),
		m_WParam(wParam),
		m_LParam(lParam),
		m_OriginalFunction(reinterpret_cast<WNDPROC>(originalAddress))
	{

	}

	HWND DialogProcContext::getWindowHandle() const {
		return m_WindowHandle;
	}

	UINT DialogProcContext::getMessage() const {
		return m_Message;
	}

	WPARAM DialogProcContext::getWParam() const {
		return m_WParam;
	}

	LPARAM DialogProcContext::getLParam() const {
		return m_LParam;
	}

	void DialogProcContext::callOriginalFunction() {
		m_Result = m_OriginalFunction(getWindowHandle(), getMessage(), getWParam(), getLParam());
	}

	bool DialogProcContext::hasResult() const {
		return m_Result.has_value();
	}

	LRESULT DialogProcContext::getResult() const {
		return m_Result.value();
	}

	void DialogProcContext::setResult(LRESULT result) {
		m_Result = result;
	}

	WORD DialogProcContext::getLOWParam() const {
		return LOWORD(m_WParam);
	}

	WORD DialogProcContext::getHIWParam() const {
		return HIWORD(m_WParam);
	}

	WORD DialogProcContext::getLOLParam() const {
		return LOWORD(m_LParam);
	}

	WORD DialogProcContext::getHILParam() const {
		return HIWORD(m_LParam);
	}

	HWND DialogProcContext::getDefaultFocus() const {
#if _DEBUG
		assert(getMessage() == WM_INITDIALOG);
#endif
		return (HWND)getWParam();
	}

	WPARAM DialogProcContext::getNotificationControlIdentifier() const {
#if _DEBUG
		assert(getMessage() == WM_NOTIFY);
#endif
		return getWParam();
	}

	NMHDR* DialogProcContext::getNotificationData() const {
#if _DEBUG
		assert(getMessage() == WM_NOTIFY);
#endif
		return (LPNMHDR)getLParam();
	}

	NMLVDISPINFOA* DialogProcContext::getNotificationListViewDisplayInfo() const {
		return (LPNMLVDISPINFOA)getNotificationData();
	}

	NMLVCUSTOMDRAW* DialogProcContext::getNotificationCustomDraw() const {
		return (LPNMLVCUSTOMDRAW)getNotificationData();
	}

	NMITEMACTIVATE* DialogProcContext::getNotificationItemActivateData() const {
		return (LPNMITEMACTIVATE)getNotificationData();
	}

	NMTTDISPINFOA* DialogProcContext::getNotificationTooltipGetDisplayInfo() const {
		const auto data = getNotificationData();
#if _DEBUG
		assert(data && data->code == TTN_GETDISPINFO);
#endif
		return (LPNMTTDISPINFOA)data;
	}

	WORD DialogProcContext::getCommandNotificationCode() const {
#if _DEBUG
		assert(getMessage() == WM_COMMAND);
#endif
		return getHIWParam();
	}

	WORD DialogProcContext::getCommandControlIdentifier() const {
#if _DEBUG
		assert(getMessage() == WM_COMMAND);
#endif
		return getLOWParam();
	}

	MINMAXINFO* DialogProcContext::getMinMaxInfo() const {
#if _DEBUG
		assert(getMessage() == WM_GETMINMAXINFO);
#endif
		return (LPMINMAXINFO)getLParam();
	}

	WORD DialogProcContext::getSizeX() const {
#if _DEBUG
		assert(getMessage() == WM_SIZE);
#endif
		return getLOLParam();
	}

	WORD DialogProcContext::getSizeY() const {
#if _DEBUG
		assert(getMessage() == WM_SIZE);
#endif
		return getHILParam();
	}

	int DialogProcContext::getMovedX() const {
#if _DEBUG
		assert(getMessage() == WM_MOVE);
#endif
		return GET_X_LPARAM(getLParam());
	}

	int DialogProcContext::getMovedY() const {
#if _DEBUG
		assert(getMessage() == WM_MOVE);
#endif
		return GET_Y_LPARAM(getLParam());
	}

	int DialogProcContext::getMouseMoveX() const {
#if _DEBUG
		assert(getMessage() == WM_MOUSEMOVE);
#endif
		return GET_X_LPARAM(getLParam());
	}

	int DialogProcContext::getMouseMoveY() const {
#if _DEBUG
		assert(getMessage() == WM_MOUSEMOVE);
#endif
		return GET_Y_LPARAM(getLParam());
	}

	short DialogProcContext::getMouseWheelDelta() const {
#if _DEBUG
		assert(getMessage() == WM_MOUSEWHEEL);
#endif
		return GET_WHEEL_DELTA_WPARAM(getWParam());
	}

	WPARAM DialogProcContext::getKeyVirtualCode() const {
#if _DEBUG
		assert(getMessage() == WM_KEYDOWN || getMessage() == WM_KEYUP);
#endif
		return getLOWParam();
	}

	WORD DialogProcContext::getKeyFlags() const {
#if _DEBUG
		assert(getMessage() == WM_KEYDOWN || getMessage() == WM_KEYUP);
#endif
		return getHILParam();
	}

	WORD DialogProcContext::getKeyScanCode() const {
#if _DEBUG
		assert(getMessage() == WM_KEYDOWN || getMessage() == WM_KEYUP);
#endif
		WORD scanCode = LOBYTE(getKeyFlags());
		if (getKeyIsExtended()) {
			scanCode = MAKEWORD(scanCode, 0xE0);
		}
		return scanCode;
	}

	bool DialogProcContext::getKeyIsExtended() const {
#if _DEBUG
		assert(getMessage() == WM_KEYDOWN || getMessage() == WM_KEYUP);
#endif
		return (getKeyFlags() & KF_EXTENDED) == KF_EXTENDED;
	}

	bool DialogProcContext::getKeyWasDown() const {
#if _DEBUG
		assert(getMessage() == WM_KEYDOWN || getMessage() == WM_KEYUP);
#endif
		return (getKeyFlags() & KF_REPEAT) == KF_REPEAT;
	}

	bool DialogProcContext::getKeyIsReleased() const {
#if _DEBUG
		assert(getMessage() == WM_KEYDOWN || getMessage() == WM_KEYUP);
#endif
		return (getKeyFlags() & KF_UP) == KF_UP;;
	}

	WORD DialogProcContext::getKeyRepeatCount() const {
#if _DEBUG
		assert(getMessage() == WM_KEYDOWN || getMessage() == WM_KEYUP);
#endif
		return getLOLParam();
	}
}
