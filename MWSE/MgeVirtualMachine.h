#pragma once

#define DEBUG_MGE_VM false

#include "MgeVmtypes.h"
#include "MgeInstruction.h"

typedef enum {GP=1, GPMAX=16, IP=17, SP=18} VMREGS;
typedef enum {FZERO=1,FPOS=2} FLAGS;
typedef char MEMACCESSOR;
struct ADDRESSSPACE;

struct VIRTUALMACHINE
{
	virtual ~VIRTUALMACHINE(void);
	

	virtual bool GetRegister(WORD regidx, VMREGTYPE& value)=0;
	virtual bool SetRegister(WORD regidx, VMREGTYPE value)=0;
	
	virtual bool IsInstruction(OPCODE inst);
	virtual bool step(bool skip= false);
	virtual bool run(void);
	
	virtual bool GetInstruction(VPVOID addr, OPCODE& opcode);
	virtual bool AccessMem(MEMACCESSOR& access, VPVOID addr, VOID* buf, VMSIZE size);
	virtual bool ReadMem(const VPVOID addr, VOID* buf, VMSIZE size);
	virtual bool WriteMem(VPVOID addr, const VOID* buf, VMSIZE size);

	virtual void SetFlags(VMREGTYPE value);
	virtual void SetFlags(VMFLOAT value);
	virtual VMFLAGSTYPE GetFlags(void);
	
	virtual bool push(VMREGTYPE val);
	virtual bool push(VMFLOAT val);
	virtual bool pop(VMREGTYPE& val);
	virtual bool pop(VMFLOAT& val);
	virtual void BadOpcodeHandler(OPCODE opcode, VPVOID ip);

	virtual bool Interrupt(VMINTERRUPT intnum);

	bool AddAddressSpace(VPVOID position, ADDRESSSPACE* space);
	bool AddInstruction(OPCODE opcode, INSTRUCTION* instruction);

private:
	//INSTRUCTIONMAP instructions;
	//ADDRESSSPACEMAP memory;
	VMFLAGSTYPE flags;
};

template<typename T>
struct VMPTR
{
	VMPTR(VIRTUALMACHINE& machine, T* to= 0)
	: vm(machine), ptr(to), oldptr(0)
	{
	}
	VMPTR(VMPTR<T>& to)
	: vm(to.vm), ptr(to.ptr), oldptr(to.oldptr), realobject(to.realobject)
	{
	}
//	operator bool() const
//	{
//		return (bool)ptr;
//	}
	operator T*()
	{
		if(!ptr)
			return 0;
			
		readobject();
		return &realobject;
	}
	T* operator->()
	{
		if(!ptr)
			return 0;
			
		readobject();
		return &realobject;
	}
	T& operator*()
	{
		if(!ptr)
			throw 0;
			
		readobject();
		return realobject;
	}
	VMPTR<T>& operator=(T* to)
	{
		ptr= to;
		return *this;
	}
	VMPTR<T>& operator=(VMPTR<T>& to)
	{
		vm= to.vm;
		ptr= to.ptr;
		oldptr= to.oldptr;
		realobject= to.realobject;
		return *this;
	}
	T** operator&()
	{
		return &ptr;
	}
	void readobject(void)
	{
		if(ptr && oldptr!=ptr)
		{
			if(!vm.ReadMem((VPVOID)ptr,&realobject,sizeof(realobject)))
				throw 0;
			oldptr= ptr;
		}
	}
  
private:
	T* ptr;
	T* oldptr;
	T realobject;
	VIRTUALMACHINE& vm;
};
