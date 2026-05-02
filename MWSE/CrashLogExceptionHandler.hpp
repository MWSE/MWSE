#pragma once
#include "CrashLogger.hpp"
#include "Log.h"

#include "TES3Defines.h"

#include "StringUtil.h"

#define SYMOPT_EX_WINE_NATIVE_MODULES 1000

constexpr UINT32 ce_printStackCount = 256;

namespace CrashLogger {
	// Set to true if you want extra logging to debug the logger itself.
	constexpr auto DEBUG_LOGGER = false;
}

namespace CrashLogger::PDB {
	extern std::string GetModule(UINT32 eip, HANDLE process) {
		IMAGEHLP_MODULE module = { 0 };
		module.SizeOfStruct = sizeof(IMAGEHLP_MODULE);
		if (!SymGetModuleInfo(process, eip, &module)) return "";

		return module.ModuleName;
	}

	inline extern UINT32 GetModuleBase(UINT32 eip, HANDLE process) {
		IMAGEHLP_MODULE module = { 0 };
		module.SizeOfStruct = sizeof(IMAGEHLP_MODULE);
		if (!SymGetModuleInfo(process, eip, &module)) return 0;

		return module.BaseOfImage;
	}

	inline extern std::string GetSymbol(UINT32 eip, HANDLE process) {
		char symbolBuffer[sizeof(SYMBOL_INFO) + 255];
		const auto symbol = (SYMBOL_INFO*)symbolBuffer;

		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		symbol->MaxNameLen = 254;
		DWORD64 offset = 0;

		if (!SymFromAddr(process, eip, &offset, symbol)) return "";

		const std::string functioName = symbol->Name;

		return fmt::format("{}+0x{:0X}", functioName, offset);
	}

	inline extern std::string GetLine(UINT32 eip, HANDLE process) {
		char lineBuffer[sizeof(IMAGEHLP_LINE) + 255];
		const auto line = (IMAGEHLP_LINE*)lineBuffer;
		line->SizeOfStruct = sizeof(IMAGEHLP_LINE);

		DWORD offset = 0;

		if (!SymGetLineFromAddr(process, eip, &offset, line)) return "";

		return fmt::format("{}:{:d}", line->FileName, line->LineNumber);
	}

	inline std::string& GetClassNameGetSymbol(void* object, std::string& buffer) {
		try {
			buffer = GetSymbol(*((UINT32*)object), GetCurrentProcess());
			return buffer;
		}
		catch (...) {
			return buffer;
		}
	}

	inline std::string& GetClassNameFromPDBSEH(void* object, std::string& buffer) {
		__try { GetClassNameGetSymbol(object, buffer); return buffer; }
		__except (EXCEPTION_EXECUTE_HANDLER) { return buffer; }
	}

	inline std::string GetClassNameFromPDB(void* object) {
		std::string name;
		GetClassNameFromPDBSEH(object, name);
		mwse::string::strip_start(name, "vtbl_");
		mwse::string::strip_start(name, "sg_");
		mwse::string::strip_end(name, "+0x0");
		return name;
	}

	struct RTTIType {
		void* typeInfo;
		UINT32 pad;
		char name[];
	};

	struct RTTILocator {
		UINT32		sig, offset, cdOffset;
		RTTIType* type;
	};

	inline const char* GetObjectClassNameInternal(void* objBase) {
		__try {
			const char* result = "";
			void** obj = (void**)objBase;
			RTTILocator** vtbl = (RTTILocator**)obj[0];
			RTTILocator* rtti = vtbl[-1];

			switch ((UINT32)vtbl) {
			case TES3::VirtualTableAddress::Activator:
				result = "Activator";
				break;
			case TES3::VirtualTableAddress::ActorAnimController:
				result = "ActorAnimController";
				break;
			case TES3::VirtualTableAddress::ActorWearsObjects:
				result = "ActorWearsObjects";
				break;
			case TES3::VirtualTableAddress::AIPackageActivate:
				result = "AIPackageActivate";
				break;
			case TES3::VirtualTableAddress::AIPackageBase:
				result = "AIPackageBase";
				break;
			case TES3::VirtualTableAddress::AIPackageEscort:
				result = "AIPackageEscort";
				break;
			case TES3::VirtualTableAddress::AIPackageFollow:
				result = "AIPackageFollow";
				break;
			case TES3::VirtualTableAddress::AIPackageTravel:
				result = "AIPackageTravel";
				break;
			case TES3::VirtualTableAddress::AIPackageWander:
				result = "AIPackageWander";
				break;
			case TES3::VirtualTableAddress::Alchemy:
				result = "Alchemy";
				break;
			case TES3::VirtualTableAddress::AnimatedObject:
				result = "AnimatedObject";
				break;
			case TES3::VirtualTableAddress::AnimationGroup:
				result = "AnimationGroup";
				break;
			case TES3::VirtualTableAddress::Apparatus:
				result = "Apparatus";
				break;
			case TES3::VirtualTableAddress::ArchiveFile:
				result = "ArchiveFile";
				break;
			case TES3::VirtualTableAddress::Armor:
				result = "Armor";
				break;
			case TES3::VirtualTableAddress::Attribute:
				result = "Attribute";
				break;
			case TES3::VirtualTableAddress::AttributeFatigue:
				result = "AttributeFatigue";
				break;
			case TES3::VirtualTableAddress::AttributeSkill:
				result = "AttributeSkill";
				break;
			case TES3::VirtualTableAddress::BaseObject:
				result = "BaseObject";
				break;
			case TES3::VirtualTableAddress::Birthsign:
				result = "Birthsign";
				break;
			case TES3::VirtualTableAddress::BodyPart:
				result = "BodyPart";
				break;
			case TES3::VirtualTableAddress::Book:
				result = "Book";
				break;
			case TES3::VirtualTableAddress::Cell:
				result = "Cell";
				break;
			case TES3::VirtualTableAddress::Class:
				result = "Class";
				break;
			case TES3::VirtualTableAddress::Clothing:
				result = "Clothing";
				break;
			case TES3::VirtualTableAddress::ContainerBase:
				result = "ContainerBase";
				break;
			case TES3::VirtualTableAddress::ContainerInstance:
				result = "ContainerInstance";
				break;
			case TES3::VirtualTableAddress::CreatureBase:
				result = "CreatureBase";
				break;
			case TES3::VirtualTableAddress::CreatureInstance:
				result = "CreatureInstance";
				break;
			case TES3::VirtualTableAddress::CutscenePlayer:
				result = "CutscenePlayer";
				break;
			case TES3::VirtualTableAddress::Dialogue:
				result = "Dialogue";
				break;
			case TES3::VirtualTableAddress::DialogueInfo:
				result = "DialogueInfo";
				break;
			case TES3::VirtualTableAddress::Door:
				result = "Door";
				break;
			case TES3::VirtualTableAddress::Enchantment:
				result = "Enchantment";
				break;
			case TES3::VirtualTableAddress::Faction:
				result = "Faction";
				break;
			case TES3::VirtualTableAddress::Game:
				result = "Game";
				break;
			case TES3::VirtualTableAddress::GameBase:
				result = "GameBase";
				break;
			case TES3::VirtualTableAddress::GameSetting:
				result = "GameSetting";
				break;
			case TES3::VirtualTableAddress::GlobalVariable:
				result = "GlobalVariable";
				break;
			case TES3::VirtualTableAddress::Ingredient:
				result = "Ingredient";
				break;
			case TES3::VirtualTableAddress::Land:
				result = "Land";
				break;
			case TES3::VirtualTableAddress::LandTexture:
				result = "LandTexture";
				break;
			case TES3::VirtualTableAddress::LeveledCreature:
				result = "LeveledCreature";
				break;
			case TES3::VirtualTableAddress::LeveledItem:
				result = "LeveledItem";
				break;
			case TES3::VirtualTableAddress::Light:
				result = "Light";
				break;
			case TES3::VirtualTableAddress::Lockpick:
				result = "Lockpick";
				break;
			case TES3::VirtualTableAddress::MagicEffect:
				result = "MagicEffect";
				break;
			case TES3::VirtualTableAddress::MagicSourceInstance:
				result = "MagicSourceInstance";
				break;
			case TES3::VirtualTableAddress::Miscellaneous:
				result = "Miscellaneous";
				break;
			case TES3::VirtualTableAddress::MobileActor:
				result = "MobileActor";
				break;
			case TES3::VirtualTableAddress::MobileCreature:
				result = "MobileCreature";
				break;
			case TES3::VirtualTableAddress::MobileNPC:
				result = "MobileNPC";
				break;
			case TES3::VirtualTableAddress::MobileObject:
				result = "MobileObject";
				break;
			case TES3::VirtualTableAddress::MobilePlayer:
				result = "MobilePlayer";
				break;
			case TES3::VirtualTableAddress::MobileProjectile:
				result = "MobileProjectile";
				break;
			case TES3::VirtualTableAddress::NPCBase:
				result = "NPCBase";
				break;
			case TES3::VirtualTableAddress::NPCInstance:
				result = "NPCInstance";
				break;
			case TES3::VirtualTableAddress::Object:
				result = "Object";
				break;
			case TES3::VirtualTableAddress::PathGrid:
				result = "PathGrid";
				break;
			case TES3::VirtualTableAddress::PhysicalObject:
				result = "PhysicalObject";
				break;
			case TES3::VirtualTableAddress::PlayerAnimController:
				result = "PlayerAnimController";
				break;
			case TES3::VirtualTableAddress::Probe:
				result = "Probe";
				break;
			case TES3::VirtualTableAddress::Quest:
				result = "Quest";
				break;
			case TES3::VirtualTableAddress::Race:
				result = "Race";
				break;
			case TES3::VirtualTableAddress::Reference:
				result = "Reference";
				break;
			case TES3::VirtualTableAddress::Region:
				result = "Region";
				break;
			case TES3::VirtualTableAddress::RepairTool:
				result = "RepairTool";
				break;
			case TES3::VirtualTableAddress::Script:
				result = "Script";
				break;
			case TES3::VirtualTableAddress::Skill:
				result = "Skill";
				break;
			case TES3::VirtualTableAddress::Sound:
				result = "Sound";
				break;
			case TES3::VirtualTableAddress::SoundGenerator:
				result = "SoundGenerator";
				break;
			case TES3::VirtualTableAddress::Spell:
				result = "Spell";
				break;
			case TES3::VirtualTableAddress::SpellProjectile:
				result = "SpellProjectile";
				break;
			case TES3::VirtualTableAddress::Static:
				result = "Static";
				break;
			case TES3::VirtualTableAddress::TArray:
				result = "TArray";
				break;
			case TES3::VirtualTableAddress::TES3Archive:
				result = "TES3Archive";
				break;
			case TES3::VirtualTableAddress::Weapon:
				result = "Weapon";
				break;
			case TES3::VirtualTableAddress::WeatherAshstorm:
				result = "WeatherAshstorm";
				break;
			case TES3::VirtualTableAddress::WeatherBase:
				result = "WeatherBase";
				break;
			case TES3::VirtualTableAddress::WeatherBlight:
				result = "WeatherBlight";
				break;
			case TES3::VirtualTableAddress::WeatherBlizzard:
				result = "WeatherBlizzard";
				break;
			case TES3::VirtualTableAddress::WeatherClear:
				result = "WeatherClear";
				break;
			case TES3::VirtualTableAddress::WeatherCloudy:
				result = "WeatherCloudy";
				break;
			case TES3::VirtualTableAddress::WeatherFog:
				result = "WeatherFog";
				break;
			case TES3::VirtualTableAddress::WeatherOvercast:
				result = "WeatherOvercast";
				break;
			case TES3::VirtualTableAddress::WeatherRain:
				result = "WeatherRain";
				break;
			case TES3::VirtualTableAddress::WeatherSnow:
				result = "WeatherSnow";
				break;
			case TES3::VirtualTableAddress::WeatherStorm:
				result = "WeatherStorm";
				break;
			default:
				break;
			}
			return result;
		}
		__except (ExceptionFilter(GetExceptionCode())) {
			return "";
		}
	}


	// use the RTTI information to return an object's class name
	inline const char* GetObjectClassNameInternal2(void* objBase) {
		__try {
			const char* result = "";
			void** obj = (void**)objBase;
			RTTILocator** vtbl = (RTTILocator**)obj[0];
			RTTILocator* rtti = vtbl[-1];
			RTTIType* type = rtti->type;

			// starts with .?AV
			if ((type->name[0] == '.') && (type->name[1] == '?')) {
				// is at most MAX_PATH chars long
				for (UINT32 i = 0; i < MAX_PATH; i++) if (type->name[i] == 0) {
					result = type->name;
					break;
				}
			}
			return result;
		}
		__except (ExceptionFilter(GetExceptionCode()))
		{
			return "";
		}
	}

	inline std::string GetClassNameFromRTTI(void* object) {
		return GetObjectClassNameInternal(object);
	}

	extern std::string GetClassNameFromRTTIorPDB(void* object) {
		if (const auto str = GetClassNameFromRTTI(object); !str.empty()) return str;
		return GetClassNameFromPDB(object);
		//if (const auto str = GetClassNameFromPDB(object); !str.contains("0x")) return str;
	}
};

namespace CrashLogger {
	inline void LogVersion(EXCEPTION_POINTERS* info) {
		__try {
			Version::Process(info);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			mwse::log::getLog() << ("Failed to log version. \n");
		}
	}

	inline void LogPlaytime(EXCEPTION_POINTERS* info) {
		__try {
			Playtime::Process(info);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			mwse::log::getLog() << ("Failed to log playtime. \n");
		}
	}

	inline void LogException(EXCEPTION_POINTERS* info) {
		__try {
			Exception::Process(info);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			mwse::log::getLog() << ("Failed to log exception. \n");
		}
	}

	inline void LogThread(EXCEPTION_POINTERS* info) {
		__try {
			Thread::Process(info);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			mwse::log::getLog() << ("Failed to log thread. \n");
		}
	}

	inline void LogCalltrace(EXCEPTION_POINTERS* info) {
		__try {
			Calltrace::Process(info);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			mwse::log::getLog() << ("Failed to log calltrace. \n");
		}
	}

	inline void LogMorrowindScriptState(EXCEPTION_POINTERS* info) {
		__try {
			MorrowindScript::Process(info);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			mwse::log::getLog() << ("Failed to log mwscript state. \n");
		}
	}

	inline void LogLuaTraceback(EXCEPTION_POINTERS* info) {
		__try {
			LuaTraceback::Process(info);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			mwse::log::getLog() << ("Failed to log lua traceback. \n");
		}
	}

	inline void LogRegistry(EXCEPTION_POINTERS* info) {
		__try {
			Registry::Process(info);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			mwse::log::getLog() << ("Failed to log registry. \n");
		}
	}

	inline void LogStack(EXCEPTION_POINTERS* info) {
		__try {
			Stack::Process(info);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			mwse::log::getLog() << ("Failed to log stack. \n");
		}
	}

	inline void LogMods(EXCEPTION_POINTERS* info) {
		__try {
			Mods::Process(info);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			mwse::log::getLog() << ("Failed to log mods. \n");
		}
	}

	inline void LogLuaMods(EXCEPTION_POINTERS* info) {
		__try {
			LuaMods::Process(info);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			mwse::log::getLog() << ("Failed to log mods. \n");
		}
	}

	inline void LogInstall(EXCEPTION_POINTERS* info) {
		__try {
			Install::Process(info);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			mwse::log::getLog() << ("Failed to log install. \n");
		}
	}

	inline void LogMemory(EXCEPTION_POINTERS* info) {
		__try {
			Memory::Process(info);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			mwse::log::getLog() << ("Failed to log memory. \n");
		}
	}

	inline void LogDevice(EXCEPTION_POINTERS* info) {
		__try {
			Device::Process(info);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			mwse::log::getLog() << ("Failed to log device. \n");
		}
	}

	inline void LogWarnings(EXCEPTION_POINTERS* info) {
		__try {
			Warnings::Process(info);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			mwse::log::getLog() << ("Failed to log warnings. \n");
		}
	}

	inline void Log(EXCEPTION_POINTERS* info) {
		auto& mwse_log = mwse::log::getLog();

		const auto begin = std::chrono::system_clock::now();
		//
		//mwse_log << ("Processing playtime \n");
		LogPlaytime(info);
		//mwse_log << ("Processing exception \n");
		LogVersion(info);
		LogException(info);
		//mwse_log << ("Processing thread \n");
		LogThread(info);
		//mwse_log << ("Processing memory \n");
		LogMemory(info);
		//mwse_log << ("Processing device");
		LogDevice(info);
		//mwse_log << ("Processing calltrace \n");
		LogCalltrace(info);
		LogLuaTraceback(info);
		//mwse_log << ("Processing registry \n");
		LogRegistry(info);
		//mwse_log << ("Processing stack \n");
		LogStack(info);
		LogMorrowindScriptState(info);
		//mwse_log << ("Processing mods);
		LogMods(info);
		LogLuaMods(info);
		//mwse_log << ("Processing install");
		LogInstall(info);
		//mwse_log << ("processing modules");
		//Modules::Process(info);
		//AssetTracker::Process(info);
		LogWarnings(info);

		const auto processing = std::chrono::system_clock::now();

		mwse_log << ("=== BASIC INFORMATION: =================================================================================================\n");
		mwse_log << ("%s", Version::Get().str().c_str());
		mwse_log << ("%s", Memory::Get().str().c_str());
		mwse_log << ("%s", Playtime::Get().str().c_str());
		mwse_log << ("%s", Thread::Get().str().c_str());
		mwse_log << ("%s", Exception::Get().str().c_str());
		mwse_log << ("%s", Install::Get().str().c_str());
		mwse_log << ("%s", Device::Get().str().c_str());
		mwse_log << ("=== CALL STACK: ========================================================================================================\n");
		mwse_log << ("%s", Calltrace::Get().str().c_str());
		mwse_log << ("=== LUA STACK: =========================================================================================================\n");
		mwse_log << ("%s", LuaTraceback::Get().str().c_str());
		mwse_log << ("=== REGISTRY: ==========================================================================================================\n");
		mwse_log << ("%s", Registry::Get().str().c_str());
		mwse_log << ("=== MWSCRIPT STATE: ====================================================================================================\n");
		mwse_log << ("%s", MorrowindScript::Get().str().c_str());
		mwse_log << ("=== STACK: =============================================================================================================\n");
		mwse_log << ("%s", Stack::Get().str().c_str());
		mwse_log << ("==== MODS: =============================================================================================================\n");
		mwse_log << ("%s", Mods::Get().str());
		mwse_log << ("==== LUA MODS: =========================================================================================================\n");
		mwse_log << ("%s", LuaMods::Get().str());
//		mwse_log << ("==== ASSETS: ===========================================================================================================\n");
//		mwse_log << ("%s", AssetTracker::Get().str());
//		mwse_log << ("==== MODULES: ==========================================================================================================\n");
//		mwse_log << ("%s", Modules::Get().str().c_str());
		mwse_log << ("=== WARNINGS: ==========================================================================================================\n");
		mwse_log << ("%s", Warnings::Get().str().c_str());

		if constexpr (CrashLogger::DEBUG_LOGGER) {
			const auto printing = std::chrono::system_clock::now();
			const auto timeProcessing = std::chrono::duration_cast<std::chrono::milliseconds>(processing - begin);
			const auto timePrinting = std::chrono::duration_cast<std::chrono::milliseconds>(printing - processing);
			mwse_log << ("=== LOGGING INFORMATION: ===============================================================================================\n");
			mwse_log << ("%s", fmt::format("Processed in {:d} ms, printed in {:d} ms", (long)timeProcessing.count(), (long)timePrinting.count()).c_str());
		}

		mwse::log::getLog().flush();

		SymCleanup(GetCurrentProcess());
	};

	inline void AttemptLog(EXCEPTION_POINTERS* info) {
		__try { Log(info); }
		__except (EXCEPTION_EXECUTE_HANDLER) {
			mwse::log::getLog() << "Failed to log exception info";
		};
	}

	static LPTOP_LEVEL_EXCEPTION_FILTER s_originalFilter = nullptr;

	inline LONG WINAPI Filter(EXCEPTION_POINTERS* info) {
		static bool caught = false;
		bool ignored = false;
		if (caught) ignored = true;
		else
		{
			caught = true;
			AttemptLog(info);
		}
		if (s_originalFilter) s_originalFilter(info); // don't return
		return !ignored ? EXCEPTION_CONTINUE_SEARCH : EXCEPTION_EXECUTE_HANDLER;
	};
}