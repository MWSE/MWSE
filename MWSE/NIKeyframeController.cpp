#include "NIKeyframeController.h"

namespace NI {
	//
	// KeyframeData
	//

	constexpr float findKeyEpsilon = 0.01f;

	sol::object KeyframeData::getRotationKeys_lua(sol::this_state L) {
		// Make sure we're looking at the main state.
		L = sol::main_thread(L);

		switch (rotationType) {
		case AnimationKey::KeyType::NoInterp:
		case AnimationKey::KeyType::Linear:
			return sol::make_object(L, nonstd::span(rotationKeys.asRotKey, rotationKeyCount));
		case AnimationKey::KeyType::Bezier:
			return sol::make_object(L, nonstd::span(rotationKeys.asBezRotKey, rotationKeyCount));
		case AnimationKey::KeyType::TCB:
			return sol::make_object(L, nonstd::span(rotationKeys.asTCBRotKey, rotationKeyCount));
		case AnimationKey::KeyType::Euler:
			return sol::make_object(L, nonstd::span(rotationKeys.asEulerRotKey, rotationKeyCount));
		default:
			throw std::runtime_error("Invalid rotation content type found. Report to MWSE developers.");
		}
	}

	sol::object KeyframeData::getPositionKeys_lua(sol::this_state L) {
		// Make sure we're looking at the main state.
		L = sol::main_thread(L);

		switch (positionType) {
		case AnimationKey::KeyType::NoInterp:
		case AnimationKey::KeyType::Linear:
			return sol::make_object(L, nonstd::span(positionKeys.asPosKey, positionKeyCount));
		case AnimationKey::KeyType::Bezier:
			return sol::make_object(L, nonstd::span(positionKeys.asBezPosKey, positionKeyCount));
		case AnimationKey::KeyType::TCB:
			return sol::make_object(L, nonstd::span(positionKeys.asTCBPosKey, positionKeyCount));
		default:
			throw std::runtime_error("Invalid position content type found. Report to MWSE developers.");
		}
	}

	sol::object KeyframeData::getScaleKeys_lua(sol::this_state L) {
		// Make sure we're looking at the main state.
		L = sol::main_thread(L);

		switch (scaleType) {
		case AnimationKey::KeyType::NoInterp:
		case AnimationKey::KeyType::Linear:
			return sol::make_object(L, nonstd::span(scaleKeys.asFloatKey, scaleKeyCount));
		case AnimationKey::KeyType::Bezier:
			return sol::make_object(L, nonstd::span(scaleKeys.asBezFloatKey, scaleKeyCount));
		case AnimationKey::KeyType::TCB:
			return sol::make_object(L, nonstd::span(scaleKeys.asTCBFloatKey, scaleKeyCount));
		default:
			throw std::runtime_error("Invalid scale content type found. Report to MWSE developers.");
		}
	}

	sol::optional<int> KeyframeData::getRotationKeyIndex_lua(sol::this_state L, float time) {
		// Make sure we're looking at the main state.
		L = sol::main_thread(L);

		// Find closest key with timing <= time. Also matches keys within +-epsilon of time.
		sol::optional<int> lastKeyIndex;
		time += findKeyEpsilon;

		switch (scaleType) {
		case AnimationKey::KeyType::NoInterp:
		case AnimationKey::KeyType::Linear:
			for (int i = 0; i < rotationKeyCount; ++i) {
				if (rotationKeys.asRotKey[i].timing >= time) { break; }
				lastKeyIndex = i;
			}
			break;
		case AnimationKey::KeyType::Bezier:
			for (int i = 0; i < rotationKeyCount; ++i) {
				if (rotationKeys.asBezRotKey[i].timing >= time) { break; }
				lastKeyIndex = i;
			}
			break;
		case AnimationKey::KeyType::TCB:
			for (int i = 0; i < rotationKeyCount; ++i) {
				if (rotationKeys.asTCBRotKey[i].timing >= time) { break; }
				lastKeyIndex = i;
			}
			break;
		case AnimationKey::KeyType::Euler:
			for (int i = 0; i < rotationKeyCount; ++i) {
				if (rotationKeys.asEulerRotKey[i].timing >= time) { break; }
				lastKeyIndex = i;
			}
			break;
		default:
			throw std::runtime_error("Invalid rotation content type found. Report to MWSE developers.");
		}

		// Convert to lua index.
		if (lastKeyIndex) { ++lastKeyIndex.value(); }
		return lastKeyIndex;
	}

	sol::optional<int> KeyframeData::getPositionKeyIndex_lua(sol::this_state L, float time) {
		// Make sure we're looking at the main state.
		L = sol::main_thread(L);

		// Find closest key with timing <= time. Also matches keys within +-epsilon of time.
		sol::optional<int> lastKeyIndex;
		time += findKeyEpsilon;

		switch (positionType) {
		case AnimationKey::KeyType::NoInterp:
		case AnimationKey::KeyType::Linear:
			for (int i = 0; i < positionKeyCount; ++i) {
				if (positionKeys.asPosKey[i].timing >= time) { break; }
				lastKeyIndex = i;
			}
			break;
		case AnimationKey::KeyType::Bezier:
			for (int i = 0; i < positionKeyCount; ++i) {
				if (positionKeys.asBezPosKey[i].timing >= time) { break; }
				lastKeyIndex = i;
			}
			break;
		case AnimationKey::KeyType::TCB:
			for (int i = 0; i < positionKeyCount; ++i) {
				if (positionKeys.asTCBPosKey[i].timing >= time) { break; }
				lastKeyIndex = i;
			}
			break;
		default:
			throw std::runtime_error("Invalid position content type found. Report to MWSE developers.");
		}

		// Convert to lua index.
		if (lastKeyIndex) { ++lastKeyIndex.value(); }
		return lastKeyIndex;
	}

	sol::optional<int> KeyframeData::getScaleKeyIndex_lua(sol::this_state L, float time) {
		// Make sure we're looking at the main state.
		L = sol::main_thread(L);

		// Find closest key with timing <= time. Also matches keys within +-epsilon of time.
		sol::optional<int> lastKeyIndex;
		time += findKeyEpsilon;

		switch (scaleType) {
		case AnimationKey::KeyType::NoInterp:
		case AnimationKey::KeyType::Linear:
			for (int i = 0; i < scaleKeyCount; ++i) {
				if (scaleKeys.asFloatKey[i].timing >= time) { break; }
				lastKeyIndex = i;
			}
			break;
		case AnimationKey::KeyType::Bezier:
			for (int i = 0; i < scaleKeyCount; ++i) {
				if (scaleKeys.asBezFloatKey[i].timing >= time) { break; }
				lastKeyIndex = i;
			}
			break;
		case AnimationKey::KeyType::TCB:
			for (int i = 0; i < scaleKeyCount; ++i) {
				if (scaleKeys.asTCBFloatKey[i].timing >= time) { break; }
				lastKeyIndex = i;
			}
			break;
		default:
			throw std::runtime_error("Invalid scale content type found. Report to MWSE developers.");
		}

		// Convert to lua index.
		if (lastKeyIndex) { ++lastKeyIndex.value(); }
		return lastKeyIndex;
	}

	void KeyframeData::updateDerivedValues() {
		if (positionKeys.asPosKey) {
			const auto fn = AnimationKey::getFillDerivedValuesFunction(AnimationKey::ContentType::Position, positionType);
			fn(positionKeys.asPosKey, positionKeyCount, positionType);
		}

		if (rotationKeys.asRotKey) {
			const auto fn = AnimationKey::getFillDerivedValuesFunction(AnimationKey::ContentType::Rotation, rotationType);
			fn(rotationKeys.asRotKey, rotationKeyCount, rotationType);
		}

		if (scaleKeys.asFloatKey) {
			const auto fn = AnimationKey::getFillDerivedValuesFunction(AnimationKey::ContentType::Float, scaleType);
			fn(scaleKeys.asFloatKey, scaleKeyCount, scaleType);
		}
	}

	const auto TES3_KeyframeData_replaceScaleData = reinterpret_cast<void(__thiscall*)(KeyframeData*,FloatKey*, unsigned int, AnimationKey::KeyType)>(0x70D420);
	void KeyframeData::replaceScaleData(FloatKey* keys, unsigned int numKeys, AnimationKey::KeyType keyType) {
		TES3_KeyframeData_replaceScaleData(this, keys, numKeys, keyType);
	}
}
