#include "NIColor.h"

namespace NI {
	// PackedColor's MWSE-canonical ctors (default + 4-byte) are inline in
	// NIColor.h. The float-based and std::array convenience ctors below are
	// SharedSE-only -- used by CSSE rendering code (DialogRenderWindow,
	// RenderWindowWidgets). They take 0.0..1.0 floats and scale to byte range.

	PackedColor::PackedColor(float _r, float _g, float _b, float _a) {
		r = unsigned char(255.0f * _r);
		g = unsigned char(255.0f * _g);
		b = unsigned char(255.0f * _b);
		a = unsigned char(255.0f * _a);
	}

	PackedColor::PackedColor(const std::array<unsigned char, 3>& from)
		: PackedColor(from[0], from[1], from[2]) {
	}

	PackedColor::PackedColor(const std::array<float, 3>& from)
		: PackedColor(from[0], from[1], from[2]) {
	}

}
