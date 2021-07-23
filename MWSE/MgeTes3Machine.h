#pragma once

#include "MgeVirtualMachine.h"
#include "Log.h"

#include "TES3Defines.h"

#include "MemoryUtil.h"

struct HWBREAKPOINT;
struct Context {};
typedef void* VPSCRIPT;
typedef void* TES3SCRIPT;
typedef void* VPREFERENCE;
typedef void* VPTEMPLATE;

typedef enum { INTSWITCHREFERENCE = 1 } INTERRUPTS;

struct TES3MACHINE : public VIRTUALMACHINE
{
	TES3MACHINE();

	virtual void searchforscripttarget(void);
	virtual void dumpscript(void);
	virtual void dumpscriptstack(void);
	virtual void dumpobjects(void);
	virtual void dumpobject(VPREFERENCE ref);
	virtual void dumptemplate(VPTEMPLATE ptempl);
	virtual bool dumpmem(VPVOID ptr, int size);

	virtual bool Interrupt(VMINTERRUPT num);
	virtual bool GetRegister(WORD regidx, VMREGTYPE& value);
	virtual bool SetRegister(WORD regidx, VMREGTYPE value);
	virtual const char* GetScriptName(void);
	virtual const char* TES3MACHINE::GetString(VPVOID addr);
	virtual bool SetVMDebuggerBreakpoint(HWBREAKPOINT* breakpoint);
	virtual HWBREAKPOINT* GetVMDebuggerBreakpoint();

	// 2005-02-15  CDC  needed for messagefix instruction
	virtual VPSCRIPT GetScript(void) {
#if DEBUG_MGE_VM
		mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
		return scriptaddr;
	}
	virtual bool SetScript(VPSCRIPT pscript);
	virtual const Context GetFlow(void);
	virtual void SetFlow(const Context newflow);

	void* Malloc(size_t size) {
#if DEBUG_MGE_VM
		mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
		return mwse::tes3::malloc(size);
	}
	void Free(void* to_free) {
#if DEBUG_MGE_VM
		mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
		mwse::tes3::free(to_free);
	}
	void* Realloc(void* to_realloc, size_t size) {
#if DEBUG_MGE_VM
		mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
		return mwse::tes3::realloc(to_realloc, size);
	}
	void CheckForSkillUp(long skill_id);
	TES3::MobilePlayer* GetMacpRecord();
	long GetRandomLong(long min, long max);
	float GetRandomFloat(float min, float max);
	long CreateArray(std::string const& caller);
	long GetArrayValue(std::string const& caller, long const id,
		long const index);
	long SetArrayValue(std::string const& caller, long const id,
		long const index, long const value);
	long GetArraySize(std::string const& caller, long const id);
	long ClearArray(std::string const& caller, long const id);
	std::vector<std::vector<long> >& arrays();

private:
	ADDRESSSPACE* executable;
	VPSCRIPT scriptaddr;
	TES3SCRIPT script;
	Context flow;
	HWBREAKPOINT* breakpoint;
	VMREGTYPE instructionpointer;
	VMREGTYPE stackpointer;
	VMREGTYPE generalregs[GPMAX + 1];
	std::mt19937 rng_;
	static long const kMaxArrayId = 16777215; // max 24 bit int - avoid exceding MW global precision
	std::vector<std::vector<long> > arrays_;
};
