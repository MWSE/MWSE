#pragma once

#include "mwAdapter.h"
#include "OpCodes.h"

#include "TES3Defines.h"

namespace mwse {
	class VMHookInterface {
	public:
		virtual float executeOperation(OpCode::OpCode_t opcode, se::memory::HookContext& context, TES3::Script* script) = 0;
		virtual void loadParametersForOperation(OpCode::OpCode_t opcode, se::memory::HookContext& context, TES3::Script* script) = 0;

		virtual bool isOpcode(const OpCode::OpCode_t opcode) = 0;
	};
}