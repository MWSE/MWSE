#include "TES3StaticLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3Static.h"
#include "LuaObject.h"

namespace mwse {
	namespace lua {
		auto createStatic( sol::table params )
		{
			return makeObjectCreator( TES3::ObjectType::Static )->create( params, false );
		}
		
		void bindTES3Static() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Static>("tes3static");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
			setUserdataForTES3PhysicalObject(usertypeDefinition);

			// Functions exposed as properties.
			usertypeDefinition["mesh"] = sol::property(&TES3::Static::getModelPath, &TES3::Static::setModelPath);

			// TODO: Deprecated. Remove before 2.1-stable.
			usertypeDefinition["model"] = sol::property(&TES3::Static::getModelPath, &TES3::Static::setModelPath);

			// utility function bindings
			usertypeDefinition["create"] = &createStatic;
		}
	}
}
