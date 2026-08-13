#pragma once

#include "CrashLogUtilities.h"

#include "TES3Cell.h"
#include "TES3GameFile.h"
#include "TES3MobileObject.h"
#include "TES3Object.h"
#include "TES3Reference.h"
#include "TES3Script.h"
#include "TES3Weather.h"
#include "NINode.h"
#include "NIObjectNET.h"
#include "NIRTTI.h"

#include "TES3Land.h"

// If class is described by a single line, no need to name the variable
// If there is a member class, if it's one-line, leave it as one-line, if there are several, prepend the name and add offset

inline std::vector<std::string> LogClass(const TES3::BaseObject&);
inline std::vector<std::string> LogClass(const TES3::MobileObject&);
inline std::vector<std::string> LogClass(const TES3::PathGrid&);
inline std::vector<std::string> LogClass(const TES3::Weather&);
inline std::vector<std::string> LogClass(const NI::Object&);

template<class Member> auto LogMember(const std::string& name, const Member& member) {
	std::vector<std::string> vec = LogClass(member);
	if (vec.size() == 1) {
		return std::vector{ name + " " + vec[0] };
	}

	// Apply an offset for multi-line classes
	std::ranges::for_each(vec, [](std::string& s) {
		s.insert(0, "    ");
	});
	vec.insert(vec.begin(), name);
	vec.insert(vec.begin(), "\t \t \t \t \t ");
	return vec;
}

template<class Member> std::string LogClassLineByLine(const Member& member) {
	std::string output;
	std::vector<std::string> vec = LogClass(member);
	return fmt::format("{}", fmt::join(vec, "\n                                  "));
}

inline std::vector<std::string> LogClass(const TES3::BaseObject& obj) {
	std::vector<std::string> vec;
	std::string objectID = obj.getObjectID();
	std::string objectName;
	TES3::GameFile* sourceFile = obj.sourceFile;
	if (!sourceFile) {
		if (!&obj) {
			objectName = fmt::format("No Source Mod: {} ({})", GetObjectTypeName(obj.objectType), "NULL");
		}
		else {
			objectName = fmt::format("No Source Mod: {}", GetObjectTypeName(obj.objectType));
		}
		vec.push_back(fmt::format("ID: {} ", objectName));
	}
	else {
		std::string modName = sourceFile->filename;
		vec.push_back(fmt::format("ID: {} ({}) : (Plugin: \"{}\")", objectID, GetObjectTypeName(obj.objectType), modName));
	}
	return vec;
}

inline auto LogClass(const TES3::Object& obj) {
	std::vector<std::string> vec = LogClass(static_cast<const TES3::BaseObject&>(obj));
	std::string objectID = obj.getObjectID();
	std::string objectName;
	TES3::GameFile* sourceFile = obj.sourceFile;
	if (!sourceFile) {
		if (!&obj) {
			objectName = fmt::format("No Source Mod: {} ({})", GetObjectTypeName(obj.objectType), "NULL");
		}
		else {
			objectName = fmt::format("No Source Mod: {}", GetObjectTypeName(obj.objectType));
		}
		vec.push_back(fmt::format("ID: {} ({})", objectID, objectName));
	}
	else {
		std::string modName = sourceFile->filename;

		vec.push_back(fmt::format("ID: {} ({}) : (Plugin: \"{}\")", objectID, GetObjectTypeName(obj.objectType), modName));
	}
	if (const auto baseObject = obj.getBaseObject()) {
			std::vector<std::string> baseVector = LogMember("BaseObject:", *baseObject);
		vec.insert(vec.end(), baseVector.begin(), baseVector.end());
	}
	return vec;
}

inline auto LogClass(const TES3::Reference& obj) {
	std::vector<std::string> vec;
	//std::vector<std::string> vec = LogClass(static_cast<TES3::BaseObject&>(obj));
	std::string objectID = obj.getObjectID();
	std::string objectName;
	TES3::GameFile* sourceFile = obj.sourceFile;
	if (!sourceFile) {
		if (!&obj) {
			objectName = fmt::format("No Source Mod: {} ({})", "NULL", "NULL");
		}
		else {
			objectName = fmt::format("No Source Mod.");
		}
		vec.push_back(fmt::format("ID: {} ({})", objectID, objectName));
	}
	else {
		std::string modName = sourceFile->filename;

		vec.push_back(fmt::format("ID: ({}) : (Plugin: \"{}\")", objectID, modName));
	}
	if (const auto baseObject = obj.getBaseObject()) {
		std::vector<std::string> baseVector = LogMember("BaseObject:", *baseObject);
		vec.insert(vec.end(), baseVector.begin(), baseVector.end());
	}
	return vec;
}

inline std::vector<std::string> LogClass(const TES3::MobileObject& obj) {
	auto vec = LogClass(static_cast<TES3::Reference&>(*obj.reference));
	return vec;
}

inline std::vector<std::string> LogClass(const TES3::PathGrid& obj) {
	auto vec = LogClass(static_cast<const TES3::BaseObject&>(obj));
	if (obj.parentCell) {
		std::vector<std::string> baseVector = LogMember("Cell:", static_cast<TES3::BaseObject&>(*obj.parentCell));
		vec.insert(vec.end(), baseVector.begin(), baseVector.end());
	}
	return vec;
}

inline auto LogClass(const TES3::Cell& obj) {
	std::vector<std::string> vec;
	const std::string objectID = obj.getEditorName();
	const std::string sourceFile = obj.sourceFile ? obj.sourceFile->getFilename() : "N/A";
	vec.push_back(fmt::format("ID: {} : (Plugin: \"{}\")", objectID, sourceFile));
	return vec;
}

inline auto LogClass(const TES3::Land& obj) {
	std::vector<std::string> vec;
	const std::string objectID = fmt::format("({}, {})", obj.gridX, obj.gridY);
	const std::string sourceFile = obj.sourceFile ? obj.sourceFile->getFilename() : "N/A";
	vec.push_back(fmt::format("ID: {} : (Plugin: \"{}\")", objectID, sourceFile));
	return vec;
}

inline std::vector<std::string> LogClass(const TES3::Weather& obj) {
	std::vector<std::string> vec;
	std::string name = obj.getName();
	vec.push_back(fmt::format("ID: {}", name));
	return vec;
}

inline std::vector<std::string> LogClass(const NI::Object& obj) {
	std::vector<std::string> vec;

	const auto rtti = obj.getRunTimeTypeInformation();
	if (!rtti) {
		vec.push_back("No RTTI available");
		return vec;
	}

	std::optional<std::string> name;
	if (obj.isInstanceOfType(NI::RTTIStaticPtr::NiObjectNET)) {
		const auto n = static_cast<const NI::ObjectNET&>(obj).getName();
		if (n) {
			vec.push_back(fmt::format("Name: {}", n));
		}
	}

	return vec;
}

//inline std::vector<std::string> LogClass(const ActorMover& obj) { if (obj.pkActor) return LogClass(*obj.pkActor); return {}; }
//inline std::vector<std::string> LogClass(const QueuedReference& obj) { if (obj.refr) return LogClass(*obj.refr); return {}; }

/*

inline std::vector<std::string>  LogClass(const BaseProcess& obj)
{
	for (const auto iter : *TESForm::GetAll())
		if ((iter->eTypeID == TESForm::kType_Creature || iter->eTypeID == TESForm::kType_Character)
			&& reinterpret_cast<Actor*>(iter)->pkBaseProcess == &obj)
			return LogClass(reinterpret_cast<const TESObjectREFR&>(*iter));
	return {};
}

inline auto LogClass(const NiControllerSequence& obj)
{
	return std::vector{
		SanitizeString(std::string("Name: ") + std::string(obj.m_kName.m_kHandle)),
		SanitizeString(std::string("RootName: ") + std::string(obj.m_kAccumRootName.m_kHandle))
	};
}

inline auto LogClass(const BSAnimGroupSequence& obj)
{
	auto vec = LogClass(static_cast<const NiControllerSequence&>(obj));
	vec.push_back(fmt::format("AnimGroup: {:04X}", obj.animGroup->animGroup));
	return vec;
}


inline std::vector<std::string> LogClass(const AnimSequenceSingle& obj) { if (obj.pkAnim) return LogClass(*obj.pkAnim); return {}; }

inline std::vector<std::string> LogClass(const AnimSequenceMultiple& obj)
{
	std::vector<std::string> vec;
	UINT32 i = 0;
	for (const auto iter : *obj.pkAnims)
	{
		i++;
		vec.append_range(LogMember(fmt::format("AnimSequence{}", i), *iter));
	}
	return vec;
}

inline std::vector<std::string> LogClass(const NiExtraData& obj)
{
	if (const auto name = obj.m_kName.GetStd(); !name.empty())
		return std::vector{ '"' + SanitizeString(name.c_str()) + '"' };
	return {};
} */

/*inline std::vector<std::string> LogClass(NiExtraData& obj)
{
	if (const auto name = obj.m_pcName; name)
		return std::vector{ '"' + SanitizeString(name) + '"' };
	return {};
}

inline std::vector<std::string> LogClass(NiObjectNET& obj)
{
	const auto name = obj.m_pcName;
	if (name)
		return std::vector{ '"' + SanitizeString(name) + '"' };
	return {};
} */

inline std::vector<std::string> LogClass(const NI::ObjectNET& obj) {
	const auto name = obj.name;
	if (name)
		return std::vector{ '"' + SanitizeString(name) + '"' };
	return {};
}

inline std::vector<std::string> LogClass(const NI::Node& obj) {
	std::vector<std::string> vec;
	if (const auto name = obj.name)
		vec = LogMember("Name: ", static_cast<const NI::ObjectNET&>(obj));
	//if (const auto ref = TESObjectREFR::FindReferenceFor3D(&obj))
		//vec.append_range(LogMember("Reference:", *ref));
	return vec;
}

/*inline std::vector<std::string> LogClass(NI::TriStrips& obj)
{
	std::vector<std::string> vec;
	if (const auto name = obj.m_pcName)
		vec = LogMember("Name: ", static_cast<NiObjectNET&>(obj));
	//if (const auto ref = TESObjectREFR::FindReferenceFor3D(&obj))
		//vec.append_range(LogMember("Reference:", *ref));
	return vec;
} */

//inline std::vector<std::string> LogClass(const BSFile& obj) { return std::vector{ '"' + SanitizeString(obj.m_path) + '"' }; }
//inline std::vector<std::string> LogClass(const TESModel& obj) { return std::vector{ '"' + SanitizeString(obj.nifPath.m_data) + '"' }; }


/*inline std::vector<std::string> LogClass(const QueuedModel& obj)
{
	std::vector<std::string> vec;
	if (obj.filePath)
		vec.push_back(std::string("Path: ") + '"' + SanitizeString(obj.filePath) + '"');
	if (obj.model)
		vec.append_range(LogMember("Model:", *obj.model));
	return vec;
} */

//inline std::vector<std::string> LogClass(const TESTexture& obj) { return std::vector{ '"' + SanitizeString(obj.ddsPath.m_data) + '"' }; }
//inline std::vector<std::string> LogClass(const QueuedTexture& obj) { return std::vector{ '"' + SanitizeString(obj.pFileName) + '"' }; }
//inline std::vector<std::string> LogClass(const NiStream& obj) { return std::vector{ '"' + SanitizeString(obj.m_acFileName) + '"' }; }
//inline std::vector<std::string> LogClass(const ActiveEffect& obj) { if (obj.enchantObject) return LogClass(*obj.enchantObject); return {}; }


inline std::vector<std::string> LogClass(const TES3::Script& obj) {
	std::vector<std::string> vec = LogClass(static_cast<const TES3::BaseObject&>(obj));
	std::string objectID = obj.getObjectID();
	std::string objectName;
	TES3::GameFile* sourceFile = obj.sourceFile;
	std::string modName = sourceFile->filename;
	vec.push_back(fmt::format("\t \t \t \t \t ID: {} ({}) : (Plugin: \"{}\")", objectID, objectName, modName));
	if (const auto baseObject = obj.getBaseObject()) {
		std::vector<std::string> baseVector = LogMember("\t \t \t \t \t BaseObject:", *baseObject);
		vec.insert(vec.end(), baseVector.begin(), baseVector.end());
	}
	return vec;
}

/*inline std::vector<std::string> LogClass(const ScriptEffect& obj)
{
	auto vec = LogClass(static_cast<const ActiveEffect&>(obj));
	if (obj.data)
		vec.append_range(LogMember("Script:", *obj.data));
	return vec;
} */

//inline std::vector<std::string> LogClass(const QueuedKF& obj) { if (obj.kf) return std::vector{ '"' + SanitizeString(obj.kf->path) + '"' }; return {}; }
//inline std::vector<std::string> LogClass(const bhkRefObject& obj) { if (const auto object = obj.hkObj) return LogClass(*object); return {}; }

/*
inline std::vector<std::string> LogClass(const NiCollisionObject& obj)
{
	if (const auto object = obj.m_pkSceneObject) {
		if (object->IsNiNode())
			return LogClass(reinterpret_cast<const NiNode&>(*object));
		return LogClass(*object);
	}
	return {};
}

inline std::vector<std::string> LogClass(const NiTimeController& obj)
{
	if (const auto object = obj.m_pkTarget) {
		if (object->IsNiNode())
			return LogMember("Target:", reinterpret_cast<const NiNode&>(*object));
		return LogMember("Target:", *object);
	}
	return {};
}

inline std::vector<std::string> LogClass(const bhkCharacterController& obj)
{
	if (const auto object = obj.GetNiObject()) {
		if (object->IsNiNode())
			return LogMember("Target:", reinterpret_cast<const NiNode&>(*object));
		return LogMember("Target:", *object);
	}
	return {};
}

inline std::vector<std::string> LogClass(const hkpWorldObject& obj)
{
	std::vector<std::string> vec;
	std::string name = obj.GetName();

	if (!name.empty())
		vec.push_back(fmt::format("Name: {}", name));

	bhkNiCollisionObject* object = bhkUtilFunctions::GetbhkNiCollisionObject(&obj);
	if (object)
		vec.append_range(LogMember("Collision Object:", reinterpret_cast<const NiCollisionObject&>(*object)));

	return vec;
}

inline std::vector<std::string> LogClass(const IMemoryHeap& obj)
{
	HeapStats stats;
	std::string name = obj.GetName();
	obj.GetHeapStats(&stats, true);
	UINT32 total = stats.uiMemHeapSize;
	UINT32 free = stats.uiMemFreeInBlocks;
	UINT32 used = stats.uiMemUsedInBlocks;
	float percentage = ConvertToMiB(used) / ConvertToMiB(total) * 100.0f;
	std::string str = fmt::format("{}: {:10}/{:10} ({:.2f}%)", name.c_str(), FormatSize(used), FormatSize(total), percentage);

	return std::vector{ str };
} */
