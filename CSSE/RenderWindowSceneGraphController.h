#pragma once

#include "NINode.h"

import NIProperty;

namespace se::cs::dialog::render_window {
	struct SceneGraphControllerVanilla {
		NI::Pointer<NI::Node> sceneRoot; // 0x0
		NI::Pointer<NI::Node> objectRoot; // 0x4
		NI::Pointer<NI::Node> landscapeRoot; // 0x8
		NI::Pointer<NI::ZBufferProperty> zBufferProperty; // 0xC
		NI::Pointer<NI::Property> wireframeProperty; // 0x10
		NI::Pick* objectPick; // 0x14
		NI::Pick* landscapePick; // 0x18
		NI::Pointer<NI::Light> directionalLight; // 0x1C
		NI::Pointer<NI::FogProperty> fog; // 0x20
	};
	static_assert(sizeof(SceneGraphControllerVanilla) == 0x24, "CS::SceneGraphController failed size validation");

	struct WidgetsController;

	struct SceneGraphController : SceneGraphControllerVanilla {
	private:
		WidgetsController* widgets; // 0x24

	public:
		void clearWidgets();
		WidgetsController* getWidgets();

		static bool __cdecl initialize(SceneGraphController* controller);

		static inline auto get() {
			return memory::ExternalGlobal<SceneGraphController*, 0x6CEB78>::get();
		}
	};
}
