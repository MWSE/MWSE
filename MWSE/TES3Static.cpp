#include "TES3Static.h"

namespace TES3 {
	const auto TES3_Static_ctor = reinterpret_cast<void(__thiscall*)(Static*)>(0x4A72D0);
	Static::Static() :
		model{}
	{
		TES3_Static_ctor( this );
	}

	const auto TES3_Static_dtor = reinterpret_cast<void(__thiscall*)(Static*)>(0x4A7350);
	Static::~Static() {
		TES3_Static_dtor( this );
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Static)
