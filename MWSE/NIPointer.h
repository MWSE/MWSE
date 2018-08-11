#pragma once

#include "TES3Util.h"

#include "Log.h"

#include "sol.hpp"

namespace NI {
	template <class T>
	class Pointer {
	public:
		Pointer(T * pointer = nullptr) {
			claim(pointer);
		}

		Pointer(const Pointer<T>& pointer) {
			claim(pointer);
		}

		~Pointer() {
			release();
		}

		bool operator==(T* pointer) const {
			return (pointer == m_Pointer);
		}

		bool operator!=(T* pointer) const {
			return (pointer != m_Pointer);
		}

		bool operator==(const Pointer<T>& pointer) const {
			return (pointer == m_Pointer);
		}

		bool operator!=(const Pointer<T>& pointer) const {
			return (pointer != m_Pointer);
		}

		operator T*() const {
			return m_Pointer;
		}

		T* operator->() const {
			return m_Pointer;
		}

		Pointer<T>& operator=(const Pointer<T>& pointer) {
			if (m_Pointer != pointer) {
				claim(pointer);
			}
			return *this;
		}

		Pointer<T>& operator=(T* pointer) {
			if (m_Pointer != pointer) {
				claim(pointer);
			}
			return *this;
		}

		T * get() {
			return m_Pointer;
		}

	private:
		T * m_Pointer = nullptr;

		void release() {
			if (m_Pointer) {
				if (--m_Pointer->references == 0) {
					m_Pointer->vTable.asObject->destructor(m_Pointer, 1);
					m_Pointer = nullptr;
				}
			}
		}

		void claim(T * pointer) {
			release();

			m_Pointer = pointer;
			if (m_Pointer) {
				m_Pointer->references++;
			}
		}
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
