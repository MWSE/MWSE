#pragma once

#include "TES3Defines.h"

inline int ExceptionFilter(unsigned int code) {
	return EXCEPTION_EXECUTE_HANDLER;
}

// Erases a string if it does not have a null-terminator or is longer than MAX_PATH.
inline std::string& SanitizeStringBySize(std::string& str) {
	if (str.length() >= MAX_PATH) {
		str.resize(MAX_PATH);
	}
	return str;
}

inline std::string& SanitizeStringFromBadData(std::string& str) {
	// Remove the first non-printable character, then anything after it.
	const auto first_nonprintable = std::remove_if(str.begin(), str.end(), [](char c) { return !std::isprint(c); });
	str.erase(first_nonprintable, str.end());

	// Replace any newlines/tabs with spaces.
	std::replace_if(str.begin(), str.end(), [](char c) { return std::isspace(c); }, ' ');

	return str;
}

inline std::string pcName;
inline std::string userName;

inline std::string& SanitizeStringFromUserInfo(std::string& str) {
	if (pcName.empty()) {
		TCHAR infoBuf[MAX_PATH];
		DWORD bufCharCount = MAX_PATH;
		if (GetComputerName(infoBuf, &bufCharCount)) pcName = infoBuf;
	}

	if (userName.empty()) {
		TCHAR infoBuf[MAX_PATH];
		DWORD bufCharCount = MAX_PATH;
		if (GetUserName(infoBuf, &bufCharCount)) userName = infoBuf;
	}

	if (pcName.size() > 1)
		while (str.find(pcName) != -1) str.replace(str.find(pcName), pcName.size(), pcName.size(), '*');

	if (userName.size() > 1)
		while (str.find(userName) != -1) str.replace(str.find(userName), userName.size(), userName.size(), '*');

	return str;
}

inline const std::string& SanitizeString(std::string& str) {
	SanitizeStringBySize(str);
	SanitizeStringFromBadData(str);
	SanitizeStringFromUserInfo(str);
	return str;
}

inline const std::string SanitizeString(const std::string& str) {
	std::string r = str;
	SanitizeString(r);
	return r;
}

inline float ConvertToKiB(const UINT64 size) {
	return (float)size / 1024.0f;
}

inline float ConvertToMiB(const UINT64 size) {
	return (float)size / 1024.0f / 1024.0f;
}

inline float ConvertToGiB(const UINT64 size) {
	return (float)size / 1024.0f / 1024.0f / 1024.0f;
}

inline std::string FormatSize(const UINT64 size) {
	std::string result;
	if (size < 1024) {
		result = fmt::format("{:>6d} B", size);
	}
	else if (size < 1024ull * 1024ull) {
		result = fmt::format("{:>6.2f} KiB", ConvertToKiB(size));
	}
	else if (size < 1024ull * 1024ull * 1024ull) {
		result = fmt::format("{:>6.2f} MiB", ConvertToMiB(size));
	}
	else {
		result = fmt::format("{:>6.2f} GiB", ConvertToGiB(size));
	}
	return result;
}

inline std::string GetMemoryUsageString(const UINT64 used, const UINT64 total) {
	const auto usedPercent = (float)used / total * 100.0f;
	return fmt::format("{:10} / {:10} ({:.2f}%)", FormatSize(used), FormatSize(total), usedPercent);
}

inline std::string GetErrorAsString(UINT32 errorMessageID) {
	if (errorMessageID == 0) return ""; //No error message has been recorded

	LPSTR messageBuffer = nullptr;

	//Ask Win32 to give us the string version of that message ID.
	//The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	//Copy the error message into a std::string.
	std::string message(messageBuffer, size);

	//Free the Win32's string's buffer.
	LocalFree(messageBuffer);

	return message;
}

inline std::string GetExceptionAsString(UINT32 exceptionMessageID) {
	switch (exceptionMessageID) {
	case EXCEPTION_ACCESS_VIOLATION:			return "EXCEPTION_ACCESS_VIOLATION";
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:		return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
	case EXCEPTION_BREAKPOINT:					return "EXCEPTION_BREAKPOINT >>";
	case EXCEPTION_DATATYPE_MISALIGNMENT:		return "EXCEPTION_DATATYPE_MISALIGNMENT >>";
	case EXCEPTION_FLT_DENORMAL_OPERAND:		return "EXCEPTION_FLT_DENORMAL_OPERAND";
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:			return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
	case EXCEPTION_FLT_INEXACT_RESULT:			return "EXCEPTION_FLT_INEXACT_RESULT";
	case EXCEPTION_FLT_INVALID_OPERATION:		return "EXCEPTION_FLT_INVALID_OPERATION";
	case EXCEPTION_FLT_OVERFLOW:				return "EXCEPTION_FLT_OVERFLOW";
	case EXCEPTION_FLT_STACK_CHECK:				return "EXCEPTION_FLT_STACK_CHECK";
	case EXCEPTION_FLT_UNDERFLOW:				return "EXCEPTION_FLT_UNDERFLOW";
	case EXCEPTION_ILLEGAL_INSTRUCTION:			return "EXCEPTION_ILLEGAL_INSTRUCTION";
	case EXCEPTION_IN_PAGE_ERROR:				return "EXCEPTION_IN_PAGE_ERROR";
	case EXCEPTION_INT_DIVIDE_BY_ZERO:			return "EXCEPTION_INT_DIVIDE_BY_ZERO";
	case EXCEPTION_INT_OVERFLOW:				return "EXCEPTION_INT_OVERFLOW";
	case EXCEPTION_INVALID_DISPOSITION:			return "EXCEPTION_INVALID_DISPOSITION";
	case EXCEPTION_NONCONTINUABLE_EXCEPTION:	return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
	case EXCEPTION_PRIV_INSTRUCTION:			return "EXCEPTION_PRIV_INSTRUCTION";
	case EXCEPTION_SINGLE_STEP:					return "EXCEPTION_SINGLE_STEP";
	case EXCEPTION_STACK_OVERFLOW:				return "EXCEPTION_STACK_OVERFLOW";
	default: return "UKNOWN_EXCEPTION";
	}
}

constexpr const char* GetObjectTypeName(TES3::ObjectType::ObjectType type) {
	using namespace TES3::ObjectType;
	switch (type) {
	case Activator: return "Activator";
	case Alchemy: return "Alchemy";
	case Ammo: return "Ammo";
	case AnimationGroup: return "AnimationGroup";
	case Apparatus: return "Apparatus";
	case Armor: return "Armor";
	case Birthsign: return "Birthsign";
	case Bodypart: return "Bodypart";
	case Book: return "Book";
	case Cell: return "Cell";
	case Class: return "Class";
	case Clothing: return "Clothing";
	case Container: return "Container";
	case Creature: return "Creature";
	case CreatureClone: return "CreatureClone";
	case Dialogue: return "Dialogue";
	case DialogueInfo: return "DialogueInfo";
	case Door: return "Door";
	case Enchantment: return "Enchantment";
	case Faction: return "Faction";
	case GameSetting: return "GameSetting";
	case Global: return "GlobalVariable";
	case Ingredient: return "Ingredient";
	case Land: return "Land";
	case LandTexture: return "LandTexture";
	case LeveledCreature: return "LeveledCreature";
	case LeveledItem: return "LeveledItem";
	case Light: return "Light";
	case Lockpick: return "Lockpick";
	case MagicEffect: return "MagicEffect";
	case MagicSourceInstance: return "MagicSourceInstance";
	case Misc: return "Miscellaneous";
	case MobileCreature: return "MobileCreature";
	case MobileNPC: return "MobileNPC";
	case MobileObject: return "MobileObject";
	case MobilePlayer: return "MobilePlayer";
	case MobileProjectile: return "MobileProjectile";
	case MobileSpellProjectile: return "MobileSpellProjectile";
	case NPC: return "NPC";
	case NPCClone: return "NPCClone";
	case PathGrid: return "PathGrid";
	case Probe: return "Probe";
	case Quest: return "Quest";
	case Race: return "Race";
	case Reference: return "Reference";
	case Region: return "Region";
	case Repair: return "RepairTool";
	case Script: return "Script";
	case Skill: return "Skill";
	case Sound: return "Sound";
	case SoundGenerator: return "SoundGenerator";
	case Spell: return "Spell";
	case Static: return "Static";
	case ObjectType::TES3: return "TES3";
	case Training: return "Training";
	case Weapon: return "Weapon";
	default: return nullptr;
	}
}
