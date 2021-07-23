#pragma once

namespace NI {
	template <class T>
	struct Rect {
		T left;
		T right;
		T top;
		T bottom;

		Rect(T l = T(0), T r = T(0), T t = T(0), T b = T(0)) {
			left = l;
			right = r;
			top = t;
			bottom = b;
		}
	};
	static_assert(sizeof(Rect<unsigned int>) == 0x10, "NI::Rect failed size validation");
}
