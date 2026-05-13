#include "LuaCameraControlEvent.h"

#include "LuaManager.h"

#include "TES3PlayerAnimationController.h"

namespace mwse::lua::event {
	CameraControlEvent::CameraControlEvent(TES3::PlayerAnimationController* animController,
		const NI::Transform& prevCameraTransform,
		const NI::Transform& prevArmCameraTransform,
		const NI::Transform& cameraTransform,
		const NI::Transform& armCameraTransform) :
		GenericEvent("cameraControl"),
		m_AnimationController(animController),
		m_PrevCameraTransform(prevCameraTransform),
		m_PrevArmCameraTransform(prevArmCameraTransform),
		m_CameraTransform(cameraTransform),
		m_ArmCameraTransform(armCameraTransform)
	{

	}

	sol::table CameraControlEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["animationController"] = m_AnimationController;
		eventData["previousCameraTransform"] = m_PrevCameraTransform;
		eventData["previousArmCameraTransform"] = m_PrevArmCameraTransform;
		eventData["cameraTransform"] = m_CameraTransform;
		eventData["armCameraTransform"] = m_ArmCameraTransform;

		return eventData;
	}
}
