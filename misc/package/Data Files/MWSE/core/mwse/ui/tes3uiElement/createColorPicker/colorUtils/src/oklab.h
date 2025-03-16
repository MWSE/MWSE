// Sources of this code are:
// https://bottosson.github.io/posts/colorpicker/
// https://bottosson.github.io/posts/gamutclipping/
// https://bottosson.github.io/posts/oklab/
/*
	Copyright (c) 2021 Björn Ottosson

	Permission is hereby granted, free of charge, to any person obtaining a copy of
	this software and associated documentation files (the "Software"), to deal in
	the Software without restriction, including without limitation the rights to
	use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
	of the Software, and to permit persons to whom the Software is furnished to do
	so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#pragma once


#define pi          3.141592653589793238462643383279502884L

#include <algorithm>
#include <cmath>

#include "types.h"


namespace {
	struct Lab { float L; float a; float b; };
	struct HSL { float h; float s; float l; };
	// finds L_cusp and C_cusp for a given hue
	// a and b must be normalized so a^2 + b^2 == 1
	struct LC { float L; float C; };

	// Alternative representation of (L_cusp, C_cusp)
	// Encoded so S = C_cusp/L_cusp and T = C_cusp/(1-L_cusp)
	// The maximum value for C in the triangle is then found as fmin(S*L, T*(1-L)), for a given L
	struct ST { float S; float T; };


	Lab linear_srgb_to_oklab(RGB c)
	{
		float l = 0.4122214708f * c.r + 0.5363325363f * c.g + 0.0514459929f * c.b;
		float m = 0.2119034982f * c.r + 0.6806995451f * c.g + 0.1073969566f * c.b;
		float s = 0.0883024619f * c.r + 0.2817188376f * c.g + 0.6299787005f * c.b;

		float l_ = cbrtf(l);
		float m_ = cbrtf(m);
		float s_ = cbrtf(s);

		return {
			0.2104542553f*l_ + 0.7936177850f*m_ - 0.0040720468f*s_,
			1.9779984951f*l_ - 2.4285922050f*m_ + 0.4505937099f*s_,
			0.0259040371f*l_ + 0.7827717662f*m_ - 0.8086757660f*s_,
		};
	}

	RGB oklab_to_linear_srgb(Lab c)
	{
		float l_ = c.L + 0.3963377774f * c.a + 0.2158037573f * c.b;
		float m_ = c.L - 0.1055613458f * c.a - 0.0638541728f * c.b;
		float s_ = c.L - 0.0894841775f * c.a - 1.2914855480f * c.b;

		float l = l_*l_*l_;
		float m = m_*m_*m_;
		float s = s_*s_*s_;

		return {
			+4.0767416621f * l - 3.3077115913f * m + 0.2309699292f * s,
			-1.2684380046f * l + 2.6097574011f * m - 0.3413193965f * s,
			-0.0041960863f * l - 0.7034186147f * m + 1.7076147010f * s,
		};
	}

	// toe function for L_r
	float toe(float x)
	{
		constexpr float k_1 = 0.206f;
		constexpr float k_2 = 0.03f;
		constexpr float k_3 = (1.f + k_1) / (1.f + k_2);
		return 0.5f * (k_3 * x - k_1 + sqrtf((k_3 * x - k_1) * (k_3 * x - k_1) + 4 * k_2 * k_3 * x));
	}

	// inverse toe function for L_r
	float toe_inv(float x)
	{
		constexpr float k_1 = 0.206f;
		constexpr float k_2 = 0.03f;
		constexpr float k_3 = (1.f + k_1) / (1.f + k_2);
		return (x * x + k_1 * x) / (k_3 * (x + k_2));
	}

	ST to_ST(LC cusp)
	{
		float L = cusp.L;
		float C = cusp.C;
		return { C / L, C / (1 - L) };
	}

	// Finds the maximum saturation possible for a given hue that fits in sRGB
	// Saturation here is defined as S = C/L
	// a and b must be normalized so a^2 + b^2 == 1
	float compute_max_saturation(float a, float b)
	{
		// Max saturation will be when one of r, g or b goes below zero.

		// Select different coefficients depending on which component goes below zero first
		float k0, k1, k2, k3, k4, wl, wm, ws;

		if (-1.88170328f * a - 0.80936493f * b > 1)
		{
			// Red component
			k0 = +1.19086277f; k1 = +1.76576728f; k2 = +0.59662641f; k3 = +0.75515197f; k4 = +0.56771245f;
			wl = +4.0767416621f; wm = -3.3077115913f; ws = +0.2309699292f;
		}
		else if (1.81444104f * a - 1.19445276f * b > 1)
		{
			// Green component
			k0 = +0.73956515f; k1 = -0.45954404f; k2 = +0.08285427f; k3 = +0.12541070f; k4 = +0.14503204f;
			wl = -1.2684380046f; wm = +2.6097574011f; ws = -0.3413193965f;
		}
		else
		{
			// Blue component
			k0 = +1.35733652f; k1 = -0.00915799f; k2 = -1.15130210f; k3 = -0.50559606f; k4 = +0.00692167f;
			wl = -0.0041960863f; wm = -0.7034186147f; ws = +1.7076147010f;
		}

		// Approximate max saturation using a polynomial:
		float S = k0 + k1 * a + k2 * b + k3 * a * a + k4 * a * b;

		// Do one step Halley's method to get closer
		// this gives an error less than 10e6, except for some blue hues where the dS/dh is close to infinite
		// this should be sufficient for most applications, otherwise do two/three steps

		float k_l = +0.3963377774f * a + 0.2158037573f * b;
		float k_m = -0.1055613458f * a - 0.0638541728f * b;
		float k_s = -0.0894841775f * a - 1.2914855480f * b;

		{
			float l_ = 1.f + S * k_l;
			float m_ = 1.f + S * k_m;
			float s_ = 1.f + S * k_s;

			float l = l_ * l_ * l_;
			float m = m_ * m_ * m_;
			float s = s_ * s_ * s_;

			float l_dS = 3.f * k_l * l_ * l_;
			float m_dS = 3.f * k_m * m_ * m_;
			float s_dS = 3.f * k_s * s_ * s_;

			float l_dS2 = 6.f * k_l * k_l * l_;
			float m_dS2 = 6.f * k_m * k_m * m_;
			float s_dS2 = 6.f * k_s * k_s * s_;

			float f  = wl * l     + wm * m     + ws * s;
			float f1 = wl * l_dS  + wm * m_dS  + ws * s_dS;
			float f2 = wl * l_dS2 + wm * m_dS2 + ws * s_dS2;

			S = S - f * f1 / (f1*f1 - 0.5f * f * f2);
		}

		return S;
	}

	LC find_cusp(float a, float b)
	{
		// First, find the maximum saturation (saturation S = C/L)
		float S_cusp = compute_max_saturation(a, b);

		// Convert to linear sRGB to find the first point where at least one of r,g or b >= 1:
		RGB rgb_at_max = oklab_to_linear_srgb({ 1, S_cusp * a, S_cusp * b });
		float L_cusp = cbrtf(1.f / std::max(std::max(rgb_at_max.r, rgb_at_max.g), rgb_at_max.b));
		float C_cusp = L_cusp * S_cusp;

		return { L_cusp , C_cusp };
	}

	// TODO: see if srgb_transfer_function and srgb_transfer_function_inv are needed in our use case.
	float srgb_transfer_function(float v)
	{
		return v;
	}
	float srgb_transfer_function_inv(float v)
	{
		return v;
	}
}

namespace oklab {
	RGB okhsv_to_srgb(HSV hsv)
	{
		float h = hsv.h;
		float s = hsv.s;
		float v = hsv.v;

		float a_ = cosf(2.f * pi * h);
		float b_ = sinf(2.f * pi * h);

		LC cusp = find_cusp(a_, b_);
		ST ST_max = to_ST(cusp);
		float S_max = ST_max.S;
		float T_max = ST_max.T;
		float S_0 = 0.5f;
		float k = 1 - S_0 / S_max;

		// first we compute L and V as if the gamut is a perfect triangle:

		// L, C when v==1:
		float L_v = 1     - s * S_0 / (S_0 + T_max - T_max * k * s);
		float C_v = s * T_max * S_0 / (S_0 + T_max - T_max * k * s);

		float L = v * L_v;
		float C = v * C_v;

		// then we compensate for both toe and the curved top part of the triangle:
		float L_vt = toe_inv(L_v);
		float C_vt = C_v * L_vt / L_v;

		float L_new = toe_inv(L);
		C = C * L_new / L;
		L = L_new;

		RGB rgb_scale = oklab_to_linear_srgb({ L_vt, a_ * C_vt, b_ * C_vt });
		float scale_L = cbrtf(1.f / fmax(fmax(rgb_scale.r, rgb_scale.g), fmax(rgb_scale.b, 0.f)));

		L = L * scale_L;
		C = C * scale_L;

		RGB rgb = oklab_to_linear_srgb({ L, C * a_, C * b_ });
		return {
			srgb_transfer_function(rgb.r),
			srgb_transfer_function(rgb.g),
			srgb_transfer_function(rgb.b),
		};
	}

	HSV srgb_to_okhsv(RGB rgb)
	{
		Lab lab = linear_srgb_to_oklab({
			srgb_transfer_function_inv(rgb.r),
			srgb_transfer_function_inv(rgb.g),
			srgb_transfer_function_inv(rgb.b)
		});

		float L = lab.L;
		float C = sqrtf(lab.a * lab.a + lab.b * lab.b);
		float h = 0.5f + 0.5f * atan2f(-lab.b, -lab.a) / pi;
		float s = 0.0f;
		float v = toe(L);

		if (std::fpclassify(L) != FP_ZERO && std::fpclassify(std::abs(L - 1.f)) != FP_ZERO && std::fpclassify(C) != FP_ZERO) {

			float a_ = lab.a / C;
			float b_ = lab.b / C;

			LC cusp = find_cusp(a_, b_);
			ST ST_max = to_ST(cusp);
			float S_max = ST_max.S;
			float T_max = ST_max.T;
			float S_0 = 0.5f;
			float k = 1 - S_0 / S_max;

			// first we find L_v, C_v, L_vt and C_vt

			float t = T_max / (C + L * T_max);
			float L_v = t * L;
			float C_v = t * C;

			float L_vt = toe_inv(L_v);
			float C_vt = C_v * L_vt / L_v;

			// we can then use these to invert the step that compensates for the toe and the curved top part of the triangle:
			RGB rgb_scale = oklab_to_linear_srgb({ L_vt, a_ * C_vt, b_ * C_vt });
			float scale_L = cbrtf(1.f / fmax(fmax(rgb_scale.r, rgb_scale.g), fmax(rgb_scale.b, 0.f)));

			L = L / scale_L;
			C = C / scale_L;

			C = C * toe(L) / L;
			L = toe(L);

			// we can now compute v and s:

			v = L / L_v;
			s = (S_0 + T_max) * C_v / ((T_max * S_0) + T_max * k * C_v);
		}

		return { h, s, v };
	}
}
