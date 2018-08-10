#pragma once

#include "TES3Util.h"

#include "Log.h"

#include "sol.hpp"

namespace NI {
	template <class T>
	class Pointer {
	public:
		Pointer(T * pointer) {
			reinterpret_cast<void(__thiscall*)(Pointer<T>*, T*)>(0x40A840)(this, pointer);
		}

		~Pointer() {
			reinterpret_cast<void(__thiscall*)(Pointer<T>*)>(0x40A880)(this);
		}

		operator T*() const {
			return m_Pointer;
		}

		T * operator->() const {
			return m_Pointer;
		}

		bool operator==(T * pointer) const {
			return (m_Pointer == pointer);
		}

		bool operator!=(T * pointer) const {
			return (m_Pointer != pointer);
		}

		T * get() {
			return m_Pointer;
		}

	private:
		T * m_Pointer;
	};
	static_assert(sizeof(Pointer<int>) == 0x4, "NI::Pointer failed size validation");
}

namespace sol {
	template <typename T>
	struct unique_usertype_traits<NI::Pointer<T>> {
		typedef T type;
		typedef NI::Pointer<T> actual_type;
		static const bool value = true;

		static bool is_null(const actual_type& value) {
			return value == nullptr;
		}

		static type* get(const actual_type& p) {
			return p;
		}
	};
}
