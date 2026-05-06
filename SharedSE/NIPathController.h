#pragma once

#include "NIAnimationData.h"
#include "NITimeController.h"

namespace NI {
	namespace PathControllerFlags {
		typedef TimeControllerFlags::value_type value_type;

		enum PathControllerFlags {
			ConstantVelocityUpdateNeeded = 0x10,
			OpenCurve                    = 0x20,
			AllowFlip                    = 0x40,
			Bank                         = 0x80,
			ConstantVelocity             = 0x100,
			Follow                       = 0x200,
			FlipFollowAxis               = 0x400
		};
		enum PathControllerFlagBit {
			ConstantVelocityUpdateNeededBit = 4,
			OpenCurveBit                    = 5,
			AllowFlipBit                    = 6,
			BankBit                         = 7,
			ConstantVelocityBit             = 8,
			FollowBit                       = 9,
			FlipFollowAxisBit               = 10
		};
	}

	struct PathController : TimeController {
		int lastUsedPathIndex; // 0x34
		int lastUsedPercentIndex; // 0x38
		Pointer<PosData> pathData; // 0x3C
		Pointer<FloatData> percentData; // 0x40
		float* partialLengths; // 0x44
		float totalLength; // 0x48
		float maxBankAngle; // 0x4C
		float smoothing; // 0x50
		short followAxis; // 0x54
		float bank_related_data; // 0x58
		int bankDirection; // 0x5C

		bool getAllowFlip() const;
		void setAllowFlip(bool value);
		bool getBank() const;
		void setBank(bool value);
		bool getConstantVelocity() const;
		void setConstantVelocity(bool value);
		bool getFollow() const;
		void setFollow(bool value);
		bool getFlipFollowAxis() const;
		void setFlipFollowAxis(bool value);
		bool getOpenCurve() const;
		void setOpenCurve(bool value);
	};
	static_assert(sizeof(PathController) == 0x60, "NI::PathController failed size validation");
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::PathController)
#endif
