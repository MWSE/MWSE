#pragma once

namespace mwse {
	size_t getVirtualMemoryUsage();

	std::optional<std::string> getClipboardText();
	bool setClipboardText(std::optional<std::string> text);

	bool getIsVirtualKeyPressed(int VK_key);
}
