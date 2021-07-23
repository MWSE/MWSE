#include "MgeTes3Machine.h"

#include "mwseString.h"

#include "mwseString.h"
#include "mwOffsets.h"
#include "VirtualMachine.h"
#include "StringUtil.h"

#include "TES3MobilePlayer.h"
#include "TES3WorldController.h"

#define PLACEHOLDER 0

#define SUPER VIRTUALMACHINE

TES3MACHINE::TES3MACHINE() 
: SUPER()
	,scriptaddr(0)
	,script()
	,flow()
	,instructionpointer(0)
	,stackpointer(0)
	//,executable(new BUFSPACE(SCRIPTMEM_SIZE))
	//,rng_(std::time(0))
{}

bool TES3MACHINE::GetRegister(WORD regidx, VMREGTYPE& value) //get the info from a processor register (EDX,EIP,...)
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	/*
	bool result= true;
	
	switch(regidx)
	{
	case IP:
		value= instructionpointer;
		result= true;
		break;
	case SP:
		value= stackpointer;
		break;
	default:
		if(regidx<=GPMAX)
			value= generalregs[regidx-GP];
		else
			result= false;
	}
	
	return result;
	*/
	return true;
}

bool TES3MACHINE::SetRegister(WORD regidx, VMREGTYPE value) //write stuff into a processor register (EDX,EIP,...)
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	/*
	bool result= true;
	
	switch(regidx)
	{
	case IP:
		instructionpointer= value;
		result= true;
		break;
	case SP:
		//fixed error 4018 (hope it works :P)
		if(value && value < (signed)STACKMEM_MIN) // || value>STACKMEM_MAX) // how do I test the upper limit?
			result= false;
		else
			stackpointer= value;
		break;
	default:
		if(regidx<=GPMAX)
		{
			SetFlags(value);
			generalregs[regidx-GP]= value;
		}
		else
			result= false;
	}
	
	return result;
	*/
	return true;
}

bool TES3MACHINE::SetScript(VPSCRIPT pscript)
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	/*
	scriptaddr=	pscript;
	return ReadMem((VPVOID)pscript,&script,sizeof(script));
	*/
	return true;
}

bool TES3MACHINE::Interrupt(VMINTERRUPT num)
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	/*
	bool result= false;
	if(num==INTSWITCHREFERENCE)
	{
		BYTE buf[512];
		VMSIZE read= 0;
		VMSIZE offset= 0;
		result= true;

        while(result && (read=min((VMSIZE)sizeof(buf),script.scdtlength-offset))>0)
		{
			result= (ReadMem(script.scdt+offset,buf,read)
				&& WriteMem((VPVOID)SCRIPTMEM_VPOS+offset,buf,read));
			offset+= read;
		}
		strings.clear();
		SetRegister(SP,0);
	}
		
	return result;
	*/
	return true;
}

const Context TES3MACHINE::GetFlow(void) //return the Flow context, (registers, ...)
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	return flow;
}

void TES3MACHINE::SetFlow(const Context newflow) //set the flow context (registers, ...)
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	this->flow= newflow;
}

bool TES3MACHINE::SetVMDebuggerBreakpoint(HWBREAKPOINT* breakpoint)
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	/*if(this->breakpoint && breakpoint) return false;
	this->breakpoint= breakpoint;*/
	return true;
}

HWBREAKPOINT* TES3MACHINE::GetVMDebuggerBreakpoint()
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	return breakpoint;
}

const char* TES3MACHINE::GetScriptName(void)
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	/*
	const char* result= NULL;
	if(scriptaddr)
		result= (const char*)script.name;
		
	return result;
	*/
	return NULL;
}

const char* TES3MACHINE::GetString(VPVOID addr)
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	mwse::mwseString& string = mwse::mwAdapter::GetVMInstance()->getString((long)addr);
	return string.c_str();
}


bool TES3MACHINE::dumpmem(VPVOID ptr, int size)
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	/*
	bool result= false;
	BYTE* buf= new BYTE[size];
	if(ReadMem(ptr,buf,size))
	{
		cLog::mLogBinaryMessage(buf, size);
		result= true;
	}
	else
		cLog::mLogMessage("dumpmem: ReadMem failed");
	delete[] buf;
	return result;

	return true;
	*/
	return true;
}

void TES3MACHINE::dumpscriptstack(void)
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	/*
	VMREGTYPE sp= 0;
	if(GetRegister(SP,sp))
	{
		cLog::mLogMessage("script sp %lx\n",sp);
		int size= 0-sp;
		dumpmem((VPVOID)sp,size<64?size:64);
	}
	else
		cLog::mLogMessage("dumpscriptstack: GetRegister failed");
	*/
}

void TES3MACHINE::dumpscript(void)
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	/*
	cLog::mLogMessage("Stack at %lx\n",scriptaddr);
	cLog::mLogBinaryMessage((LPVOID)&script,sizeof(script));
	cLog::mLogMessage("SCDT\n");
	dumpmem(script.scdt,script.scdtlength);
	*/
}

void TES3MACHINE::dumptemplate(VPTEMPLATE ptempl)
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	/*
	TES3TEMPLATE templ;
	char buf[256];
	if(ReadMem((VPVOID)ptempl,(void*)&templ,sizeof(TES3TEMPLATE)))
	{
		cLog::mLogMessage("Template from %lx\n",ptempl);
		cLog::mLogBinaryMessage((void*)&templ,sizeof(TES3TEMPLATE));
		if(ReadMem((VPVOID)templ.objectid,buf,32))
		{
			cLog::mLogMessage("ObjectID from %lx\n",templ.objectid);
			cLog::mLogBinaryMessage(buf,32);
		}
		else
			cLog::mLogMessage("dumpobject: ObjectID failed\n");
	}
	else
		cLog::mLogMessage("dumpobject: Template failed\n");
	*/

}
bool consoleCreated = false;
void TES3MACHINE::dumpobject(VPREFERENCE pref)
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	/*
    if (!consoleCreated) {
        AllocConsole();
    }
	// 2005-02-05  CDC     2005-07-06  0 is an address, non-digits quit now
	// Allows for interactive exploration of memory - not pretty, but works
	// You'll need a fairly wide screen and should start the extender from
	// a CygWin shell window for best effect.
	// The MEMLOOK function will trigger this code which breaks out of the
	// program and dumps the REFR data for the current object.
	// Warning: Memory addresses in the listing are in little-endian order
	// but those listed on the first line aren't.
	// You can type a memory address (in hex) to dump a section of memory
	// starting at that address or a letter (other than a through f and
	// A through F) to return to the game.  If you type a small hex number 
	// the value will be used like an array subscript to pick a memory 
	// address from the currently shown data treating the data as an array 
	// of long values.  Some interesting values include:
	//   From the REFR data:
	//      0A  the template for the current object
	//      11  the attachments for the current object
	//      05  a cell link field (from there 03 takes you to the cell template)
	//   From the template data area:
	//	0B  the objectid for this object
	//	1B  a second template I call the base template
	//   For NPC_ type template or base template data:
	//	14  the character's real name
	//	2C  the character's race
	//	2D  the character's class
	//	2E  the character's head
	//	2F  the character's hair
	//   For the attachment list, the first item is a type code
	//	01  is the next attachment list item
	//	02  is the data for that attachment
	//   For attachement type 6, the first field (00) is a count.
	//	01  is the owner
	//	05  is the script
	//   Attachement type 8 is long and has lot's of interesting data
	unsigned long addr = (unsigned long) pref;
        unsigned x, y, size;
	BYTE buffer[1024];
	BYTE* ptr= buffer;

	size = sizeof(TES3REFERENCE);
	_cprintf("Script %lx %lx: Reference from ",scriptaddr,script);

    if ( addr <= 0x40 ) 
		addr = *((unsigned long *)(ptr+addr*4));
	_cprintf("%lX - %lX\n", addr, addr+size);
	if(!ReadMem((VPVOID)addr,(void*)&buffer,size))
		return;
	for(y=0;y<size;y+=32)
	{
		for(x=0;x<32 && (x+y)<size;x++) {
			_cprintf("%02X",((int)ptr[y+x])&0xFF);
			if ( x == 3 || x == 11 || x == 19 || x == 27 )
				_cprintf(" ");
			if ( x == 7 || x == 23 )
				_cprintf("  ");
			if ( x == 15 )
				_cprintf(" | ");
		}
		_cprintf(" ");
		for(x=0;x<32 && (x+y)<size;x++) {
			if(isprint(ptr[y+x]))
				_cprintf("%c",ptr[y+x]);
			else
				_cprintf(".");
			if ( x == 3 || x == 11 || x == 19 || x == 27 )
				_cprintf(" ");
			if ( x == 7 || x == 23 )
				_cprintf("  ");
			if ( x == 15 )
				_cprintf(" | ");
		}
#if 0
		_cprintf("\n");
		for(x=0;x<32 && (x+y)<size;x+=4) {
			_cprintf("%4.2f ",*((float*)(ptr+y+x)));
			if ( x == 7 || x == 23 )
				_cprintf(" ");
			if ( x == 15 )
				_cprintf(" | ");
		}

		_cprintf("  ");
		for(x=0;x<32 && (x+y)<size;x+=4) {
			_cprintf("%ld ",*((unsigned long*)(ptr+y+x)));
			if ( x == 7 || x == 23 )
				_cprintf(" ");
			if ( x == 15 )
				_cprintf(" | ");
		}
#endif
		_cprintf("\n");
	}
	*/
}

void TES3MACHINE::dumpobjects(void)
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	/*
	const Context context( GetFlow() );
	VPVOID master2= 0;
	VPVOID target= 0;
	VPVOID pvariables= 0;
	TES3VARIABLES variables;
	
	cLog::mLogMessage("Stack from %lx\n",context.Esp);
	if(!dumpmem((VPVOID)context.Esp,64))
		cLog::mLogMessage("dump: Stack failed\n");
		
	cLog::mLogMessage("Base from %lx\n",context.Ebp);
	if(!dumpmem((VPVOID)context.Ebp,64))
		cLog::mLogMessage("dump: Stack failed\n");

	cLog::mLogMessage("View Target:\n");
	if(ReadMem((VPVOID)reltolinear(MASTER2_IMAGE),&master2,sizeof(master2)))
	{
		cLog::mLogMessage("master2= %lx\n",master2);
		if(ReadMem(master2+0x4c+0x9c,&target,sizeof(target)))
			dumpobject((VPREFERENCE)target);
		else
			cLog::mLogMessage("dump: target failed\n");
	}
	else
		cLog::mLogMessage("dump: ViewData failed\n");

	cLog::mLogMessage("Script Local Variables\n");
	dumpobject((VPREFERENCE)context.Ecx);
	
	if(ReadMem((VPVOID)reltolinear(LOCALVARIABLES_IMAGE),&pvariables,sizeof(pvariables))
		&& ReadMem(pvariables,&variables,sizeof(variables)))
	{
		cLog::mLogMessage("LOCALVARIABLES from %lx\n",pvariables);
		cLog::mLogBinaryMessage(&variables,sizeof(variables));
	}
	else
		cLog::mLogMessage("dump: LOCALVARIABLES failed\n");
		
	cLog::mLogMessage("script at %lx\n",scriptaddr);
	cLog::mLogBinaryMessage((LPVOID)&script,sizeof(script));
	*/
}

void TES3MACHINE::searchforscripttarget(void)
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	/*
	const Context context( GetFlow() );
	DWORD scripttarget= context.Ecx;
	cLog::mLogMessage("Searching for script target %lx\n",scripttarget);

	for(VPVOID pagestart= (VPVOID)0;pagestart<(VPVOID)0x80000000;pagestart+= 0x1000)
	{
		BYTE buf[0x1000];
		if(ReadMem(pagestart,buf,sizeof(buf)))
		{
			for(int i=0;i<= sizeof(buf)-sizeof(DWORD);i++)
			{
				DWORD val= *(DWORD*)&buf[i];
				if(val==scripttarget)
					cLog::mLogMessage("found scripttarget pointer %lx at %lx\n",val,pagestart+i);
			}
		}
	}
	*/
}

void TES3MACHINE::CheckForSkillUp(long skill_id)
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	TES3::WorldController::get()->getMobilePlayer()->levelSkill(skill_id);
}

TES3::MobilePlayer* TES3MACHINE::GetMacpRecord()
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	return TES3::WorldController::get()->getMobilePlayer();
}

long TES3MACHINE::GetRandomLong(long min, long max)
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	std::uniform_int_distribution<long> dist(min, max);
	return dist(rng_);
}

float TES3MACHINE::GetRandomFloat(float min, float max)
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	std::uniform_real_distribution<float> dist(min, max);
	return dist(rng_);
}

long TES3MACHINE::CreateArray(std::string const& caller)
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	long id = 0;
	if (arrays_.size() < kMaxArrayId) {
		id = arrays_.size() + 1;
		arrays_.push_back(std::vector<long>());
	} else {
		std::string const error_message = caller +
			": Unable to create array. Maximum number of arrays reached. id: %d\n";
		//cLog::mLogMessage(error_message.c_str(), id);
	}
	return id;
}

long TES3MACHINE::GetArrayValue(std::string const& caller, long const id,
	long const index)
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	long value = 0;
	if (id > 0 && id <= arrays_.size()) {
		std::vector<long> const& a = arrays_[id - 1];
		if (index >= 0 && index < a.size()) {
			value = a[index];
		} else 	{
		std::string const error_message = caller +
			": Array index out of bounds. id: %d index: %d\n";
			///cLog::mLogMessage(error_message.c_str(), id, index);
		}
	} else {
		std::string const error_message = caller +
			": Invalid array id: %d\n";
		//cLog::mLogMessage(error_message.c_str(), id);
	}
	return value;
}

long TES3MACHINE::SetArrayValue(std::string const& caller, long const id,
	long const index, long const value)
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	long success = 0;
	if (id > 0 && id <= arrays_.size())	{
		if (index >= 0)	{
			std::vector<long>& a = arrays_[id - 1];
			if (index + 1 > a.size()) {
				a.resize(index + 1);
			}
			a[index] = value;
			success = 1;
		} else {
			std::string const error_message = caller +
				": Array index out of bounds. id: %d index: %d\n";
			//cLog::mLogMessage(error_message.c_str(), id, index);
		}
	} else {
		std::string const error_message = caller +
			": Invalid array id: %d\n";
		//cLog::mLogMessage(error_message.c_str(), id);
	}
	return success;
}

long TES3MACHINE::GetArraySize(std::string const& caller, long const id)
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	long size = 0;
	if (id > 0 && id <= arrays_.size()) {
		size = arrays_[id - 1].size();
	} else {
		std::string const error_message = caller +
			": Invalid array id: %d\n";
		//cLog::mLogMessage(error_message.c_str(), id);
	}
	return size;
}

long TES3MACHINE::ClearArray(std::string const& caller, long const id)
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	long success = 0;
	if (id > 0 && id <= arrays_.size()) {
		arrays_[id - 1].clear();
		success = 1;
	} else {
		std::string const error_message = caller +
			": Invalid array id: %d\n";
		//cLog::mLogMessage(error_message.c_str(), id);
	}
	return success;
}

std::vector<std::vector<long> >& TES3MACHINE::arrays()
{
#if DEBUG_MGE_VM
	mwse::log::getLog() << __FUNCTION__ << std::endl;
#endif
	return arrays_;
}