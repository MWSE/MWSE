#pragma once

#include "TES3Reference.h"

namespace TES3 {
	template <typename T>
	class Allocator {
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T value_type;

		template <class U>
		struct rebind { typedef Allocator<U> o; };

		static constexpr auto tes3new = reinterpret_cast<T* (__cdecl*)(size_t)>(0x727692);
		static constexpr auto tes3delete = reinterpret_cast<void(__cdecl*)(T*)>(0x727530);

		Allocator() {}
		Allocator(const Allocator& o) {}
		template <class U> Allocator(const Allocator<U>& o) {}
		~Allocator() {}

		template <class U>
		Allocator& operator=(const Allocator<U>&) { return *this; }

		pointer address(reference r) const { return &r; }
		const_pointer address(const_reference r) const { return &r; }

		template <class U> pointer allocate(size_type n, Allocator<U>::const_pointer hint = 0) { return tes3new(n * sizeof(T)); }
		void deallocate(pointer p) { if (p) tes3delete(p); }

		void construct(pointer p, const_reference cr) { p->T(cr); }
		void destroy(pointer p) { p->~T(); }

		size_type max_size() { return -1; }
	};
}
