#pragma once

#include "LuaExecutor.h"

namespace mwse::lua {
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
}
