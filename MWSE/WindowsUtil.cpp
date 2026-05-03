#include "WindowsUtil.h"

#include "TES3Game.h"

#include "LuaUtil.h"

namespace mwse::lua {
	int64_t performanceFrequency = -1;
	int64_t startTimestamp = 0;

	double getHighPrecisionClock() {
		LARGE_INTEGER endingTime;
		QueryPerformanceCounter(&endingTime);
		int64_t elapsed = endingTime.QuadPart - startTimestamp;
		// Conversion to microseconds, we do this first to avoid loss-of-precision during the next division
		elapsed *= 1000000ll;
		double t = static_cast<double>(elapsed / performanceFrequency);
		t /= 1000000;
		return t;
	}

	size_t getVirtualMemoryUsage() {
		PROCESS_MEMORY_COUNTERS_EX memCounter;
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&memCounter, sizeof(memCounter));
		return memCounter.PrivateUsage;
	}

	std::optional<std::string> getClipboardText() {
		if (!IsClipboardFormatAvailable(CF_TEXT)) {
			return {};
		}

		if (!OpenClipboard(TES3::Game::get()->windowHandle)) {
			return {};
		}

		auto clipboardHandle = GetClipboardData(CF_TEXT);
		if (clipboardHandle == nullptr) {
			CloseClipboard();
			return {};
		}

		const char* clipboardText = static_cast<const char*>(GlobalLock(clipboardHandle));
		if (clipboardText == nullptr) {
			CloseClipboard();
			return {};
		}

		auto result = std::move(std::string(clipboardText));

		GlobalUnlock(clipboardHandle);
		CloseClipboard();

		return std::move(result);
	}

	bool setClipboardText(std::optional<std::string> text) {
		if (!OpenClipboard(TES3::Game::get()->windowHandle)) {
			return false;
		}

		if (!EmptyClipboard()) {
			CloseClipboard();
			return false;
		}

		// Allow just clearing the text.
		if (!text) {
			CloseClipboard();
			return true;
		}

		const auto stringSize = text.value().length() + 1;
		auto clipBuffer = GlobalAlloc(GMEM_MOVEABLE, stringSize);
		if (clipBuffer == nullptr) {
			CloseClipboard();
			return false;
		}

		char* buffer = (char*)GlobalLock(clipBuffer);
		if (buffer == nullptr) {
			CloseClipboard();
			return false;
		}

		strcpy_s(buffer, stringSize, text.value().c_str());

		GlobalUnlock(clipBuffer);
		SetClipboardData(CF_TEXT, clipBuffer);
		CloseClipboard();

		return true;
	}

	// From https://stackoverflow.com/questions/291424/canonical-way-to-parse-the-command-line-into-arguments-in-plain-c-windows-api
	LPSTR* CommandLineToArgvA(LPSTR lpCmdLine, INT* pNumArgs) {
		int retval;
		retval = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, lpCmdLine, -1, NULL, 0);
		if (!SUCCEEDED(retval))
			return NULL;

		LPWSTR lpWideCharStr = (LPWSTR)malloc(retval * sizeof(WCHAR));
		if (lpWideCharStr == NULL)
			return NULL;

		retval = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, lpCmdLine, -1, lpWideCharStr, retval);
		if (!SUCCEEDED(retval))
		{
			free(lpWideCharStr);
			return NULL;
		}

		int numArgs;
		LPWSTR* args;
		args = CommandLineToArgvW(lpWideCharStr, &numArgs);
		free(lpWideCharStr);
		if (args == NULL)
			return NULL;

		int storage = numArgs * sizeof(LPSTR);
		for (int i = 0; i < numArgs; ++i)
		{
			BOOL lpUsedDefaultChar = FALSE;
			retval = WideCharToMultiByte(CP_ACP, 0, args[i], -1, NULL, 0, NULL, &lpUsedDefaultChar);
			if (!SUCCEEDED(retval))
			{
				LocalFree(args);
				return NULL;
			}

			storage += retval;
		}

		LPSTR* result = (LPSTR*)LocalAlloc(LMEM_FIXED, storage);
		if (result == NULL)
		{
			LocalFree(args);
			return NULL;
		}

		int bufLen = storage - numArgs * sizeof(LPSTR);
		LPSTR buffer = ((LPSTR)result) + numArgs * sizeof(LPSTR);
		for (int i = 0; i < numArgs; ++i)
		{
			assert(bufLen > 0);
			BOOL lpUsedDefaultChar = FALSE;
			retval = WideCharToMultiByte(CP_ACP, 0, args[i], -1, buffer, bufLen, NULL, &lpUsedDefaultChar);
			if (!SUCCEEDED(retval))
			{
				LocalFree(result);
				LocalFree(args);
				return NULL;
			}

			result[i] = buffer;
			buffer += retval;
			bufLen -= retval;
		}

		LocalFree(args);

		*pNumArgs = numArgs;
		return result;
	}

	sol::table getCommandLine(sol::this_state ts) {
		const auto commandLine = GetCommandLineA();
		int argc = 0;
		auto argv = CommandLineToArgvA(commandLine, &argc);
		if (argv == nullptr) {
			return sol::nil;
		}

		sol::state_view state = ts;
		sol::table results = state.create_table();
		for (auto i = 0; i < argc; ++i) {
			results[i + 1] = argv[i];
		}

		LocalFree(argv);

		return results;
	}

	sol::optional<std::shared_ptr<LuaExecutor>> createProcess(sol::table params) {
		auto command = getOptionalParam<const char*>(params, "command", nullptr);
		if (command == nullptr) {
			throw std::invalid_argument("Invalid 'command' parameter passed. It cannot be nil.");
		}

		auto executor = std::make_shared<LuaExecutor>(command);
		executor->start();
		if (!executor->isValid()) {
			return executor;
		}

		// Allow non-async calls.
		if (getOptionalParam<bool>(params, "async", true)) {
			executor->wait();
		}

		return executor;
	}

	void openURL(std::string_view url) {
		ShellExecute(0, 0, url.data(), 0, 0, SW_SHOW);
	}

	bool getIsVirtualKeyPressed(int VK_key) {
		return (GetAsyncKeyState(VK_key) & 0x8000) == 0x8000;
	}
}

namespace mwse::windows {
	using getThreadDescription_type = decltype(::GetThreadDescription);
	static std::optional<getThreadDescription_type*> _GetThreadDescription;
	using setThreadDescription_type = decltype(::SetThreadDescription);
	static std::optional<setThreadDescription_type*> _SetThreadDescription;

	std::optional<std::wstring> GetThreadDescription(HANDLE thread) {
		// Initialize handle to function.
		if (!_GetThreadDescription) {
			const auto kernelBase = GetModuleHandleA("KernelBase.dll");
			if (kernelBase) {
				_GetThreadDescription = (getThreadDescription_type*)GetProcAddress(kernelBase, "GetThreadDescription");
			}
			else {
				_GetThreadDescription = nullptr;
			}
		}

		const auto getThreadDescription = _GetThreadDescription.value_or(nullptr);
		if (getThreadDescription) {
			wchar_t* pThreadName = nullptr;
			if (!SUCCEEDED(getThreadDescription(GetCurrentThread(), &pThreadName))) {
				return {};
			}

			std::wstring name = pThreadName;
			LocalFree(pThreadName);

			return name;
		}

		return {};
	}

	bool SetThreadDescription(HANDLE thread, const std::wstring_view& description) {
		// Initialize handle to function.
		if (!_SetThreadDescription) {
			const auto kernelBase = GetModuleHandleA("KernelBase.dll");
			if (kernelBase) {
				_SetThreadDescription = (setThreadDescription_type*)GetProcAddress(kernelBase, "SetThreadDescription");
			}
			else {
				_SetThreadDescription = nullptr;
			}
		}

		const auto setThreadDescription = _SetThreadDescription.value_or(nullptr);
		if (!setThreadDescription) {
			return false;
		}

		return SUCCEEDED(setThreadDescription(thread, description.data()));
	}

	//
	//
	//

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
