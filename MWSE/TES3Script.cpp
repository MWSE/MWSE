#include "TES3Script.h"

#include "LuaManager.h"
#include "LuaScriptExecuteEvent.h"
#include "LuaScriptExecutedEvent.h"
#include "TES3ScriptLua.h"

#include "TES3GameFile.h"
#include "TES3ItemData.h"
#include "TES3Reference.h"
#include "TES3WorldController.h"
#include "TES3UIMenuController.h"
#include "TES3ScriptCompiler.h"
#include "TES3Util.h"

#include "StringUtil.h"

namespace TES3 {
	//
	// TES3::GlobalScript
	//

	std::shared_ptr<mwse::lua::ScriptContext> GlobalScript::createContext() const {
		TES3::ItemData* itemData = reference->getAttachedItemData();
		if (itemData) {
			return std::make_shared<mwse::lua::ScriptContext>(script, itemData->scriptData);
		}
		return nullptr;
	}

	//
	// TES3::Script
	//

	Script* Script::currentlyExecutingScript = nullptr;
	Reference* Script::currentlyExecutingScriptReference = nullptr;

	const auto TES3_Script_ctor = reinterpret_cast<Script*(__thiscall *)(Script*)>(0x4FD830);
	Script* Script::ctor() {
		auto* result = TES3_Script_ctor(this);
		if (result) {
			result->espFileOffset = 0;
		}
		return result;
	}

	const auto TES3_Script_loadRecordSpecific = reinterpret_cast<bool(__thiscall*)(Script*, TES3::GameFile*)>(0x4FF700);
	bool Script::loadRecordSpecific(GameFile* gameFile) {
		if (gameFile && !gameFile->getFlag20()) {
			espFileOffset = gameFile->fileOffsetNextForm;
		}
		return TES3_Script_loadRecordSpecific(this, gameFile);
	}

	bool Script::reloadScript() {
		if (!machineCode) {
			return true;
		}

		if (!sourceMod || !sourceMod->reopen(0, false)) {
			return false;
		}

		if (espFileOffset != 0) {
			if (!sourceMod->setFilePointer(static_cast<unsigned int>(espFileOffset))) {
				return false;
			}
		}

		const auto firstRecord = sourceMod->getFirstSubrecord();
		if (firstRecord != 'TPCS') {
			return false;
		}

		bool scriptFound = false;
		do {
			const auto chunkType = sourceMod->getNextSubrecord();
			switch (chunkType) {
			case 'DHCS':
			{
				ScriptHeader chunkData = {};
				if (!sourceMod->readChunkData(&chunkData, sizeof(chunkData))) {
					return false;
				}

				if (_stricmp(chunkData.name, getObjectID()) == 0) {
					scriptFound = true;
				}
			}
			break;

			case 'TDCS':
				if (scriptFound) {
					if (machineCode) {
						se::memory::_delete(machineCode);
					}

					const auto dataSize = static_cast<size_t>(sourceMod->currentChunkHeader.size + 4);
					machineCode = reinterpret_cast<unsigned char*>(se::memory::_new(dataSize));
					memset(machineCode, 0, dataSize);
					if (!sourceMod->readChunkData(machineCode, 0)) {
						return false;
					}

					resetEventsAndVars();
					return true;
				}
				break;
			}
		} while (sourceMod->hasMoreRecords());

		return false;
	}

	const auto TES3_Script_executeScriptOpCode = reinterpret_cast<float(__thiscall *)(Script*, int, char, BaseObject*)>(0x505770);
	float Script::executeScriptOpCode(unsigned int opCode, char charParam, BaseObject * objectParam) {
		return TES3_Script_executeScriptOpCode(this, opCode, charParam, objectParam);
	}

	const auto TES3_Script_getLocalVarIndexAndType = reinterpret_cast<char(__thiscall *)(Script*, const char*, unsigned int*)>(0x50E7B0);
	char Script::getLocalVarIndexAndType(const char* name, unsigned int* out_index) {
		return TES3_Script_getLocalVarIndexAndType(this, name, out_index);
	}

	sol::optional<unsigned int> Script::getShortVarIndex(const char* name) const {
		for (auto i = 0u; i < header.shortCount; ++i) {
			const char* varName = shortVarNamePointers[i];
			if (varName && _stricmp(name, varName) == 0) {
				return i;
			}
		}
		return {};
	}

	const auto TES3_Script_getShortValue = reinterpret_cast<short(__thiscall*)(Script*, unsigned int, bool)>(0x4FFB90);
	short Script::getShortValue(unsigned int index, bool useLocalVars) {
		return TES3_Script_getShortValue(this, index, useLocalVars);
	}

	const auto TES3_Script_getLongValue = reinterpret_cast<int(__thiscall*)(Script*, unsigned int, bool)>(0x4FFC00);
	int Script::getLongValue(unsigned int index, bool useLocalVars) {
		return TES3_Script_getLongValue(this, index, useLocalVars);
	}

	const auto TES3_Script_getFloatValue = reinterpret_cast<float(__thiscall*)(Script*, unsigned int, bool)>(0x4FFC70);
	float Script::getFloatValue(unsigned int index, bool useLocalVars) {
		return TES3_Script_getFloatValue(this, index, useLocalVars);
	}

	const auto TES3_Script_DoCommand = reinterpret_cast<void(__thiscall*)(Script*, ScriptCompiler *, const char*, int, Reference *, ScriptVariables *, DialogueInfo *, Dialogue *)>(0x50E5A0);
	void Script::doCommand(ScriptCompiler * compiler, const char* command, int source, Reference * reference, ScriptVariables * variables, DialogueInfo * info, Dialogue * dialogue) {
		TES3_Script_DoCommand(this, compiler, command, source, reference, variables, info, dialogue);
	}

	const auto TES3_Script_execute = reinterpret_cast<void(__thiscall*)(Script*, Reference*, ScriptVariables*, DialogueInfo*, Reference*)>(0x5028A0);
	void Script::execute(Reference* reference, ScriptVariables* data, DialogueInfo* info, Reference* reference2) {
		currentlyExecutingScript = this;
		currentlyExecutingScriptReference = reference;

		if (mwse::lua::event::ScriptExecuteEvent::getEventEnabled()) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::object response = stateHandle.triggerEvent(new mwse::lua::event::ScriptExecuteEvent(this, reference, data, info, reference2));
			if (response.get_type() == sol::type::table) {
				sol::table eventData = response;
				if (eventData.get_or("block", false)) {
					currentlyExecutingScript = nullptr;
					currentlyExecutingScriptReference = nullptr;
					return;
				}
			}
		}

		TES3_Script_execute(this, reference, data, info, reference2);

		if (mwse::lua::event::ScriptExecutedEvent::getEventEnabled()) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			stateHandle.triggerEvent(new mwse::lua::event::ScriptExecutedEvent(this, reference, data, info, reference2));
		}

		currentlyExecutingScript = nullptr;
		currentlyExecutingScriptReference = nullptr;
	}

	void Script::setToCompiledResult(ScriptHeader* header, void* byteCode, const char* varNames, bool resolveParams) {
		const auto TES3_Script_setToCompiledResult = reinterpret_cast<void(__thiscall*)(Script*, ScriptHeader*, void*, const char*, bool)>(0x4FDA40);
		TES3_Script_setToCompiledResult(this, header, byteCode, varNames, resolveParams);
	}

	void Script::resetEventsAndVars() {
		const auto TES3_Script_resetEventsAndVars = reinterpret_cast<void(__thiscall*)(Script*)>(0x4FF450);
		TES3_Script_resetEventsAndVars(this);
	}

	sol::table Script::getLocalVars_lua(sol::this_state ts, sol::optional<bool> useLocals) {
		if (header.shortCount == 0 && header.longCount == 0 && header.floatCount == 0) {
			return sol::nil;
		}

		sol::state_view state = ts;

		sol::table results = state.create_table();

		// Append any short variables.
		for (auto i = 0u; i < header.shortCount; ++i) {
			const char* varName = shortVarNamePointers[i];
			results[varName] = state.create_table_with("type", 's', "index", i, "value", getShortValue(i, useLocals.value_or(false)));
		}

		// Append any long variables.
		for (auto i = 0u; i < header.longCount; ++i) {
			const char* varName = longVarNamePointers[i];
			results[varName] = state.create_table_with("type", 'l', "index", i, "value", getLongValue(i, useLocals.value_or(false)));
		}

		// Append any float variables.
		for (auto i = 0u; i < header.floatCount; ++i) {
			const char* varName = floatVarNamePointers[i];
			results[varName] = state.create_table_with("type", 'f', "index", i, "value", getFloatValue(i, useLocals.value_or(false)));
		}

		return results;
	}

	sol::optional<std::string> Script::getScriptText() const {
		if (sourceMod == nullptr) {
			return {};
		}

		GameFile tempFile = GameFile(sourceMod->path, sourceMod->filename);
		tempFile.collectActiveMods();
		if (!tempFile.reopen()) {
			return {};
		}

		union RecordType {
			unsigned int asUnsignedInt;
			ObjectType::ObjectType asObjectType;
			char asChar[4];

			RecordType() {
				asUnsignedInt = 0;
			}

			RecordType(unsigned int v) {
				asUnsignedInt = v;
			}
		};

		do {
			RecordType recordType = tempFile.getFirstSubrecord();
			if (!recordType.asUnsignedInt) {
				break;
			}

			if (recordType.asObjectType != ObjectType::Script) {
				continue;
			}

			bool scriptFound = false;
			do {
				RecordType subrecordType = tempFile.getNextSubrecord();
				if (!subrecordType.asUnsignedInt) {
					break;
				}


				switch (subrecordType.asUnsignedInt) {
				case 'DHCS':
				{
					ScriptHeader chunkData = {};
					tempFile.readChunkData(&chunkData);
					if (se::string::iequal(chunkData.name, header.name)) {
						scriptFound = true;
					}
					break;
				}
				case 'XTCS':
				{
					if (scriptFound) {
						std::string result = {};
						result.resize(tempFile.currentChunkHeader.size);
						tempFile.readChunkData(result.data(), tempFile.currentChunkHeader.size);
						return std::move(result);
					}
					break;
				}
				}
			} while (tempFile.hasMoreRecords());
		} while (tempFile.nextRecord());

		return {};
	}

	std::span<BYTE> Script::getByteCode() const {
		return std::span<BYTE>(machineCode, header.dataSize);
	}

	static TES3::ScriptCompiler scriptRecompiler;

	bool Script::recompile(const char* text) {
		if (!scriptRecompiler.compile(text)) {
			return false;
		}

		setToCompiledResult(&scriptRecompiler.scriptHeader, scriptRecompiler.scriptBuffer, scriptRecompiler.scriptLineBuffer);

		return true;
	}

	std::shared_ptr<mwse::lua::ScriptContext> Script::createContext() {
		return std::make_shared<mwse::lua::ScriptContext>(this, &varValues);
	}

	unsigned int Script::getShortVariableCount() const {
		return header.shortCount;
	}

	unsigned int Script::getLongVariableCount() const {
		return header.longCount;
	}

	unsigned int Script::getFloatVariableCount() const {
		return header.floatCount;
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Script)
