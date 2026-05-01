#pragma once

#include "TES3Weather.h"

namespace TES3 {
	struct WeatherCustom : Weather {
		std::string name;
		sol::optional<int> overrideId;
		sol::protected_function simulateFunction;
		sol::protected_function transitionFunction;
		sol::protected_function unloadFunction;
		sol::table data;
		bool isStormy;

		static Weather_vTable VirtualTable;

		WeatherCustom();
		WeatherCustom(WeatherController* wc);
		~WeatherCustom();

		void vtbl_delete(bool del);
		void vtbl_simulate(float transitionScalar, float deltaTime);
		void vtbl_transition();
		void vtbl_unload();
	};
}
