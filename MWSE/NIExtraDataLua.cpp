#include "NIExtraDataLua.h"

#include "LuaManager.h"

#include "NIRTTI.h"
#include "NIExtraData.h"

namespace mwse::lua {
	void bindNIExtraData() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Binding for NI::ExtraData.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::ExtraData>("niExtraData");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::Object>();
			setUserdataForNIExtraData(usertypeDefinition);
		}

		// Binding for NI::StringExtraData.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::StringExtraData>("niStringExtraData");
			usertypeDefinition["new"] = sol::constructors<NI::StringExtraData(const char*)>();

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::ExtraData, NI::Object>();
			setUserdataForNIExtraData(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["string"] = sol::property(&NI::StringExtraData::getString, &NI::StringExtraData::setString);
		}

		// Binding for NI::Tes3ExtraData.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::Tes3ExtraData>("niTES3ExtraData");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::ExtraData, NI::Object>();
			setUserdataForNIExtraData(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["reference"] = &NI::Tes3ExtraData::reference;
		}

		// Binding for NI::TextKeyExtraData.
		{
			// Binding for NI::TextKey.
			{
				// Start our usertype.
				auto usertypeDefinition = state.new_usertype<NI::TextKey>("niTextKey");
				usertypeDefinition["new"] = sol::no_constructor;

				// Basic property binding.
				usertypeDefinition["text"] = sol::property(&NI::TextKey::getText, &NI::TextKey::setText);
				usertypeDefinition["time"] = &NI::TextKey::time;
			}

			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::TextKeyExtraData>("niTextKeyExtraData");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::ExtraData, NI::Object>();
			setUserdataForNIExtraData(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["keys"] = sol::readonly_property(&NI::TextKeyExtraData::getKeys);
		}
	}
}
