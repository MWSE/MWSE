#include "MgeTes3Machine.h"

#include "mwseString.h"

#include "mwseString.h"
#include "mwOffsets.h"
#include "VirtualMachine.h"
#include "StringUtil.h"

#include "TES3MobilePlayer.h"
#include "TES3WorldController.h"

TES3MACHINE::TES3MACHINE() :
	VIRTUALMACHINE(),
	scriptaddr(0),
	script(),
	flow(),
	instructionpointer(0),
	stackpointer(0)
{

}

//get the info from a processor register (EDX,EIP,...)
bool TES3MACHINE::GetRegister(WORD regidx, VMREGTYPE& value) {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}

	return true;
}

//write stuff into a processor register (EDX,EIP,...)
bool TES3MACHINE::SetRegister(WORD regidx, VMREGTYPE value) {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}

	return true;
}

bool TES3MACHINE::SetScript(VPSCRIPT pscript) {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}

	return true;
}

bool TES3MACHINE::Interrupt(VMINTERRUPT num) {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}

	return true;
}

//return the Flow context, (registers, ...)
const Context TES3MACHINE::GetFlow(void) {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}

	return flow;
}

//set the flow context (registers, ...)
void TES3MACHINE::SetFlow(const Context newflow) {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}

	flow = newflow;
}

bool TES3MACHINE::SetVMDebuggerBreakpoint(HWBREAKPOINT* breakpoint) {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}

	return true;
}

HWBREAKPOINT* TES3MACHINE::GetVMDebuggerBreakpoint() {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}

	return breakpoint;
}

const char* TES3MACHINE::GetScriptName(void) {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}

	return NULL;
}

const char* TES3MACHINE::GetString(VPVOID addr) {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}

	return mwse::mwAdapter::GetVMInstance()->getString((long)addr).c_str();
}


bool TES3MACHINE::dumpmem(VPVOID ptr, int size) {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}

	return true;
}

void TES3MACHINE::dumpscriptstack(void) {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}
}

void TES3MACHINE::dumpscript(void) {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}
}

void TES3MACHINE::dumptemplate(VPTEMPLATE ptempl) {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}
}

void TES3MACHINE::dumpobject(VPREFERENCE pref) {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}
}

void TES3MACHINE::dumpobjects(void) {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}
}

void TES3MACHINE::searchforscripttarget(void) {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}
}

void TES3MACHINE::CheckForSkillUp(long skill_id) {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}
	TES3::WorldController::get()->getMobilePlayer()->progressSkillLevelIfRequirementsMet(skill_id);
}

TES3::MobilePlayer* TES3MACHINE::GetMacpRecord() {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}

	return TES3::WorldController::get()->getMobilePlayer();
}

long TES3MACHINE::GetRandomLong(long min, long max) {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}

	std::uniform_int_distribution<long> dist(min, max);
	return dist(rng_);
}

float TES3MACHINE::GetRandomFloat(float min, float max) {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}

	std::uniform_real_distribution<float> dist(min, max);
	return dist(rng_);
}

long TES3MACHINE::CreateArray(std::string const& caller) {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}

	long id = 0;
	if (arrays_.size() < kMaxArrayId) {
		id = arrays_.size() + 1;
		arrays_.push_back(std::vector<long>());
	}
	else {
		if constexpr (DEBUG_MGE_VM) {
			mwse::log::getLog() << caller << ": Unable to create array. Maximum number of arrays reached. id: " << id << std::endl;
		}
	}

	return id;
}

long TES3MACHINE::GetArrayValue(std::string const& caller, long const id, long const index) {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}

	long value = 0;
	if (id > 0 && (unsigned long)id <= arrays_.size()) {
		std::vector<long> const& a = arrays_[id - 1];
		if (index >= 0 && (unsigned long)index < a.size()) {
			value = a[index];
		}
		else {
			if constexpr (DEBUG_MGE_VM) {
				mwse::log::getLog() << caller << "Array index out of bounds. id: " << id << " index: " << index << std::endl;
			}
		}
	}
	else {
		if constexpr (DEBUG_MGE_VM) {
			mwse::log::getLog() << caller << ": Invalid array id: " << id << std::endl;
		}
	}

	return value;
}

long TES3MACHINE::SetArrayValue(std::string const& caller, long const id, long const index, long const value) {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}

	long success = 0;
	if (id > 0 && (unsigned long)id <= arrays_.size()) {
		if (index >= 0) {
			std::vector<long>& a = arrays_[id - 1];
			if ((unsigned long)index + 1 > a.size()) {
				a.resize(index + 1);
			}
			a[index] = value;
			success = 1;
		}
		else {
			if constexpr (DEBUG_MGE_VM) {
				mwse::log::getLog() << caller << "Array index out of bounds. id: " << id << " index: " << index << std::endl;
			}
		}
	}
	else {
		if constexpr (DEBUG_MGE_VM) {
			mwse::log::getLog() << caller << ": Invalid array id: " << id << std::endl;
		}
	}

	return success;
}

long TES3MACHINE::GetArraySize(std::string const& caller, long const id) {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}

	long size = 0;
	if (id > 0 && (unsigned long)id <= arrays_.size()) {
		size = arrays_[id - 1].size();
	}
	else {
		if constexpr (DEBUG_MGE_VM) {
			mwse::log::getLog() << caller << ": Invalid array id: " << id << std::endl;
		}
	}

	return size;
}

long TES3MACHINE::ClearArray(std::string const& caller, long const id) {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}

	long success = 0;
	if (id > 0 && (unsigned long)id <= arrays_.size()) {
		arrays_[id - 1].clear();
		success = 1;
	}
	else {
		if constexpr (DEBUG_MGE_VM) {
			mwse::log::getLog() << caller << ": Invalid array id: " << id << std::endl;
		}
	}

	return success;
}

std::vector<std::vector<long> >& TES3MACHINE::arrays() {
	if constexpr (DEBUG_MGE_VM) {
		mwse::log::getLog() << __FUNCTION__ << std::endl;
	}
	return arrays_;
}