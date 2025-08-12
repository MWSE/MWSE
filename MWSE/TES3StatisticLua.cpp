#include "TES3StatisticLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Statistic.h"

namespace mwse::lua {
	void bindTES3Statistic() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Statistic>("tes3statistic");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["base"] = sol::property(&TES3::Statistic::getBase, &TES3::Statistic::setBase);
			usertypeDefinition["baseRaw"] = &TES3::Statistic::base;
			usertypeDefinition["current"] = sol::property(&TES3::Statistic::getCurrent, &TES3::Statistic::setCurrent_lua);
			usertypeDefinition["currentRaw"] = &TES3::Statistic::current;
			usertypeDefinition["normalized"] = sol::property(&TES3::Statistic::getNormalized);
		}

		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::SkillStatistic>("tes3statisticSkill");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::Statistic>();

			// Basic property binding.
			usertypeDefinition["base"] = sol::property(&TES3::SkillStatistic::getBase, &TES3::SkillStatistic::setBase);
			usertypeDefinition["baseRaw"] = &TES3::SkillStatistic::base;
			usertypeDefinition["current"] = sol::property(&TES3::SkillStatistic::getCurrent, &TES3::SkillStatistic::setCurrent_lua);
			usertypeDefinition["currentRaw"] = &TES3::SkillStatistic::current;
			usertypeDefinition["normalized"] = sol::property(&TES3::SkillStatistic::getNormalized);
			usertypeDefinition["type"] = &TES3::SkillStatistic::type;
		}
	}
}


