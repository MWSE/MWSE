#pragma once

namespace se::crash {
	using LogProvider = std::ostream& (*)();
	using CrashCallback = void (*)(EXCEPTION_POINTERS*);
	using ExtraLogCallback = void (*)(EXCEPTION_POINTERS*, std::ostream&);
	using StringProvider = std::string (*)();

	struct Settings {
		std::string productName;
		std::string crashMessage;
		std::string miniDumpPath;
		std::string buildLabel;
		std::string buildDate;
		std::string warningsPath = "Warnings.txt";
		std::vector<std::wstring> dataSectionModules;
		LogProvider logProvider = nullptr;
		CrashCallback beforeCrashLog = nullptr;
		ExtraLogCallback extraLog = nullptr;
		StringProvider gpuProvider = nullptr;
		bool createFullMiniDump = false;
	};

	void configure(Settings settings);
	void initialize(Settings settings);
	void noteStartTime();
	void createMiniDump(EXCEPTION_POINTERS* info);
	void writeCrashLog(EXCEPTION_POINTERS* info);
	LONG handleException(EXCEPTION_POINTERS* info);
	bool installUnhandledExceptionFilter(Settings settings);
}
