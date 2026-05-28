#pragma once

namespace TES3 {
	struct CriticalSection : _RTL_CRITICAL_SECTION {
		int unknown_0x18;
		int unknown_0x1C;
		int unknown_0x20;

		CriticalSection() = delete;
		~CriticalSection() = delete;

		//
		// Other related this-call functions.
		//

		void enter(const char* id = "MWSE:Undefined");
		void leave();

		// RAII scope guard. Pair an enter() with a guaranteed leave() at scope
		// exit; non-copyable, non-movable so the lifetime is unambiguous.
		class Lock {
			CriticalSection& cs;
		public:
			explicit Lock(CriticalSection& cs, const char* id = "MWSE:Undefined") : cs(cs) {
				cs.enter(id);
			}
			~Lock() { cs.leave(); }
			Lock(const Lock&) = delete;
			Lock& operator=(const Lock&) = delete;
		};
	};
	static_assert(sizeof(CriticalSection) == 0x24, "TES3::CriticalSection failed size validation");
}
