#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	//----------------------------------------
	class Push : public InstructionInterface_t {
	public:
		Push();
		virtual float execute(VMExecuteInterface& virtualMachine);
		virtual void loadParameters(VMExecuteInterface& virtualMachine);
	protected:
	private:
		long pushData;
	};

	static Push pushInstance;

	Push::Push() : InstructionInterface_t(OpCode::_PushLong) {}

	void Push::loadParameters(mwse::VMExecuteInterface& virtualMachine) {
		pushData = virtualMachine.getLongValue();
	}

	float Push::execute(mwse::VMExecuteInterface& virtualMachine) {
		Stack::getInstance().pushLong(pushData);
		return 0.0f;
	}
	//----------------------------------------
	//----------------------------------------
	class PushB : public InstructionInterface_t {
	public:
		PushB();
		virtual float execute(VMExecuteInterface& virtualMachine);
		virtual void loadParameters(VMExecuteInterface& virtualMachine);
	protected:
	private:
		long pushData;
	};

	static PushB pushBInstance;

	PushB::PushB() : InstructionInterface_t(OpCode::_PushByte) {}

	void PushB::loadParameters(mwse::VMExecuteInterface& virtualMachine) {
		pushData = virtualMachine.getByteValue();
	}

	float PushB::execute(mwse::VMExecuteInterface& virtualMachine) {
		Stack::getInstance().pushByte(static_cast<char>(pushData));
		return 0.0f;
	}
	//----------------------------------------
	//----------------------------------------
	class PushS : public InstructionInterface_t {
	public:
		PushS();
		virtual float execute(VMExecuteInterface& virtualMachine);
		virtual void loadParameters(VMExecuteInterface& virtualMachine);
	protected:
	private:
		long pushData;
	};

	static PushS pushSInstance;

	PushS::PushS() : InstructionInterface_t(OpCode::_PushShort) {}

	void PushS::loadParameters(mwse::VMExecuteInterface& virtualMachine) {
		pushData = virtualMachine.getShortValue();
	}

	float PushS::execute(mwse::VMExecuteInterface& virtualMachine) {
		Stack::getInstance().pushShort(static_cast<short>(pushData));
		return 0.0f;
	}
	//----------------------------------------
}