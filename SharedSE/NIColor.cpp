#include "NIColor.h"

namespace NI {
	PackedColor::PackedColor() {
		r = 0;
		g = 0;
		b = 0;
		a = 255;
	}

	PackedColor::PackedColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a) {
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	PackedColor::PackedColor(float _r, float _g, float _b, float _a) {
		r = unsigned char(255.0f * _r);
		g = unsigned char(255.0f * _g);
		b = unsigned char(255.0f * _b);
		a = unsigned char(255.0f * _a);
	}

	PackedColor::PackedColor(const std::array<unsigned char, 3>& from) : PackedColor(from[0], from[1], from[2]) {

	}

	PackedColor::PackedColor(const std::array<float, 3>& from) : PackedColor(from[0], from[1], from[2]) {

	}

	Color::Color(float _r, float _g, float _b) :
		r(_r),
		g(_g),
		b(_b)
	{

	}
}
