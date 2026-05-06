#include "NITriangle.h"

namespace NI {
	Triangle::Triangle() : Triangle(0, 0, 0) {

	}

	Triangle::Triangle(unsigned short first, unsigned short second, unsigned short third) {
		vertices[0] = first;
		vertices[1] = second;
		vertices[2] = third;
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	Triangle::Triangle(const sol::table table) {
		*this = table;
	}

	Triangle& Triangle::operator=(const sol::table table) {
		for (auto i = 0; i < 3; ++i) {
			vertices[i] = table.get_or(i + 1, 0);
		}
		return *this;
	}
#endif

	std::reference_wrapper<unsigned short[3]> Triangle::getVertices() {
		return std::ref(vertices);
	}
}
