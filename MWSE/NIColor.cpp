#include "NIColor.h"

namespace NI {

	//
	// NiColor
	//

	std::string PackedColor::toString() {
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(2) << "<" << int(r) << ", " << int(g) << ", " << int(b) << ", " << int(a) << ">";
		return std::move(ss.str());
	}

	Color::Color(TES3::Vector3& vector) {
		r = vector.x;
		g = vector.y;
		b = vector.z;
	}

	Color::Color(sol::table table) {
		r = table.get_or("r", table.get_or(1, 0.0f));
		g = table.get_or("g", table.get_or(2, 0.0f));
		b = table.get_or("b", table.get_or(3, 0.0f));
	}

	Color::Color(sol::object object) {
		if (object.is<TES3::Vector3>()) {
			*this = Color(object.as<TES3::Vector3>());
		}
		else if (object.is<sol::table>()) {
			*this = Color(object.as<sol::table>());
		}
		throw std::invalid_argument("Could not convert lua object to NiColor.");
	}

	Color& Color::operator=(const TES3::Vector3& vector) {
		r = vector.x;
		g = vector.y;
		b = vector.z;
		return *this;
	}

	Color& Color::operator=(const sol::table table) {
		r = table.get_or("r", table.get_or(1, 0.0f));
		g = table.get_or("g", table.get_or(2, 0.0f));
		b = table.get_or("b", table.get_or(3, 0.0f));
		return *this;
	}

	Color& Color::operator=(const sol::object object) {
		if (object.is<TES3::Vector3>()) {
			*this = object.as<TES3::Vector3>();
		}
		else if (object.is<sol::table>()) {
			*this = object.as<sol::table>();
		}
		else {
			throw std::invalid_argument("Could not convert lua object to NiColor.");
		}
		return *this;
	}

	bool Color::operator==(const Color& c) {
		return r == c.r && g == c.g && b == c.b;
	}

	bool Color::operator!=(const Color& c) {
		return r != c.r || g != c.g || b != c.b;
	}

	Color Color::operator+(const Color& c) {
		return Color(r + c.r, g + c.g, b + c.b);
	}

	Color Color::operator-(const Color& c) {
		return Color(r - c.r, g - c.g, b - c.b);
	}

	Color Color::operator*(const Color & c) {
		return Color(r * c.r, g * c.g, b * c.b);
	}

	Color Color::operator*(const float scalar) {
		return Color(r * scalar, g * scalar, b * scalar);
	}

	Color Color::copy() const {
		return *this;
	}

	Color Color::lerp(const Color& to, float transition) const {
		auto transA = 1.0f - transition;
		return Color(r * transA + to.r * transition, g * transA + to.g * transition, b * transA + to.b * transition);
	}

	TES3::Vector3 Color::toVector3() const {
		return TES3::Vector3(r, g, b);
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
