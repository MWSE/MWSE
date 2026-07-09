module;

#include "NIAVObject.h"
#include "NITimeController.h"

#include "SolUtil.h"

export module NILookAtController;

namespace NI {
	export namespace LookAtControllerFlags {
		typedef TimeControllerFlags::value_type value_type;

		enum LookAtControllerFlags {
			FlipAxis = 0x10,
			AxisX = 0,
			AxisY = 0x20,
			AxisZ = 0x40,
			AxisMask = 0x60
		};
		enum LookAtControllerFlagBit {
			FlipAxisBit = 4,
			AxisBit = 5
		};
	}

	export struct LookAtController : TimeController {
		AVObject* lookAt; // 0x34. Not an NI::Pointer.

		unsigned int getAxis() const;
		void setAxis(unsigned int axis);
		bool getFlip() const;
		void setFlip(bool flip);
		AVObject* getLookAt() const;
		void setLookAt(AVObject* lookAtObject);
	};
	static_assert(sizeof(LookAtController) == 0x38, "NI::LookAtController failed size validation");
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::LookAtController)
#endif
