#pragma once

namespace mwse::log {
	void OpenLog(const char* path);
	void CloseLog();

	std::ostream& getLog();
	std::ostream& getDebug(); //outputs to OutputDebugString

	void prettyDump(const void* data, const size_t length);
	void prettyDump(const void* data, const size_t length, std::ostream& output);

	template<class T> void prettyDump(const T* data) {
		prettyDump(data, sizeof(T));
	};

	template<class T> void prettyDump(const T* data, std::ostream& output) {
		prettyDump(data, sizeof(T), output);
	};
}
