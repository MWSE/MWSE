#include <algorithm>
#include <cmath>
#include <cstdint>

#include "oklab.h"
#include "types.h"

namespace {
	// "Over" alpha compositing
	float over(float cA, float cB, double alphaA, double alphaB, double inverse, double alphaO) {
		return (cA * alphaA + cB * alphaB * inverse) / alphaO;
	}
}

extern "C" HSV sRGBtoHSV(RGB c) {
	HSV result = oklab::srgb_to_okhsv(c);
	// Oklab uses hue in [0, 1]. Remap  and wrap to [0, 360).
	result.h = std::fmod(result.h * 360.f, 360.f);
	return result;
}

extern "C" RGB HSVtosRGB(HSV hsv) {
	// Oklab uses hue range [0, 1].
	hsv.h = hsv.h / 360;
	return oklab::okhsv_to_srgb(hsv);
}

extern "C" void generateMainPicker(const double hue, RGB *data, const std::uint32_t width, const std::uint32_t height) {
	HSV hsv = { static_cast<float>(hue), 0.f, 0.f };
	const float heightf = static_cast<float>(height);
	const float widthf = static_cast<float>(width);

	for (std::uint32_t y = 0; y < height; ++y) {
		const std::uint32_t offset = y * width;
		hsv.v = 1 - y / heightf;

		for (std::uint32_t x = 1; x <= width; ++x) {
			hsv.s = x / widthf;
			data[offset + x] = HSVtosRGB(hsv);
		}
	}
}

extern "C" void generateSaturationPicker(const double hue, const double value, RGB *data, const std::uint32_t width, const std::uint32_t height) {
	HSV hsv = { static_cast<float>(hue), 1.f, static_cast<float>(value) };
	const float heightf = static_cast<float>(height);

	for (std::uint32_t y = 0; y < height; ++y) {
		const std::uint32_t offset = y * width;
		hsv.s = 1 - y / heightf;

		for (std::uint32_t x = 1; x <= width; ++x) {
			data[offset + x] = HSVtosRGB(hsv);
		}
	}
}

extern "C" void generatePreviewImage(const RGB color, const double alpha, RGB *destination, double *alphas, RGB *checkers, const std::uint32_t width, const std::uint32_t height) {
	for (std::uint32_t y = 0; y < height; ++y) {
		const std::uint32_t offset = y * width;
		for (std::uint32_t x = 1; x <= width; ++x) {
			const std::uint32_t i = offset + x;
			auto &p1 = destination[i];
			auto &p2 = checkers[i];
			// Background is always opaque here.
			constexpr double a2 = 1.0;

			double inverse = 1.0 - alpha;
			double alphaOut = alpha + a2 * inverse;
			p1.r = over(color.r, p2.r, alpha, a2, inverse, alphaOut);
			p1.g = over(color.g, p2.g, alpha, a2, inverse, alphaOut);
			p1.b = over(color.b, p2.b, alpha, a2, inverse, alphaOut);
			alphas[i] = alphaOut;
		}
	}
}
