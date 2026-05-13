#include "mwOffsets.h"
#include "mwAdapter.h"
#include "MemAccess.h"
#include "MemoryUtil.h"

#include "VirtualMachine.h"

#include "ScriptUtil.h"

namespace mwse::mwAdapter {
	// House the registers obtained before/to return after hooking a function.
	static se::memory::HookContext context;

	// Our virtual machine. It's not really a virtual machine, and all the heavy
	// lifting is still done by the Morrowind scripting VM.
	static VirtualMachine vmInstance;

	// Determines if we want to own this opcode, and handles it if so.
	static void _stdcall HookGetNextInstructionIndirect() {
		OpCode::OpCode_t opcode = (OpCode::OpCode_t)context.eax;

		if (vmInstance.isOpcode(opcode)) {
			TES3::Script* script = reinterpret_cast<TES3::Script*>(context.ebx);

			vmInstance.loadParametersForOperation(opcode, context, script);

			// Set eax to zero. This tells Morrowind that the opcode is invalid and it will return GetNextInstruction.
			// It doesn't throw any errors when doing so.
			context.eax = 0x0;
		}
	}

	// If we own the opcode here, execute the code assigned to it.
	static float _stdcall HookRunFunctionIndirect() {
		OpCode::OpCode_t opcode = (OpCode::OpCode_t)context.edx;

		if (vmInstance.isOpcode(opcode)) {
			TES3::Script* script = *(reinterpret_cast<TES3::Script**>(context.esp + 0x8));

			// Our default return address. This can be changed below.
			context.callbackAddress = 0x50D62D;

			float returnValue = vmInstance.executeOperation(opcode, context, script);

			// Increment script instruction pointer in esi, when called from game_executeScript function only.
			long callReturn = *reinterpret_cast<long*>(context.ebp + 4);
			if (callReturn == 0x5053C0u) {
				long* returnESI = reinterpret_cast<long*>(context.esp + 4);
				*returnESI = *(vmInstance.getScriptIP());
			}

			return returnValue;
		}
		else {
			return 0.0f;
		}
	}

	// Called when a reference is created for during PlaceAtPC
	static void _stdcall HookPlaceAtPCReferenceCreatedIndirect() {
		TES3::Reference* reference = reinterpret_cast<TES3::Reference*>(context.esi);
		mwse::mwscript::OnPlaceReferenceCreated(reference);
	}

	// Code to generate a jump in memory. Don't forget to unprotect it first!
	void genJump(DWORD Address, DWORD To) {
		MemAccess<unsigned char>::Set(Address, 0xE9);
		MemAccess<DWORD>::Set(Address + 1, To - Address - 0x5);
	}

	// Code to generate a call in memory. Don't forget to unprotect it first!
	void genCall(DWORD Address, DWORD To) {
		MemAccess<unsigned char>::Set(Address, 0xE8);
		MemAccess<DWORD>::Set(Address + 1, To - Address - 0x5);
	}

	// Code to generate a nop in memory. Don't forget to unprotect it first!
	void genNOP(DWORD Address) {
		MemAccess<unsigned char>::Set(Address, 0x90);
	}

	// Hook scaffolding for HookGetNextInstructionIndirect().
	static const DWORD getNextInstructionReturnAddress = 0x500561u;
	static __declspec(naked) void HookGetNextInstructionDirect() {
		__asm {
			// Save eax.
			mov context.eax, eax

			// Save the flags.
			pushfd
			pop eax
			mov context.flags, eax

			// Save the other registers.
			mov context.ebx, ebx
			mov context.ecx, ecx
			mov context.edx, edx
			mov context.esi, esi
			mov context.edi, edi
			mov context.ebp, ebp
			mov context.esp, esp

			// Overwritten code. This is adjusted slightly.
			//! TODO: Why was this changed?
			mov eax, 0x7CEC08
			mov [eax], esi

			// Actually use our hook.
			call HookGetNextInstructionIndirect

			// Restore all registers and flags.
			mov eax, context.eax
			mov ebx, context.ebx
			mov ecx, context.ecx
			mov edx, context.edx
			mov esi, context.esi
			mov edi, context.edi
			mov ebp, context.ebp
			mov esp, context.esp
			push context.flags
			popfd

			// Resume normal execution. This can't be changed.
			jmp getNextInstructionReturnAddress
		}
	}

	// Hook scaffolding for HookRunFunctionIndirect().
	static __declspec(naked) void HookRunFunctionDirect() {
		__asm {
			// Save eax.
			mov context.eax, eax

			// Save the flags.
			pushfd
			pop eax
			mov context.flags, eax

			// Save the other registers.
			mov context.ebx, ebx
			mov context.ecx, ecx
			mov context.edx, edx
			mov context.esi, esi
			mov context.edi, edi
			mov context.ebp, ebp
			mov context.esp, esp

			// Store our default return address. HookRunFunctionIndirect may
			// change this (relatively) safely.
			mov context.callbackAddress, 0x505837

			// Actually use our hook.
			call HookRunFunctionIndirect

			// Restore all registers and flags.
			mov eax, context.eax
			mov ebx, context.ebx
			mov ecx, context.ecx
			mov edx, context.edx
			mov esi, context.esi
			mov edi, context.edi
			mov ebp, context.ebp
			mov esp, context.esp
			push context.flags
			popfd

			// We use the stack from original runFunction, adjust to compensate.
			// Store FPU FP0 in [esp+0C], and pop it off the stack.
			fstp [esp + 0x0C]

			// Overwritten code from our hook that we need to duplicate.
			add edx, 0x0FFFFF000

			// Return to where execution was before.
			jmp context.callbackAddress
		}
	}

	// Define return address, because we can't use constexpr here.
#define HookPlaceAtPCReferenceCreatedDirect_Return 0x509998
	static_assert(TES3_HOOK_PLACE_GETREFERENCE_RETURN == HookPlaceAtPCReferenceCreatedDirect_Return);
	static __declspec(naked) void HookPlaceAtPCReferenceCreatedDirect() {
		__asm {
			// Save eax.
			mov context.eax, eax

			// Save the flags.
			pushfd
			pop eax
			mov context.flags, eax

			// Save the other registers.
			mov context.ebx, ebx
			mov context.ecx, ecx
			mov context.edx, edx
			mov context.esi, esi
			mov context.edi, edi
			mov context.ebp, ebp
			mov context.esp, esp

			// Store our default return address. HookRunFunctionIndirect may
			// change this (relatively) safely.
			mov context.callbackAddress, HookPlaceAtPCReferenceCreatedDirect_Return

			// Actually use our hook.
			call HookPlaceAtPCReferenceCreatedIndirect

			// Restore all registers and flags.
			mov eax, context.eax
			mov ebx, context.ebx
			mov ecx, context.ecx
			mov edx, context.edx
			mov esi, context.esi
			mov edi, context.edi
			mov ebp, context.ebp
			mov esp, context.esp
			push context.flags
			popfd

			// Overwritten code at this hook's address.
			mov ecx, TES3_DATA_HANDLER_IMAGE
			mov ecx, [ecx]

			// Return to where execution was before.
			jmp context.callbackAddress
		}
	}

	// Put our hooks into the Morrowind engine.
	void Hook() {
		DWORD OldProtect;

		// Add our first hook. This hook happens when Morrowind is trying to
		// determine the next instruction. We determine if we want to own the
		// opcode and set up anything important here.
		VirtualProtect((DWORD*)TES3_HOOK_GET_NEXT_INSTRUCTION, TES3_HOOK_GET_NEXT_INSTRUCTION_SIZE, PAGE_READWRITE, &OldProtect);
		genJump(TES3_HOOK_GET_NEXT_INSTRUCTION, reinterpret_cast<DWORD>(HookGetNextInstructionDirect));
		genNOP(TES3_HOOK_GET_NEXT_INSTRUCTION_RETURN - 1);
		VirtualProtect((DWORD*)TES3_HOOK_GET_NEXT_INSTRUCTION, TES3_HOOK_GET_NEXT_INSTRUCTION_SIZE, OldProtect, &OldProtect);

		// Add our second hook. This is where the actual run call would get
		// executed, and if we own the opcode we again take over and run our code.
		VirtualProtect((DWORD*)TES3_HOOK_RUN_FUNCTION, TES3_HOOK_RUN_FUNCTION_SIZE, PAGE_READWRITE, &OldProtect);
		genJump(TES3_HOOK_RUN_FUNCTION, reinterpret_cast<DWORD>(HookRunFunctionDirect));
		genNOP(TES3_HOOK_RUN_FUNCTION_RETURN - 1);
		VirtualProtect((DWORD*)TES3_HOOK_RUN_FUNCTION, TES3_HOOK_RUN_FUNCTION_SIZE, OldProtect, &OldProtect);

		// PlaceAtPC creates a new reference to an object. We want to return
		// that with xPlace, but can't get the value back cleanly. We'll have to
		// grab it mid-execution.
		VirtualProtect((DWORD*)TES3_HOOK_PLACE_GETREFERENCE, TES3_HOOK_PLACE_GETREFERENCE_SIZE, PAGE_READWRITE, &OldProtect);
		genJump(TES3_HOOK_PLACE_GETREFERENCE, reinterpret_cast<DWORD>(HookPlaceAtPCReferenceCreatedDirect));
		genNOP(TES3_HOOK_PLACE_GETREFERENCE_RETURN - 1);
		VirtualProtect((DWORD*)TES3_HOOK_PLACE_GETREFERENCE, TES3_HOOK_PLACE_GETREFERENCE_SIZE, OldProtect, &OldProtect);
	}

	// If we need access to our virtual machine outside of the usual context,
	// this is how we get it. Useful for things like our MGE XE interface hack.
	VirtualMachine* GetVMInstance() {
		return &vmInstance;
	}
}