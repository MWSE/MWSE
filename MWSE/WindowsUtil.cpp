#include "WindowsUtil.h"

#include "TES3Game.h"

namespace mwse {
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

	bool getIsVirtualKeyPressed(int VK_key) {
		return (GetAsyncKeyState(VK_key) & 0x8000) == 0x8000;
	}
}
