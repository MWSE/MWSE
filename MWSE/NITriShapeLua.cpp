#include "NISwitchNode.h"

#include "NIObjectLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIDefines.h"
#include "NIRTTI.h"
#include "NITriShape.h"
#include "NiTriShapeData.h"

namespace mwse {
	namespace lua {
		void bindNITriShape() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Binding for NI::TriShape.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<NI::TriShape>("niTriShape");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::TriBasedGeometry, NI::Geometry, NI::AVObject, NI::ObjectNET, NI::Object>();
				setUserdataForNIAVObject(usertypeDefinition);

				// Basic property binding.
				usertypeDefinition["data"] = sol::property(&NI::TriShape::getModelData, &NI::TriShape::setModelData);
				usertypeDefinition["skinInstance"] = &NI::TriShape::skinInstance;

				// Lazy access to geometry data. Don't encourage this.
				usertypeDefinition["normals"] = sol::readonly_property(&NI::TriShape::getNormals);
				usertypeDefinition["vertices"] = sol::readonly_property(&NI::TriShape::getVertices);

				// Basic function binding.
				usertypeDefinition["createBoundingBox"] = &NI::TriShape::createBoundingBox_lua;
			}
		}
	}
}
