#pragma once

#include "TES3Vectors.h"

namespace NI {
	struct PackedColor {
		unsigned char b; // 0x0
		unsigned char g; // 0x1
		unsigned char r; // 0x2
		unsigned char a; // 0x3

		PackedColor() : r(0), g(0), b(0), a(0) {};
		PackedColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 0) : r(_r), g(_g), b(_b), a(_a) {};

		std::string toString();
	};
	static_assert(sizeof(PackedColor) == 0x4, "NI::PackedColor failed size validation");

	struct Color {
		float r; // 0x0
		float g; // 0x4
		float b; // 0x8

		Color() : r(0.0f), g(0.0f), b(0.0f) {}
		Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b) {}
		Color(TES3::Vector3& vector);
		Color(sol::table table);
		Color(sol::object object);

		Color& operator=(const TES3::Vector3& vector);
		Color& operator=(const sol::table table);
		Color& operator=(const sol::object object);

		bool operator==(const Color& c);
		bool operator!=(const Color& c);
		Color operator+(const Color&);
		Color operator-(const Color&);
		Color operator*(const Color&);
		Color operator*(const float);

		//
		// Custom functions.
		//

		Color copy() const;
		Color lerp(const Color& to, float transition) const;
		TES3::Vector3 toVector3() const;

		void clamp();

		std::string toString() const;

	};
	static_assert(sizeof(Color) == 0xC, "NI::Color failed size validation");

	struct ColorA : Color {
		float a;

		ColorA() : Color(), a(0.0f) {}
		ColorA(float _r, float _g, float _b) : Color(_r, _g, _b), a(0.0f) {}
		ColorA(float _r, float _g, float _b, float _a) : Color(_r, _g, _b), a(_a) {}

		//
		// Custom functions.
		//

		ColorA copy() const;
		ColorA lerp(const ColorA& to, float transition) const;

		std::string toString() const;

	};
	static_assert(sizeof(ColorA) == 0x10, "NI::ColorA failed size validation");
}
