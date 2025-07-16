#include "TES3AnimationGroupLua.h"

#include "LuaManager.h"

#include "TES3AnimationGroup.h"
#include "TES3Sound.h"

namespace mwse::lua {
	void bindTES3AnimationGroup() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::AnimationGroup::SoundGenKey>("tes3animationGroupSoundGenKey");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property bindings.
			usertypeDefinition["pitch"] = &TES3::AnimationGroup::SoundGenKey::pitch;
			usertypeDefinition["sound"] = &TES3::AnimationGroup::SoundGenKey::sound;
			usertypeDefinition["startFrame"] = &TES3::AnimationGroup::SoundGenKey::startFrame;
			usertypeDefinition["startTime"] = &TES3::AnimationGroup::SoundGenKey::startTime;
			usertypeDefinition["volume"] = &TES3::AnimationGroup::SoundGenKey::volume;
		}

		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::AnimationGroup>("tes3animationGroup");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property bindings.
			usertypeDefinition["actionCount"] = sol::readonly_property(&TES3::AnimationGroup::actionCount);
			usertypeDefinition["actionFrames"] = sol::readonly_property(&TES3::AnimationGroup::getActionFrames);
			usertypeDefinition["actionTimings"] = sol::readonly_property(&TES3::AnimationGroup::getActionTimings);
			usertypeDefinition["groupId"] = sol::readonly_property(&TES3::AnimationGroup::groupId);
			usertypeDefinition["nextGroup"] = sol::readonly_property(&TES3::AnimationGroup::nextGroup);
			usertypeDefinition["soundGenCount"] = sol::readonly_property(&TES3::AnimationGroup::soundGenCount);
			usertypeDefinition["soundGenKeys"] = sol::readonly_property(&TES3::AnimationGroup::getSoundGenKeys);
		}
	}
}
