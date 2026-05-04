#pragma once

#include "NIDefines.h"

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
// MWSE-side Color uses Vector3 (which is bridged to TES3::Vector3) in some
// signatures. Pull in the bridged primitive header so the typedef alias is
// in scope when MWSE-private callers consume this file after the redirect.
#include "NIVector3.h"
#endif

namespace NI {
	struct PackedColor {
		unsigned char b; // 0x0
		unsigned char g; // 0x1
		unsigned char r; // 0x2
		unsigned char a; // 0x3

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		// MWSE-original uses inline ctors with a=0 default. Implementations
		// inline -- no MWSE/NIColor.cpp counterpart for these constructors.
		PackedColor() : r(0), g(0), b(0), a(0) {}
		PackedColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 0)
			: r(_r), g(_g), b(_b), a(_a) {}

		// MWSE-only methods; impls in MWSE-private NIColor.cpp.
		PackedColor operator*(float scalar) const;
		std::string toString() const;
#else
		// SharedSE / CSSE: split decl/impl, a=255 default (opaque).
		// Impls live in SharedSE/NIColor.cpp.
		PackedColor();
		PackedColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
		PackedColor(float r, float g, float b, float a = 1.0f);
		PackedColor(const std::array<unsigned char, 3>& from);
		PackedColor(const std::array<float, 3>& from);
#endif
	};
	static_assert(sizeof(PackedColor) == 0x4, "NI::PackedColor failed size validation");

	struct Color {
		float r; // 0x0
		float g; // 0x4
		float b; // 0x8

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		// MWSE-original: inline default ctor.
		Color() : r(0.0f), g(0.0f), b(0.0f) {}
#endif
		Color(float _r, float _g, float _b);

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		// MWSE-only ctors and assignment ops. Impls in MWSE-private NIColor.cpp.
		Color(const ColorA& c);
		Color(const Vector3& vector);  // Vector3 = TES3::Vector3 under bridge.

		Color& operator=(const Vector3& vector);
#endif
#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		Color(sol::table table);
		Color(const sol::object& object);

		Color& operator=(const sol::table table);
		Color& operator=(const sol::object& object);
#endif

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
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
#endif
	};
	static_assert(sizeof(Color) == 0xC, "NI::Color failed size validation");

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
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
#endif
}
