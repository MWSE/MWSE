#pragma once

namespace mwse::lua::event {
	namespace detail {
		template <typename TEvent>
		class ScopedEventState {
		public:
			ScopedEventState() :
				m_EventEnabled(TEvent::getEventEnabled())
			{
				TEvent::setEventEnabled(false);
			}

			~ScopedEventState() {
				TEvent::setEventEnabled(m_EventEnabled);
			}

		private:
			bool m_EventEnabled;
		};
	}

	template <typename... TEvents>
	class ScopedEventSuppressor : private detail::ScopedEventState<TEvents>... {
	public:
		static_assert(sizeof...(TEvents) > 0, "ScopedEventSuppressor requires at least one event type.");

		ScopedEventSuppressor() = default;
		~ScopedEventSuppressor() = default;

		ScopedEventSuppressor(const ScopedEventSuppressor&) = delete;
		ScopedEventSuppressor& operator=(const ScopedEventSuppressor&) = delete;
	};
}
