#pragma once

#include <array>

#include "NIDefines.h"
#include "NIVector3.h"

namespace NI {
	struct PackedColor {
		unsigned char b; // 0x0
		unsigned char g; // 0x1
		unsigned char r; // 0x2
		unsigned char a; // 0x3

		// MWSE source of truth: inline ctors with a=0 default (matches the
		// engine memory layout; verified against IDA disassembly of
		// Morrowind.exe). SharedSE previously had a=255 default which was
		// incorrect.
		PackedColor() : r(0), g(0), b(0), a(0) {}
		PackedColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 0)
			: r(_r), g(_g), b(_b), a(_a) {}

		// SharedSE-only convenience ctors (used by CSSE rendering code).
		// Float values are in 0.0..1.0 range; alpha defaults to 1.0f (opaque).
		// Impls in SharedSE/NIColor.cpp.
		PackedColor(float r, float g, float b, float a = 1.0f);
		PackedColor(const std::array<unsigned char, 3>& from);
		PackedColor(const std::array<float, 3>& from);

		// Pure-C++ methods (impls in MWSE-private NIColor.cpp; CSSE doesn't call them).
		PackedColor operator*(float scalar) const;
		std::string toString() const;
	};
	static_assert(sizeof(PackedColor) == 0x4, "NI::PackedColor failed size validation");

	struct Color {
		float r; // 0x0
		float g; // 0x4
		float b; // 0x8

		Color() : r(0.0f), g(0.0f), b(0.0f) {}
		Color(float _r, float _g, float _b);
		Color(const ColorA& c);
		Color(const Vector3& vector);
#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		Color(sol::table table);
		Color(const sol::object& object);
#endif

		Color& operator=(const Vector3& vector);
#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		Color& operator=(const sol::table table);
		Color& operator=(const sol::object& object);
#endif

		bool operator==(const Color&) const;
		bool operator!=(const Color&) const;
		Color operator+(const Color&) const;
		Color operator-(const Color&) const;
		Color operator*(const Color&) const;
		Color operator*(const float) const;

		//
		// Custom functions.
		//

		Color copy() const;
		Color lerp(const Color& to, float transition) const;
		Vector3 toVector3() const;

		void clamp();

		std::string toString() const;
		std::string toJson() const;
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
