#include "NIPickLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIUtil.h"
#include "NIDefines.h"
#include "NIGeometry.h"
#include "NINode.h"
#include "NIPick.h"
#include "NIRTTI.h"

#include "TES3Defines.h"
#include "TES3Reference.h"

namespace mwse::lua {
	void bindNIPick() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		state.new_usertype<NI::Pick>("niPick",
			// Disable construction of this type.
			"new", sol::no_constructor,

			//
			// Properties.
			//

			"pickType", &NI::Pick::pickType,
			"sortType", &NI::Pick::sortType,
			"intersectType", &NI::Pick::intersectType,
			"coordinateType", &NI::Pick::coordinateType,
			"frontOnly", &NI::Pick::frontOnly,
			"observeAppCullFlag", &NI::Pick::observeAppCullFlag,
			"root", &NI::Pick::root,
			"results", sol::readonly_property(&NI::Pick::results),
			"lastAddedRecord", &NI::Pick::lastAddedRecord,
			"returnTexture", &NI::Pick::returnTexture,
			"returnNormal", &NI::Pick::returnNormal,
			"returnSmoothNormal", &NI::Pick::returnSmoothNormal,
			"returnColor", &NI::Pick::returnColor,

			//
			// Methods.
			//

			"pickObjects", &NI::Pick::pickObjects,
			"clearResults", &NI::Pick::clearResults

			);

		state.new_usertype<NI::PickRecord>("niPickRecord",
			// Disable construction of this type.
			"new", sol::no_constructor,

			//
			// Properties.
			//

			"object", sol::readonly_property(&NI::PickRecord::object),
			"parent", sol::readonly_property(&NI::PickRecord::proxyParent),
			"intersection", &NI::PickRecord::intersection,
			"distance", &NI::PickRecord::distance,
			"triangleIndex", &NI::PickRecord::triangleIndex,
			"vertexIndex", sol::property(&NI::PickRecord::getVertexIndex),
			"texture", &NI::PickRecord::texture,
			"normal", &NI::PickRecord::normal,
			"color", &NI::PickRecord::color,

			// Provide quick reference to the associated object's reference.
			"reference", sol::readonly_property(&NI::PickRecord::getTES3Reference)

			);
	}
}
