#include "NIAnimationKey.h"

namespace NI {
	template <typename T>
	struct AnimationKeyFunctions {
		struct InterpolationType {
			union {
				struct { T none; T linear; T bezier; T TCB; T euler; };
				T functions[5];
			};
			T& operator[](AnimationKey::KeyType type) {
				return functions[static_cast<unsigned int>(type)];
			}
		};
		union {
			struct {
				InterpolationType float_;
				InterpolationType position;
				InterpolationType rotation;
				InterpolationType color;
				InterpolationType text;
				InterpolationType vis;
			};
			InterpolationType types[6];
		};
		InterpolationType& operator[](AnimationKey::ContentType type) {
			return types[static_cast<unsigned int>(type)];
		}
	};
	static_assert(sizeof(AnimationKeyFunctions<AnimationKey::FillDerivedValuesFunction>) == 0x78, "NI::AnimationKeyFunctions failed size validation");

	AnimationKey::FillDerivedValuesFunction AnimationKey::getFillDerivedValuesFunction(AnimationKey::ContentType content, AnimationKey::KeyType key) {
		auto& fns = *reinterpret_cast<AnimationKeyFunctions<FillDerivedValuesFunction>*>(0x7DF868);
		return fns[content][key];
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	std::reference_wrapper<decltype(EulerRotKey::numKeys)> EulerRotKey::getNumKeys_lua() {
		return std::ref(numKeys);
	}

	std::reference_wrapper<decltype(EulerRotKey::keyTypes)> EulerRotKey::getKeyTypes_lua() {
		return std::ref(keyTypes);
	}

	sol::table EulerRotKey::getKeys_lua(sol::this_state ts) {
		sol::state_view state = ts;
		auto table = state.create_table(3);
		for (auto i = 0; i < 3; ++i) {
			if (keys[i].asFloatKey) {
				switch (keyTypes[i]) {
				case AnimationKey::KeyType::NoInterp:
				case AnimationKey::KeyType::Linear:
					table[i + 1] = nonstd::span(keys[i].asFloatKey, numKeys[i]);
					break;
				case AnimationKey::KeyType::Bezier:
					table[i + 1] = nonstd::span(keys[i].asBezFloatKey, numKeys[i]);
					break;
				case AnimationKey::KeyType::TCB:
					table[i + 1] = nonstd::span(keys[i].asTCBFloatKey, numKeys[i]);
					break;
				default:
					throw std::runtime_error("Invalid euler rotation key type found. Report to MWSE developers.");
				}
			}
		}
		return table;
	}

	std::reference_wrapper<decltype(EulerRotKey::lastIndices)> EulerRotKey::getLastIndices_lua() {
		return std::ref(lastIndices);
	}
#endif
}
