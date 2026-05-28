#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

#include "TES3Defines.h"

#include "NITransform.h"

namespace mwse::lua::event {
	class CameraControlEvent : public GenericEvent, public DisableableEvent<CameraControlEvent> {
	public:
		CameraControlEvent(TES3::PlayerAnimationController* animController,
			const NI::Transform& prevCameraTransform,
			const NI::Transform& prevArmCameraTransform,
			const NI::Transform& cameraTransform,
			const NI::Transform& armCameraTransform);
		sol::table createEventTable();

	protected:
		TES3::PlayerAnimationController* m_AnimationController;
		NI::Transform m_PrevCameraTransform;
		NI::Transform m_PrevArmCameraTransform;
		NI::Transform m_CameraTransform;
		NI::Transform m_ArmCameraTransform;
	};
}
