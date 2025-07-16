#include "NITimeControllerLua.h"

#include "NIObjectLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIAnimationData.h"
#include "NILookAtController.h"
#include "NIKeyframeController.h"
#include "NIKeyframeManager.h"
#include "NIPathController.h"
#include "NITimeController.h"

namespace mwse::lua {
	template <typename T>
	void setUserdataForNITimeController(sol::usertype<T>& usertypeDefinition) {
		setUserdataForNIObject(usertypeDefinition);

		// Basic property binding.
		usertypeDefinition["frequency"] = &NI::TimeController::frequency;
		usertypeDefinition["phase"] = &NI::TimeController::phase;
		usertypeDefinition["lowKeyFrame"] = &NI::TimeController::lowKeyFrame;
		usertypeDefinition["highKeyFrame"] = &NI::TimeController::highKeyFrame;
		usertypeDefinition["startTime"] = &NI::TimeController::startTime;
		usertypeDefinition["lastScaledTime"] = &NI::TimeController::lastScaledTime;
		usertypeDefinition["lastTime"] = &NI::TimeController::lastTime;
		usertypeDefinition["target"] = sol::readonly_property(&NI::TimeController::target);
		usertypeDefinition["nextController"] = &NI::TimeController::nextController;
		usertypeDefinition["active"] = sol::property(&NI::TimeController::getActive, &NI::TimeController::setActive);
		usertypeDefinition["animTimingType"] = sol::property(&NI::TimeController::getAnimTimingType, &NI::TimeController::setAnimTimingType);
		usertypeDefinition["cycleType"] = sol::property(&NI::TimeController::getCycleType, &NI::TimeController::setCycleType);

		// Basic function binding.
		usertypeDefinition["setTarget"] = &NI::TimeController::setTarget;
		usertypeDefinition["start"] = &NI::TimeController::start;
		usertypeDefinition["stop"] = &NI::TimeController::stop;
	}

	void bindNITimeController() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Bind NI::TimeController
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::TimeController>("niTimeController");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::Object>();
			setUserdataForNITimeController(usertypeDefinition);
		}

		// Bind NI::LookAtController
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::LookAtController>("niLookAtController");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::TimeController, NI::Object>();
			setUserdataForNITimeController(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["axis"] = sol::property(&NI::LookAtController::getAxis, &NI::LookAtController::setAxis);
			usertypeDefinition["flip"] = sol::property(&NI::LookAtController::getFlip, &NI::LookAtController::setFlip);
			usertypeDefinition["lookAt"] = sol::property(&NI::LookAtController::getLookAt, &NI::LookAtController::setLookAt);
		}

		// Bind various keyframe data types.
		{
			// Bind NI::AnimationKey
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<NI::AnimationKey>("niAnimationKey");
				usertypeDefinition["new"] = sol::no_constructor;

				// Basic property binding.
				usertypeDefinition["timing"] = &NI::AnimationKey::timing;
			}

			// Bind NI::FloatKey
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<NI::FloatKey>("niFloatKey");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::AnimationKey>();
				usertypeDefinition["timing"] = &NI::FloatKey::timing;

				// Basic property binding.
				usertypeDefinition["value"] = &NI::FloatKey::value;
			}

			// Bind NI::BezFloatKey
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<NI::BezFloatKey>("niBezFloatKey");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::FloatKey, NI::AnimationKey>();
				usertypeDefinition["timing"] = &NI::BezFloatKey::timing;
				usertypeDefinition["value"] = &NI::BezFloatKey::value;

				// Basic property binding.
				usertypeDefinition["inTangent"] = &NI::BezFloatKey::inTangent;
				usertypeDefinition["outTangent"] = &NI::BezFloatKey::outTangent;
			}

			// Bind NI::TCBFloatKey
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<NI::TCBFloatKey>("niTCBFloatKey");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::FloatKey, NI::AnimationKey>();
				usertypeDefinition["timing"] = &NI::TCBFloatKey::timing;
				usertypeDefinition["value"] = &NI::TCBFloatKey::value;

				// Basic property binding.
				usertypeDefinition["bias"] = &NI::TCBFloatKey::bias;
				usertypeDefinition["continuity"] = &NI::TCBFloatKey::continuity;
				usertypeDefinition["derivedA"] = &NI::TCBFloatKey::derivedA;
				usertypeDefinition["derivedB"] = &NI::TCBFloatKey::derivedB;
				usertypeDefinition["tension"] = &NI::TCBFloatKey::tension;
			}

			// Bind NI::RotKey
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<NI::RotKey>("niRotKey");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::AnimationKey>();
				usertypeDefinition["timing"] = &NI::RotKey::timing;

				// Basic property binding.
				usertypeDefinition["value"] = &NI::RotKey::value;
			}

			// Bind NI::BezRotKey
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<NI::BezRotKey>("niBezRotKey");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::RotKey, NI::AnimationKey>();
				usertypeDefinition["timing"] = &NI::BezRotKey::timing;
				usertypeDefinition["value"] = &NI::BezRotKey::value;

				// Basic property binding.
				usertypeDefinition["intermediate"] = &NI::BezRotKey::intermediate;
			}

			// Bind NI::TCBRotKey
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<NI::TCBRotKey>("niTCBRotKey");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::RotKey, NI::AnimationKey>();
				usertypeDefinition["timing"] = &NI::TCBRotKey::timing;
				usertypeDefinition["value"] = &NI::TCBRotKey::value;

				// Basic property binding.
				usertypeDefinition["bias"] = &NI::TCBRotKey::bias;
				usertypeDefinition["continuity"] = &NI::TCBRotKey::continuity;
				usertypeDefinition["intermediateA"] = &NI::TCBRotKey::intermediateA;
				usertypeDefinition["intermediateB"] = &NI::TCBRotKey::intermediateB;
				usertypeDefinition["tension"] = &NI::TCBRotKey::tension;
			}

			// Bind NI::EulerRotKey
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<NI::EulerRotKey>("niEulerRotKey");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::RotKey, NI::AnimationKey>();
				usertypeDefinition["timing"] = &NI::EulerRotKey::timing;
				usertypeDefinition["value"] = &NI::EulerRotKey::value;

				// Basic property binding.
				usertypeDefinition["keyOrder"] = &NI::EulerRotKey::order;
				usertypeDefinition["keys"] = sol::readonly_property(&NI::EulerRotKey::getKeys_lua);
				usertypeDefinition["keyTypes"] = sol::readonly_property(&NI::EulerRotKey::getKeyTypes_lua);
				usertypeDefinition["lastIndices"] = sol::readonly_property(&NI::EulerRotKey::getLastIndices_lua);
				usertypeDefinition["numKeys"] = sol::readonly_property(&NI::EulerRotKey::getNumKeys_lua);
			}

			// Bind NI::PosKey
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<NI::PosKey>("niPosKey");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::AnimationKey>();
				usertypeDefinition["timing"] = &NI::PosKey::timing;

				// Basic property binding.
				usertypeDefinition["value"] = &NI::PosKey::value;
			}

			// Bind NI::BezPosKey
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<NI::BezPosKey>("niBezPosKey");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::PosKey, NI::AnimationKey>();
				usertypeDefinition["timing"] = &NI::BezPosKey::timing;
				usertypeDefinition["value"] = &NI::BezPosKey::value;

				// Basic property binding.
				usertypeDefinition["inTangent"] = &NI::BezPosKey::inTangent;
				usertypeDefinition["intermediateA"] = &NI::BezPosKey::intermediateA;
				usertypeDefinition["intermediateB"] = &NI::BezPosKey::intermediateB;
				usertypeDefinition["outTangent"] = &NI::BezPosKey::outTangent;
			}

			// Bind NI::TCBPosKey
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<NI::TCBPosKey>("niTCBPosKey");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::PosKey, NI::AnimationKey>();
				usertypeDefinition["timing"] = &NI::TCBPosKey::timing;
				usertypeDefinition["value"] = &NI::TCBPosKey::value;

				// Basic property binding.
				usertypeDefinition["bias"] = &NI::TCBPosKey::bias;
				usertypeDefinition["continuity"] = &NI::TCBPosKey::continuity;
				usertypeDefinition["derivedA"] = &NI::TCBPosKey::derivedA;
				usertypeDefinition["derivedB"] = &NI::TCBPosKey::derivedB;
				usertypeDefinition["intermediateA"] = &NI::TCBPosKey::intermediateA;
				usertypeDefinition["intermediateB"] = &NI::TCBPosKey::intermediateB;
				usertypeDefinition["tension"] = &NI::TCBPosKey::tension;
			}
		}

		// Bind NI::KeyframeData
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::KeyframeData>("niKeyframeData");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::Object>();
			setUserdataForNIObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["positionKeyCount"] = sol::readonly_property(&NI::KeyframeData::positionKeyCount);
			usertypeDefinition["positionKeys"] = sol::readonly_property(&NI::KeyframeData::getPositionKeys_lua);
			usertypeDefinition["positionType"] = sol::readonly_property(&NI::KeyframeData::positionType);
			usertypeDefinition["rotationKeyCount"] = sol::readonly_property(&NI::KeyframeData::rotationKeyCount);
			usertypeDefinition["rotationKeys"] = sol::readonly_property(&NI::KeyframeData::getRotationKeys_lua);
			usertypeDefinition["rotationType"] = sol::readonly_property(&NI::KeyframeData::rotationType);
			usertypeDefinition["scaleKeyCount"] = sol::readonly_property(&NI::KeyframeData::scaleKeyCount);
			usertypeDefinition["scaleKeys"] = sol::readonly_property(&NI::KeyframeData::getScaleKeys_lua);
			usertypeDefinition["scaleType"] = sol::readonly_property(&NI::KeyframeData::scaleType);

			// Basic function binding.
			usertypeDefinition["getRotationKeyIndex"] = &NI::KeyframeData::getRotationKeyIndex_lua;
			usertypeDefinition["getPositionKeyIndex"] = &NI::KeyframeData::getPositionKeyIndex_lua;
			usertypeDefinition["getScaleKeyIndex"] = &NI::KeyframeData::getScaleKeyIndex_lua;
			usertypeDefinition["updateDerivedValues"] = &NI::KeyframeData::updateDerivedValues;
		}

		// Bind NI::KeyframeController
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::KeyframeController>("niKeyframeController");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::TimeController, NI::Object>();
			setUserdataForNITimeController(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["data"] = sol::readonly_property(&NI::KeyframeController::keyframeData);
			usertypeDefinition["lastUsedPositionIndex"] = &NI::KeyframeController::lastPosIndex;
			usertypeDefinition["lastUsedRotationIndex"] = &NI::KeyframeController::lastRotIndex;
			usertypeDefinition["lastUsedScaleIndex"] = &NI::KeyframeController::lastScaleIndex;
		}

		// Bind NI::KeyframeManager
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::KeyframeManager>("niKeyframeManager");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::TimeController, NI::Object>();
			setUserdataForNITimeController(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["cumulative"] = &NI::KeyframeManager::cumulative;
			usertypeDefinition["globalScaleRotation"] = &NI::KeyframeManager::globalScaleRotation;
			usertypeDefinition["globalTranslation"] = &NI::KeyframeManager::globalTranslation;
			usertypeDefinition["sequences"] = sol::readonly_property(&NI::KeyframeManager::sequences);
		}

		// Bind NI::Sequence
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::Sequence>("niSequence");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["boneNames"] = sol::readonly_property(&NI::Sequence::boneNames);
			usertypeDefinition["controllers"] = sol::readonly_property(&NI::Sequence::controllers);
			usertypeDefinition["endPointTime"] = sol::readonly_property(&NI::Sequence::endPointTime);
			usertypeDefinition["filename"] = sol::readonly_property(&NI::Sequence::filename);
			usertypeDefinition["fileNum"] = sol::readonly_property(&NI::Sequence::fileNum);
			usertypeDefinition["isCumulative"] = sol::readonly_property(&NI::Sequence::bCumulative);
			usertypeDefinition["isFirstFrame"] = sol::readonly_property(&NI::Sequence::bFirstFrame);
			usertypeDefinition["manager"] = sol::readonly_property(&NI::Sequence::manager);
			usertypeDefinition["name"] = sol::readonly_property(&NI::Sequence::name);
			usertypeDefinition["lastTime"] = sol::readonly_property(&NI::Sequence::lastTime);
			usertypeDefinition["offset"] = sol::readonly_property(&NI::Sequence::offset);
			usertypeDefinition["partnerSequence"] = sol::readonly_property(&NI::Sequence::partnerSequence);
			usertypeDefinition["state"] = sol::readonly_property(&NI::Sequence::state);
			usertypeDefinition["textKeyControllerIndex"] = sol::readonly_property(&NI::Sequence::textKeyControllerIndex);
			usertypeDefinition["textKeys"] = sol::readonly_property(&NI::Sequence::textKeys);
		}

		// Bind NI::PathController
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::PathController>("niPathController");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::TimeController, NI::Object>();
			setUserdataForNITimeController(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["lastUsedPathIndex"] = sol::readonly_property(&NI::PathController::lastUsedPathIndex);
			usertypeDefinition["lastUsedPercentIndex"] = sol::readonly_property(&NI::PathController::lastUsedPercentIndex);
			usertypeDefinition["pathData"] = sol::readonly_property(&NI::PathController::pathData);
			usertypeDefinition["percentData"] = sol::readonly_property(&NI::PathController::percentData);
			usertypeDefinition["totalLength"] = sol::readonly_property(&NI::PathController::totalLength);
			usertypeDefinition["maxBankAngle"] = &NI::PathController::maxBankAngle;
			usertypeDefinition["smoothing"] = &NI::PathController::smoothing;
			usertypeDefinition["followAxis"] = &NI::PathController::followAxis;
			usertypeDefinition["bankDirection"] = &NI::PathController::bankDirection;
			usertypeDefinition["bank_related_data"] = &NI::PathController::bank_related_data;

			// Functions exposed as properties.
			usertypeDefinition["allowFlip"] = sol::property(&NI::PathController::getAllowFlip, &NI::PathController::setAllowFlip);
			usertypeDefinition["bank"] = sol::property(&NI::PathController::getBank, &NI::PathController::setBank);
			usertypeDefinition["constantVelocity"] = sol::property(&NI::PathController::getConstantVelocity, &NI::PathController::setConstantVelocity);
			usertypeDefinition["flipFollowAxis"] = sol::property(&NI::PathController::getFlipFollowAxis, &NI::PathController::setFlipFollowAxis);
			usertypeDefinition["follow"] = sol::property(&NI::PathController::getFollow, &NI::PathController::setFollow);
			usertypeDefinition["openCurve"] = sol::property(&NI::PathController::getOpenCurve, &NI::PathController::setOpenCurve);
		}
	}
}
