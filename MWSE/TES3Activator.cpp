#include "TES3Activator.h"

namespace TES3 {
	static const auto TES3_Activator_ctor = reinterpret_cast<void(__thiscall*)(Activator*)>(0x49F990);
	Activator::Activator() :
		unknown_0x30{},
		model{},
		name{},
		script{}
	{
		TES3_Activator_ctor(this);
	}

	static const auto TES3_Activator_dtor = reinterpret_cast<void(__thiscall*)(Activator*)>(0x49FA50);
	Activator::~Activator()
	{
		TES3_Activator_dtor(this);
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Activator)
