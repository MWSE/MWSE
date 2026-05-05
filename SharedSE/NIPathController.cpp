#include "NIPathController.h"

#include "BitUtil.h"

namespace NI {
	bool PathController::getAllowFlip() const      { return BIT_TEST(flags, PathControllerFlags::AllowFlipBit); }
	void PathController::setAllowFlip(bool value)  { BIT_SET(flags, PathControllerFlags::AllowFlipBit, value); }

	bool PathController::getBank() const           { return BIT_TEST(flags, PathControllerFlags::BankBit); }
	void PathController::setBank(bool value)       { BIT_SET(flags, PathControllerFlags::BankBit, value); }

	bool PathController::getConstantVelocity() const      { return BIT_TEST(flags, PathControllerFlags::ConstantVelocityBit); }
	void PathController::setConstantVelocity(bool value)  { BIT_SET(flags, PathControllerFlags::ConstantVelocityBit, value); }

	bool PathController::getFollow() const         { return BIT_TEST(flags, PathControllerFlags::FollowBit); }
	void PathController::setFollow(bool value)     { BIT_SET(flags, PathControllerFlags::FollowBit, value); }

	bool PathController::getFlipFollowAxis() const      { return BIT_TEST(flags, PathControllerFlags::FlipFollowAxisBit); }
	void PathController::setFlipFollowAxis(bool value)  { BIT_SET(flags, PathControllerFlags::FlipFollowAxisBit, value); }

	bool PathController::getOpenCurve() const      { return BIT_TEST(flags, PathControllerFlags::OpenCurveBit); }
	void PathController::setOpenCurve(bool value)  { BIT_SET(flags, PathControllerFlags::OpenCurveBit, value); }
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::PathController)
#endif
