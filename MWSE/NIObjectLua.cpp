#include "NIObjectLua.h"

#include "sol.hpp"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIDefines.h"
#include "NIAVObject.h"
#include "NINode.h"
#include "NIObject.h"
#include "NIObjectNET.h"
#include "NIPointer.h"
#include "NIRTTI.h"

namespace mwse {
	namespace lua {
		void bindNIObject() {
			// Get our lua state.
			sol::state& state = LuaManager::getInstance().getState();

			// Binding for NI::RTTI.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.create_simple_usertype<NI::RTTI>();
				usertypeDefinition.set("new", sol::no_constructor);

				// Basic property binding.
				usertypeDefinition.set("name", &NI::RTTI::name);
				usertypeDefinition.set("parent", &NI::RTTI::baseRTTI);

				// Finish up our usertype.
				state.set_usertype("niRTTI", usertypeDefinition);
			}

			// Binding for NI::Object.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.create_simple_usertype<NI::Object>();
				usertypeDefinition.set("new", sol::no_constructor);

				// Basic property binding.
				usertypeDefinition.set("references", sol::readonly_property(&NI::Object::references));

				// Basic function binding.
				usertypeDefinition.set("clone", [](NI::Object& self) { return makeLuaObject(self.createClone()); });
				usertypeDefinition.set("isOfType", static_cast<bool (NI::Object::*)(uintptr_t)>(&NI::Object::isOfType));
				usertypeDefinition.set("isInstanceOfType", static_cast<bool (NI::Object::*)(uintptr_t)>(&NI::Object::isInstanceOfType));

				// Functions exposed as properties.
				usertypeDefinition.set("RTTI", sol::readonly_property([](NI::Object& self) { return self.getRunTimeTypeInformation(); }));
				usertypeDefinition.set("runTimeTypeInformation", sol::readonly_property([](NI::Object& self) { return self.getRunTimeTypeInformation(); }));

				// Finish up our usertype.
				state.set_usertype("niObject", usertypeDefinition);
			}

			// Binding for NI::ObjectNET.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.create_simple_usertype<NI::ObjectNET>();
				usertypeDefinition.set("new", sol::no_constructor);

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition.set(sol::base_classes, sol::bases<NI::Object>());

				// Basic property binding.
				usertypeDefinition.set("name", sol::readonly_property([](NI::ObjectNET& self) { return self.name; }));

				// Finish up our usertype.
				state.set_usertype("niObjectNET", usertypeDefinition);
			}

			// Binding for NI::AVObject.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.create_simple_usertype<NI::AVObject>();
				usertypeDefinition.set("new", sol::no_constructor);

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition.set(sol::base_classes, sol::bases<NI::ObjectNET, NI::Object>());

				// Basic property binding.
				usertypeDefinition.set("flags", &NI::AVObject::flags);
				usertypeDefinition.set("localRotation", &NI::AVObject::localRotation);
				usertypeDefinition.set("localScale", &NI::AVObject::localScale);
				usertypeDefinition.set("localTranslate", &NI::AVObject::localTranslate);
				usertypeDefinition.set("worldBoundOrigin", &NI::AVObject::worldBoundOrigin);
				usertypeDefinition.set("worldBoundRadius", &NI::AVObject::worldBoundRadius);
				usertypeDefinition.set("worldTransform", &NI::AVObject::worldTransform);

				// Friendly access to flags.
				usertypeDefinition.set("appCulled", sol::property(
					[](NI::AVObject& self) -> bool { return (self.flags & 1) == 1; },
					[](NI::AVObject& self, bool set) { set ? self.flags |= 1 : self.flags &= ~1; }
				));

				// Access to other objects that need to be packaged.
				usertypeDefinition.set("parent", sol::readonly_property([](NI::AVObject& self) { return makeLuaObject(self.parentNode); }));

				// Basic function binding.
				usertypeDefinition.set("getObjectByName", [](NI::AVObject& self, const char* name) { return makeLuaObject(self.getObjectByName(name)); });

				// Finish up our usertype.
				state.set_usertype("niAVObject", usertypeDefinition);
			}
		}
	}
}
