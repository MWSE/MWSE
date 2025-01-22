#pragma once

namespace mwse::lua::event {
	template <typename T>
	class DisableableEvent {
	public:
		inline static bool getEventEnabled() { return m_EventEnabled; }
		inline static void setEventEnabled(bool enabled) { m_EventEnabled = enabled; }

	protected:
		inline static bool m_EventEnabled = false;
	};
}
