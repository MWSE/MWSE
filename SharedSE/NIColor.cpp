#include "NIColor.h"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace NI {
	//
	// NiPackedColor
	//

	// SharedSE-only float-based and std::array convenience ctors. Used by CSSE
	// rendering code (DialogRenderWindow, RenderWindowWidgets); take 0.0..1.0
	// floats and scale to byte range.

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

	PackedColor PackedColor::operator*(float scalar) const {
		const auto _r = unsigned char(float(r) * scalar);
		const auto _g = unsigned char(float(g) * scalar);
		const auto _b = unsigned char(float(b) * scalar);
		const auto _a = unsigned char(float(a) * scalar);
		return { _b, _g, _r, _a };
	}

	std::string PackedColor::toString() const {
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(2) << "<" << int(r) << ", " << int(g) << ", " << int(b) << ", " << int(a) << ">";
		return std::move(ss.str());
	}

	//
	// NiColor
	//

	Color::Color(const Vector3& vector) {
		r = vector.x;
		g = vector.y;
		b = vector.z;
	}

	Color::Color(const ColorA& c) {
		r = c.r;
		g = c.g;
		b = c.b;
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	Color::Color(sol::table table) {
		r = table.get_or("r", table.get_or(1, 0.0f));
		g = table.get_or("g", table.get_or(2, 0.0f));
		b = table.get_or("b", table.get_or(3, 0.0f));
	}

	Color::Color(const sol::object& object) {
		if (object.is<Color>()) {
			*this = Color(object.as<Color>());
		}
		else if (object.is<ColorA>()) {
			*this = Color(object.as<ColorA>());
		}
		else if (object.is<Vector3>()) {
			*this = Color(object.as<Vector3>());
		}
		else if (object.is<sol::table>()) {
			*this = Color(object.as<sol::table>());
		}
		else {
			throw std::invalid_argument("Could not convert lua object to NiColor.");
		}
	}
#endif

	Color& Color::operator=(const Vector3& vector) {
		r = vector.x;
		g = vector.y;
		b = vector.z;
		return *this;
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	Color& Color::operator=(const sol::table table) {
		r = table.get_or("r", table.get_or(1, 0.0f));
		g = table.get_or("g", table.get_or(2, 0.0f));
		b = table.get_or("b", table.get_or(3, 0.0f));
		return *this;
	}

	Color& Color::operator=(const sol::object& object) {
		if (object.is<Color>()) {
			*this = Color(object.as<Color>());
		}
		else if (object.is<ColorA>()) {
			*this = Color(object.as<ColorA>());
		}
		else if (object.is<Vector3>()) {
			*this = Color(object.as<Vector3>());
		}
		else if (object.is<sol::table>()) {
			*this = Color(object.as<sol::table>());
		}
		else {
			throw std::invalid_argument("Could not convert lua object to NiColor.");
		}
		return *this;
	}
#endif

	bool Color::operator==(const Color& c) const {
		return r == c.r && g == c.g && b == c.b;
	}

	bool Color::operator!=(const Color& c) const {
		return r != c.r || g != c.g || b != c.b;
	}

	Color Color::operator+(const Color& c) const {
		return Color(r + c.r, g + c.g, b + c.b);
	}

	Color Color::operator-(const Color& c) const {
		return Color(r - c.r, g - c.g, b - c.b);
	}

	Color Color::operator*(const Color& c) const {
		return Color(r * c.r, g * c.g, b * c.b);
	}

	Color Color::operator*(const float scalar) const {
		return Color(r * scalar, g * scalar, b * scalar);
	}

	Color Color::copy() const {
		return *this;
	}

	Color Color::lerp(const Color& to, float transition) const {
		auto transA = 1.0f - transition;
		return Color(r * transA + to.r * transition, g * transA + to.g * transition, b * transA + to.b * transition);
	}

	Vector3 Color::toVector3() const {
		return Vector3(r, g, b);
	}

	void Color::clamp() {
		r = std::clamp(r, 0.0f, 1.0f);
		g = std::clamp(g, 0.0f, 1.0f);
		b = std::clamp(b, 0.0f, 1.0f);
	}

	std::string Color::toString() const {
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(2) << "<" << r << ", " << g << ", " << b << ">";
		return std::move(ss.str());
	}

	std::string Color::toJson() const {
		std::ostringstream ss;
		ss << "{\"r\":" << r << ",\"g\":" << g << ",\"b\":" << b << "}";
		return std::move(ss.str());
	}

	//
	// NiColorA
	//

	ColorA ColorA::copy() const {
		return *this;
	}

	ColorA ColorA::lerp(const ColorA& to, float transition) const {
		auto transA = 1.0f - transition;
		return ColorA(r * transA + to.r * transition, g * transA + to.g * transition, b * transA + to.b * transition, a * transA + to.a * transition);
	}

	std::string ColorA::toString() const {
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(2) << "<" << r << ", " << g << ", " << b << ", " << a << ">";
		return std::move(ss.str());
	}
}
