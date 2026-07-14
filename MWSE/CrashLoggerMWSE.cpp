#include "CrashLogger.h"

#include "Log.h"
#include "LuaManager.h"
#include "LuaUtil.h"
#include "TES3DataHandler.h"
#include "TES3GameFile.h"
#include "TES3Object.h"
#include "TES3Reference.h"
#include "TES3Script.h"

#include "BuildDate.h"
#include "StringUtil.h"

namespace CrashLogger::Version {
	static std::stringstream output;

	void Process(EXCEPTION_POINTERS* info) {
		try {
#ifdef APPVEYOR_BUILD_NUMBER
			output << fmt::format("Appveyor Build: {}\n", APPVEYOR_BUILD_NUMBER);
#else
			output << "Appveyor Build: <unavailable>\n";
#endif
			output << fmt::format("Build Date: {}\n", MWSE_BUILD_DATE);
		}
		catch (...) {
			output << "Failed to log version.\n";
		}
	}

	std::stringstream& Get() {
		output.flush();
		return output;
	}
}

namespace CrashLogger::MorrowindScript {
	static std::stringstream output;

	static const char* SafeGetObjectId(const TES3::BaseObject* object) {
		__try {
			return object->getObjectID();
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			return nullptr;
		}
	}

	static const char* SafeGetSourceFile(const TES3::BaseObject* object) {
		__try {
			return object->getSourceFilename();
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			return nullptr;
		}
	}

	template <typename T>
	static void SafePrintObject(const char* title, const T* object) {
		if (!object) {
			output << "  " << title << ": nullptr\n";
			return;
		}

		const auto id = SafeGetObjectId(object);
		const auto source = SafeGetSourceFile(object);
		output << "  " << title << ": " << (id ? id : "<memory corrupted>") << " (" << (source ? source : "<memory corrupted>") << ")\n";
		if (id) {
			mwse::log::prettyDump(object, output);
		}
	}

	void Process(EXCEPTION_POINTERS* info) {
		try {
			if (TES3::Script::currentlyExecutingScript) {
				output << "Currently executing mwscript context:\n";
				SafePrintObject("Script", TES3::Script::currentlyExecutingScript);
				SafePrintObject("Reference", TES3::Script::currentlyExecutingScriptReference);
				output << "  OpCode: 0x" << std::hex << *reinterpret_cast<DWORD*>(0x7A91C4) << "\n";
				output << "  Cursor Offset: 0x" << std::hex << *reinterpret_cast<DWORD*>(0x7CEBB0) << "\n";
			}
			else {
				output << "No mwscript instance running.\n";
			}
		}
		catch (...) {
			output << "Failed to process mwscript state traceback.\n";
		}
	}

	std::stringstream& Get() {
		output.flush();
		return output;
	}
}

namespace CrashLogger::LuaTraceback {
	static std::stringstream output;

	void Process(EXCEPTION_POINTERS* info) {
		try {
			const auto stackTrace = mwse::lua::getStackTrace(true);
			if (!stackTrace.empty()) {
				output << stackTrace << '\n';
			}
		}
		catch (...) {
			output << "Failed to process lua traceback.\n";
		}
	}

	std::stringstream& Get() {
		output.flush();
		return output;
	}
}

namespace CrashLogger::Mods {
	static std::stringstream output;

	void Process(EXCEPTION_POINTERS* info) {
		try {
			const auto dataHandler = TES3::DataHandler::get();
			if (!dataHandler) return;

			const auto activeMods = dataHandler->nonDynamicData->getActiveMods();
			if (activeMods.empty()) return;

			const auto filenameLength = std::strlen((*std::max_element(activeMods.begin(), activeMods.end(), [](const auto& a, const auto& b) {
				return std::strlen(a->getFilename()) < std::strlen(b->getFilename());
			}))->getFilename());
			const auto authorLength = std::strlen((*std::max_element(activeMods.begin(), activeMods.end(), [](const auto& a, const auto& b) {
				return std::strlen(a->getAuthor()) < std::strlen(b->getAuthor());
			}))->getAuthor());

			output << fmt::format("{:<{}} | {:<{}} | {:<s}", "File", filenameLength, "Author", authorLength, "Size") << '\n';
			for (const auto gameFile : activeMods) {
				if (!gameFile) break;
				const auto filename = gameFile->getFilename();
				const auto author = se::string::trim_copy(gameFile->getAuthor());
				output << fmt::format("{:<{}} | {:<{}} | {} bytes\n", filename, filenameLength, author, authorLength, gameFile->getFileSize());
			}
		}
		catch (...) {
			output << "Failed to process mods.\n";
		}
	}

	std::stringstream& Get() {
		output.flush();
		return output;
	}
}

namespace CrashLogger::LuaMods {
	static std::stringstream output;

	struct LuaModResult {
		std::string key;
		std::string firstAuthor;
		std::string version;
	};

	void Process(EXCEPTION_POINTERS* info) {
		try {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			const auto& lua = stateHandle.getState();
			const sol::table luaMWSE = lua["mwse"];
			const sol::table luaRuntimes = luaMWSE["runtimes"];

			std::vector<LuaModResult> results;
			for (auto index = 1u; index <= luaRuntimes.size(); ++index) {
				const sol::table runtime = luaRuntimes[index];
				if (runtime.get_or("core_mod", false)) continue;

				const sol::optional<std::string> key = runtime["key"];
				const sol::optional<std::string> name = runtime["metadata"]["package"]["name"];
				sol::optional<std::vector<std::string>> authors = runtime["metadata"]["package"]["authors"];
				const sol::optional<std::string> version = runtime["metadata"]["package"]["version"];

				if (authors && authors.value().size() > 5) {
					authors.value().resize(5);
				}

				results.push_back({
					name ? fmt::format("{} ({})", name.value(), key.value_or("<invalid>")) : key.value_or("<invalid>"),
					authors ? fmt::format("{}", fmt::join(authors.value(), ", ")) : "",
					version.value_or(""),
				});
			}

			if (results.empty()) return;

			const auto keyLength = std::max_element(results.begin(), results.end(), [](const auto& a, const auto& b) {
				return a.key.length() < b.key.length();
			})->key.length();
			const auto authorLength = std::max_element(results.begin(), results.end(), [](const auto& a, const auto& b) {
				return a.firstAuthor.length() < b.firstAuthor.length();
			})->firstAuthor.length();

			output << fmt::format("{:<{}} | {:<{}} | {:<s}", "Mod", keyLength, "Author", authorLength, "Version") << '\n';
			for (const auto& mod : results) {
				output << fmt::format("{:<{}} | {:<{}} | {}\n", mod.key, keyLength, mod.firstAuthor, authorLength, mod.version);
			}
		}
		catch (...) {
			output << "Failed to process lua mods.\n";
		}
	}

	std::stringstream& Get() {
		output.flush();
		return output;
	}
}
