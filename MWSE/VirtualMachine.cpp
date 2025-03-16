#include "VirtualMachine.h"
#include "InstructionStore.h"
#include "InstructionInterface.h"
#include "mwseString.h"
#include "mwOffsets.h"
#include "Log.h"
#include "TES3Util.h"
#include "StringUtil.h"
#include "Stack.h"

#include "TES3DataHandler.h"
#include "TES3GameFile.h"
#include "TES3GlobalVariable.h"
#include "TES3Game.h"
#include "TES3MobilePlayer.h"
#include "TES3Reference.h"
#include "TES3Script.h"
#include "TES3ItemData.h"
#include "TES3WorldController.h"

using namespace mwse;

VirtualMachine::VirtualMachine()
	: context(),
	oldscript(NULL)
{
	// Holds the current location in the script. when you read parameters from the script stream, you need to add the number of bytes read to this.
	mwScriptIP = reinterpret_cast<long*>(TES3_IP_IMAGE);
}

void VirtualMachine::loadParametersForOperation(OpCode::OpCode_t opcode, HookContext& context, TES3::Script* script)
{
	InstructionInterface_t* instruction = InstructionStore::getInstance().get(opcode);

	setHookContext(context);
	setScript(script);

	// Call OnScriptChange if we're running a different script.
	if (oldscript != NULL && oldscript != script) {
		OnScriptChange();
	}
	oldscript = script;

	instruction->loadParameters(*this);

	// Feed up any changes to the context to the hook.
	context = getHookContext();
}

float VirtualMachine::executeOperation(OpCode::OpCode_t opcode, HookContext& context, TES3::Script* script)
{
	InstructionInterface_t* instruction = InstructionStore::getInstance().get(opcode);

	setHookContext(context);
	setScript(script);

	float returnvalue = instruction->execute(*this);	//what else does 'execute' need?

	context = getHookContext();

	return returnvalue;
}

void VirtualMachine::OnScriptChange()
{
	// When a script changes, we can clear our stack.
	mwse::Stack::getInstance().clear();
}

bool VirtualMachine::isOpcode(const OpCode::OpCode_t opcode)
{
	return InstructionStore::getInstance().isOpcode(opcode);
}

void VirtualMachine::setHookContext(HookContext context)
{
	this->context = context;
}

HookContext VirtualMachine::getHookContext()
{
	return this->context;
}

void VirtualMachine::setScript(TES3::Script* script)
{
	this->script = script;
}

TES3::Script* VirtualMachine::getScript()
{
	return script;
}

TES3::BaseObject* VirtualMachine::getTemplate(const char* id)
{
	return TES3::DataHandler::get()->nonDynamicData->resolveObject(id);
}

TES3::Reference* VirtualMachine::getReference()
{
	return *reinterpret_cast<TES3::Reference**>(TES3_SCRIPTTARGETREF_IMAGE);
}

TES3::Reference* VirtualMachine::getReference(const char* id)
{
	bool isplayer = !_stricmp(id, "player") || !_stricmp(id, "playersavegame");
	if (isplayer) {
		return TES3::WorldController::get()->getMobilePlayer()->reference;
	}
	else
	{
		return TES3::DataHandler::get()->nonDynamicData->findFirstCloneOfActor(id);
	}
}

void VirtualMachine::setReference(TES3::Reference* reference)
{
	if (reference == NULL || reference->baseObject == NULL) {
#if _DEBUG
		mwse::log::getLog() << __FUNCTION__ << ": Attempted to set to invalid reference." << std::endl;
#endif
		return;
	}

	OpCode::OpCode_t opcode = OpCode::_SetReference;	//'->'
	unsigned char inref = 1;
	HookContext context = getHookContext();

	OpCode::OpCode_t* currentOpcode = reinterpret_cast<OpCode::OpCode_t*>(TES3_OP_IMAGE);
	*currentOpcode = opcode;

	TES3::Reference** currentReference = reinterpret_cast<TES3::Reference**>(TES3_SCRIPTTARGETREF_IMAGE);
	*currentReference = reference;

	void** currentTemplate = reinterpret_cast<void**>(TES3_SCRIPTTARGETTEMPL_IMAGE);
	*currentTemplate = reference->baseObject;

	unsigned char* currentInref = reinterpret_cast<unsigned char*>(context.ebp + 0x23);	//inref apparently
	*currentInref = inref;

	//this should be it. a lot of testing is needed of course ;)
}

TES3::Reference* VirtualMachine::getCurrentTarget()
{
	return TES3::Game::get()->playerTarget;
}

long VirtualMachine::getLongVariable(int index)
{
	TES3::Script* script = getScript();
	return script->getLongValue(index, true);
}

long VirtualMachine::getLongVariable(const char* id)
{
	unsigned int varIndex = 0;
	TES3::Script* script = getScript();
	char type = script->getLocalVarIndexAndType(id, &varIndex);
	if (!type) {
		return 0;
	}

	return script->getLongValue(varIndex, true);
}

long VirtualMachine::getLongVariable(int index, TES3::Reference& reference)
{
	auto attachment = reference.getAttachedItemData();
	if (attachment) {
		return attachment->scriptData->longVarValues[index];
	}

	return 0;
}

void VirtualMachine::setLongVariable(int index, long value)
{
	auto localVariables = *(reinterpret_cast<TES3::ScriptVariables**>(TES3_LOCALVARIABLES_IMAGE));
	if (index >= script->header.longCount) {
		return;
	}
	localVariables->longVarValues[index] = value;
}

void VirtualMachine::setLongVariable(const char* id, long value)
{
	unsigned int varIndex = 0;
	TES3::Script* script = getScript();
	char type = script->getLocalVarIndexAndType(id, &varIndex);
	if (type != 'l') {
#if _DEBUG
		mwse::log::getLog() << "Could not set value for variable '" << id << "'. Expected type 'l', got type '" << type << "'" << std::endl;
#endif
		return;
	}

	setLongVariable(varIndex, value);
}

void VirtualMachine::setLongVariable(int index, long value, TES3::Reference& reference)
{
	auto attachment = reference.getAttachedItemData();
	if (attachment == NULL) {
		return;
	}

	TES3::Script* script = attachment->script;
	TES3::ScriptVariables* localVariables = attachment->scriptData;
	if (index >= script->header.longCount) {
		return;
	}
	localVariables->longVarValues[index] = value;
}

short VirtualMachine::getShortVariable(int index)
{
	TES3::Script* script = getScript();
	return script->getShortValue(index, true);
}

short VirtualMachine::getShortVariable(const char* id)
{
	unsigned int varIndex = 0;
	TES3::Script* script = getScript();
	char type = script->getLocalVarIndexAndType(id, &varIndex);
	if (!type) {
		return 0;
	}

	return script->getShortValue(varIndex, true);
}

short VirtualMachine::getShortVariable(int index, TES3::Reference& reference)
{
	auto attachment = reference.getAttachedItemData();
	if (attachment) {
		return attachment->scriptData->shortVarValues[index];
	}

	return 0;
}

void VirtualMachine::setShortVariable(int index, short value)
{
	auto localVariables = *(reinterpret_cast<TES3::ScriptVariables**>(TES3_LOCALVARIABLES_IMAGE));
	if (index >= script->header.shortCount) {
		return;
	}
	localVariables->shortVarValues[index] = value;
}

void VirtualMachine::setShortVariable(const char* id, short value)
{
	unsigned int varIndex = 0;
	TES3::Script* script = getScript();
	char type = script->getLocalVarIndexAndType(id, &varIndex);
	if (type != 's') {
#if _DEBUG
		mwse::log::getLog() << "Could not set value for variable '" << id << "'. Expected type 's', got type '" << type << "'" << std::endl;
#endif
		return;
	}

	setShortVariable(varIndex, value);
}

void VirtualMachine::setShortVariable(int index, short value, TES3::Reference& reference)
{
	auto attachment = reference.getAttachedItemData();
	if (attachment == NULL) {
		return;
	}

	TES3::Script* script = attachment->script;
	TES3::ScriptVariables* localVariables = attachment->scriptData;
	if (index >= script->header.shortCount) {
		return;
	}
	localVariables->shortVarValues[index] = value;
}

float VirtualMachine::getFloatVariable(int index)
{
	TES3::Script* script = getScript();
	return script->getFloatValue(index, true);
}

float VirtualMachine::getFloatVariable(const char* id)
{
	unsigned int varIndex = 0;
	TES3::Script* script = getScript();
	char type = script->getLocalVarIndexAndType(id, &varIndex);
	if (!type) {
		return 0;
	}

	return script->getFloatValue(varIndex, true);
}

float VirtualMachine::getFloatVariable(int index, TES3::Reference& reference)
{
	auto attachment = reference.getAttachedItemData();
	if (attachment) {
		return attachment->scriptData->floatVarValues[index];
	}

	return 0;
}

void VirtualMachine::setFloatVariable(int index, float value)
{
	auto localVariables = *(reinterpret_cast<TES3::ScriptVariables**>(TES3_LOCALVARIABLES_IMAGE));
	if (index >= script->header.floatCount) {
		return;
	}
	localVariables->floatVarValues[index] = value;
}

void VirtualMachine::setFloatVariable(const char* id, float value)
{
	unsigned int varIndex = 0;
	TES3::Script* script = getScript();
	char type = script->getLocalVarIndexAndType(id, &varIndex);
	if (type != 'f') {
#if _DEBUG
		mwse::log::getLog() << "Could not set value for variable '" << id << "'. Expected type 'f', got type '" << type << "'" << std::endl;
#endif
		return;
	}

	setFloatVariable(varIndex, value);
}

void VirtualMachine::setFloatVariable(int index, float value, TES3::Reference& reference)
{
	auto attachment = reference.getAttachedItemData();
	if (attachment == NULL) {
		return;
	}

	TES3::Script* script = attachment->script;
	TES3::ScriptVariables* localVariables = attachment->scriptData;
	if (index >= script->header.floatCount) {
		return;
	}
	localVariables->floatVarValues[index] = value;
}

long VirtualMachine::getLongGlobal(const char* id)
{
	TES3::GlobalVariable* globalVar = TES3::DataHandler::get()->nonDynamicData->findGlobalVariable(id);
	if (globalVar == NULL) {
#if _DEBUG
		mwse::log::getLog() << "Could not find global variable '" << id << "'." << std::endl;
#endif
		return 0;
	}
	else if (globalVar->valueType != 'l') {
#if _DEBUG
		mwse::log::getLog() << "Could not set value for variable '" << id << "'. Expected type 'l', got type '" << globalVar->valueType << "'" << std::endl;
#endif
		return 0;
	}
	return static_cast<long>(globalVar->value);
}

void VirtualMachine::setLongGlobal(const char* id, long value)
{
	TES3::GlobalVariable* globalVar = TES3::DataHandler::get()->nonDynamicData->findGlobalVariable(id);
	if (globalVar == NULL) {
#if _DEBUG
		mwse::log::getLog() << "Could not find global variable '" << id << "'." << std::endl;
#endif
		return;
	}
	else if (globalVar->valueType != 'l') {
#if _DEBUG
		mwse::log::getLog() << "Could not set value for global variable '" << id << "'. Expected type 'l', got type '" << globalVar->valueType << "'" << std::endl;
#endif
		return;
	}

	globalVar->value = value;
}

short VirtualMachine::getShortGlobal(const char* id)
{
	TES3::GlobalVariable* globalVar = TES3::DataHandler::get()->nonDynamicData->findGlobalVariable(id);
	if (globalVar == NULL) {
#if _DEBUG
		mwse::log::getLog() << "Could not find global variable '" << id << "'." << std::endl;
#endif
		return 0;
	}
	else if (globalVar->valueType != 's') {
#if _DEBUG
		mwse::log::getLog() << "Could not set value for variable '" << id << "'. Expected type 's', got type '" << globalVar->valueType << "'" << std::endl;
#endif
		return 0;
	}
	return static_cast<short>(globalVar->value);
}

void VirtualMachine::setShortGlobal(const char* id, short value)
{
	TES3::GlobalVariable* globalVar = TES3::DataHandler::get()->nonDynamicData->findGlobalVariable(id);
	if (globalVar == NULL) {
#if _DEBUG
		mwse::log::getLog() << "Could not find global variable '" << id << "'." << std::endl;
#endif
		return;
	}
	else if (globalVar->valueType != 's') {
#if _DEBUG
		mwse::log::getLog() << "Could not set value for global variable '" << id << "'. Expected type 's', got type '" << globalVar->valueType << "'" << std::endl;
#endif
		return;
	}

	globalVar->value = value;
}

float VirtualMachine::getFloatGlobal(const char* id)
{
	TES3::GlobalVariable* globalVar = TES3::DataHandler::get()->nonDynamicData->findGlobalVariable(id);
	if (globalVar == NULL) {
#if _DEBUG
		mwse::log::getLog() << "Could not find global variable '" << id << "'." << std::endl;
#endif
		return 0;
	}
	else if (globalVar->valueType != 'f') {
#if _DEBUG
		mwse::log::getLog() << "Could not set value for variable '" << id << "'. Expected type 'f', got type '" << globalVar->valueType << "'" << std::endl;
#endif
		return 0;
	}
	return globalVar->value;
}

void VirtualMachine::setFloatGlobal(const char* id, float value)
{
	TES3::GlobalVariable* globalVar = TES3::DataHandler::get()->nonDynamicData->findGlobalVariable(id);
	if (globalVar == NULL) {
#if _DEBUG
		mwse::log::getLog() << "Could not find global variable '" << id << "'." << std::endl;
#endif
		return;
	}
	else if (globalVar->valueType != 'f') {
#if _DEBUG
		mwse::log::getLog() << "Could not set value for global variable '" << id << "'. Expected type 'f', got type '" << globalVar->valueType << "'" << std::endl;
#endif
		return;
	}

	globalVar->value = value;
}

char VirtualMachine::getByteValue(bool peek)
{
	int* scriptIP = reinterpret_cast<int*>(TES3_IP_IMAGE);

	void* scriptstream = script->machineCode;
	scriptstream = reinterpret_cast<void*>(reinterpret_cast<char*>(scriptstream) + *scriptIP);	//go to current position in script stream

	char returnData = *(reinterpret_cast<char*>(scriptstream));	//<-- change char here! (twice)

	if (!peek)
	{
		*scriptIP += sizeof(char);	//<-- change char here
	}

	return returnData;
}

short VirtualMachine::getShortValue(bool peek)
{
	int* scriptIP = reinterpret_cast<int*>(TES3_IP_IMAGE);

	void* scriptstream = script->machineCode;
	scriptstream = reinterpret_cast<void*>(reinterpret_cast<char*>(scriptstream) + *scriptIP);

	short returnData = *(reinterpret_cast<short*>(scriptstream));	//<-- change char here! (twice)

	if (!peek)
	{
		*scriptIP += sizeof(short);	//<-- change char here
	}

	return returnData;
}

long VirtualMachine::getLongValue(bool peek)
{
	int* scriptIP = reinterpret_cast<int*>(TES3_IP_IMAGE);

	void* scriptstream = script->machineCode;
	scriptstream = reinterpret_cast<void*>(reinterpret_cast<char*>(scriptstream) + *scriptIP);

	long returnData = *(reinterpret_cast<long*>(scriptstream));	//<-- change char here! (twice)

	if (!peek)
	{
		*scriptIP += sizeof(long);	//<-- change char here
	}

	return returnData;
}

float VirtualMachine::getFloatValue(bool peek)
{
	int* scriptIP = reinterpret_cast<int*>(TES3_IP_IMAGE);

	void* scriptstream = script->machineCode;
	scriptstream = reinterpret_cast<void*>(reinterpret_cast<char*>(scriptstream) + *scriptIP);

	float returnData = *(reinterpret_cast<float*>(scriptstream));	//<-- change char here! (twice)

	if (!peek)
	{
		*scriptIP += sizeof(float);	//<-- change char here
	}

	return returnData;
}

mwseString& VirtualMachine::getString(long fromStack)	//ask grant, need a '*' or a '&' here?? (and in the header files)
{
	// Invalid ID. Return an empty string.
	if (fromStack == 0x0)
	{
		return mwse::string::store::create("");
	}

	// Small enough to fit in the script as a literal string. Parse it from the script
	// stream.
	else if (fromStack < 32767)
	{
		void* scriptstream = script->machineCode;
		scriptstream = reinterpret_cast<void*>(reinterpret_cast<char*>(scriptstream) + fromStack);

		char blen = *(reinterpret_cast<char*>(scriptstream));

		long strlen = static_cast<long>(blen);

		scriptstream = reinterpret_cast<void*>(reinterpret_cast<char*>(scriptstream) + sizeof(blen));

		char* string = reinterpret_cast<char*>(scriptstream);

		return mwse::string::store::getOrCreate(string, strlen);
	}

	// If it's not in the script, it might be in storage.
	else if (mwse::string::store::exists(fromStack))
	{
		return mwse::string::store::get(fromStack);
	}

	// If it's not in storage, but is probably not a char*, return an empty string and log a message.
	else if (fromStack < 0x3F0000) {
		mwse::log::getLog() << "ERROR: Script '" << script->getObjectID() << "' in game file '" << (script->sourceMod != nullptr ? script->sourceMod->filename : "{N/A}") << "' contained garbage string reference! String references cannot be stored across saves." << std::endl;
		return mwse::string::store::create("");
	}

	// Otherwise, assume it's a char*, though we should never hit this case.
	else
	{
		const char* string = reinterpret_cast<char*>(fromStack);
		return mwse::string::store::getOrCreate(string);
	}
}

void VirtualMachine::dumpScriptVariables()
{
	if (!script) {
		mwse::log::getLog() << __FUNCTION__ << " - No script found." << std::endl;
		return;
	}

	mwse::log::getLog() << __FUNCTION__ << " - Variable dump for '" << script->header.name << "'" << std::endl;

	mwse::log::getLog() << "  Longs (" << script->header.longCount << "):" << std::endl;
	for (int i = 0; i < script->header.longCount; ++i) {
		mwse::log::getLog() << "    " << script->longVarNamePointers[i] << " = " << std::dec << script->varValues.longVarValues[i] << std::endl;
	}

	mwse::log::getLog() << "  Floats (" << script->header.floatCount << "):" << std::endl;
	for (int i = 0; i < script->header.floatCount; ++i) {
		mwse::log::getLog() << "    " << script->floatVarNamePointers[i] << " = " << std::dec << script->varValues.floatVarValues[i] << std::endl;
	}

	mwse::log::getLog() << "  Shorts (" << script->header.shortCount << "):" << std::endl;
	for (int i = 0; i < script->header.shortCount; ++i) {
		mwse::log::getLog() << "    " << script->shortVarNamePointers[i] << " = " << std::dec << script->varValues.shortVarValues[i] << std::endl;
	}
}

long* VirtualMachine::getScriptIP()
{
	return mwScriptIP;
}