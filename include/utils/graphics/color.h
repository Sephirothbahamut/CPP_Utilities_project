#pragma once
#include <stdint.h>
#include <cassert>
#include <algorithm>
#include <cmath>

// color conversions from: https://axonflux.com/handy-rgb-to-hsl-and-rgb-to-hsv-color-model-c

namespace utils::graphics
	{
	class color
		{
		public:
			struct hsv;
			struct hsl;
			/// <summary>
			/// Colors in RGB format, where each component is a byte.
			/// Floats constructor takes floats in the range 0-1.
			/// The alpha is treated like the other components.
			/// </summary>
			struct rgb
				{
				uint8_t r{0}, g{0}, b{0};
				rgb(float   r, float   g, float   b) noexcept : rgb{static_cast<uint8_t>(r * 255.f), static_cast<uint8_t>(g * 255.f), static_cast<uint8_t>(b * 255.f)}
					{
					assert(r >= 0.f && r <= 1.f);
					assert(g >= 0.f && g <= 1.f);
					assert(b >= 0.f && b <= 1.f);
					}
				rgb(uint8_t r, uint8_t g, uint8_t b) noexcept : r{r}, g{g}, b{b} {}

				/// <summary>
				/// Redirect integral types to uint8_t constructor
				/// </summary>
				template <typename T>
				rgb(T r, T g, T b, typename std::enable_if<std::is_integral<T>::value, T>::type=0) : rgb{static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b)}
					{
					assert(r >= T{0} && r <= T{255});
					assert(g >= T{0} && g <= T{255});
					assert(b >= T{0} && b <= T{255});
					}

				operator hsv() const noexcept;
				operator hsl() const noexcept;
				};
			/// <summary>
			/// Colors in HSV format.
			/// Hue is a float angle in the range 0-360.
			/// Saturation is a float in the range 0-1.
			/// Value is a float in the range 0-1.
			/// </summary>
			struct hsv
				{
				float h{0}, s{0}, v{0};

				hsv(float h, float s, float v) noexcept : h{h}, s{s}, v{v} { assert(validate()); }

				bool validate() const noexcept
					{
					return h >= 0.f && h <= 360.f
						&& s >= 0.f && s <= 1.f
						&& v >= 0.f && v <= 1.f;
					}

				operator rgb() const noexcept;
				operator hsl() const noexcept;
				};
			/// <summary>
			/// Colors in HSL format.
			/// Hue is a float angle in the range 0-360.
			/// Saturation is a float in the range 0-1.
			/// Lightness is a float in the range 0-1.
			/// </summary>
			struct hsl
				{
				float h{0}, s{0}, l{0};

				hsl(float h, float s, float l) noexcept : h{h}, s{s}, l{l} { assert(validate()); }

				bool validate() const noexcept
					{
					return h >= 0.f && h <= 360.f
						&& s >= 0.f && s <= 1.f
						&& l >= 0.f && l <= 1.f;
					}

				operator rgb() const noexcept;
				operator hsv() const noexcept;
				};

			color() noexcept = default;
			color(rgb rgb, uint8_t a = 255) noexcept : inner{rgb}, a{a} {}
			color(hsv hsv, uint8_t a = 255) noexcept : inner{hsv}, a{a} {}
			color(hsl hsl, uint8_t a = 255) noexcept : inner{hsl}, a{a} {}

			rgb   as_rgb() const noexcept { return inner; }
			hsv   as_hsv() const noexcept { return inner; }
			hsl   as_hsl() const noexcept { return inner; }
			operator rgb() const noexcept { return inner; }
			operator hsv() const noexcept { return inner; }
			operator hsl() const noexcept { return inner; }

			uint8_t get_r    () const noexcept { return inner.r; }
			uint8_t get_g    () const noexcept { return inner.g; }
			uint8_t get_b    () const noexcept { return inner.b; }
			float   get_h    () const noexcept { return as_hsv().h; }
			float   get_hsv_s() const noexcept { return as_hsv().s; }
			float   get_v    () const noexcept { return as_hsv().v; }
			float   get_hsl_s() const noexcept { return as_hsl().s; }
			float   get_l    () const noexcept { return as_hsl().l; }
			void set_r    (uint8_t r) noexcept { inner.r = r; }
			void set_g    (uint8_t g) noexcept { inner.g = g; }
			void set_b    (uint8_t b) noexcept { inner.b = b; }
			void set_h    (float   h) noexcept { auto tmp{as_hsv()}; tmp.h = h; *this = tmp; }
			void set_hsv_s(float   s) noexcept { auto tmp{as_hsv()}; tmp.s = s; *this = tmp; }
			void set_v    (float   v) noexcept { auto tmp{as_hsv()}; tmp.v = v; *this = tmp; }
			void set_hsl_s(float   s) noexcept { auto tmp{as_hsl()}; tmp.s = s; *this = tmp; }
			void set_l    (float   l) noexcept { auto tmp{as_hsl()}; tmp.l = l; *this = tmp; }

			__declspec(property(get = get_r,     put = set_r))     uint8_t r;
			__declspec(property(get = get_g,     put = set_g))     uint8_t g;
			__declspec(property(get = get_b,     put = set_b))     uint8_t b;
			__declspec(property(get = get_h,     put = set_h))     float   h;
			__declspec(property(get = get_hsv_s, put = set_hsv_s)) float   hsv_s;
			__declspec(property(get = get_v,     put = set_v))     float   v;
			__declspec(property(get = get_hsl_s, put = set_hsl_s)) float   hsl_s;
			__declspec(property(get = get_l,     put = set_l))     float   l;

			uint8_t a{255};

			static const color black;
			static const color white;
			static const color red;
			static const color green;
			static const color blue;
			static const color yellow;
			static const color magenta;
			static const color cyan;
			static const color transparent;
		private:
			rgb inner;
		};

	inline const color color::black      {rgb{  0,   0,   0}};
	inline const color color::white      {rgb{255, 255, 255}};
	inline const color color::red        {rgb{255,   0,   0}};
	inline const color color::green      {rgb{  0, 255,   0}};
	inline const color color::blue       {rgb{  0,   0, 255}};
	inline const color color::yellow     {rgb{255, 255,   0}};
	inline const color color::magenta    {rgb{255,   0, 255}};
	inline const color color::cyan       {rgb{  0, 255, 255}};
	inline const color color::transparent{rgb{  0,   0,   0}, 0};

	inline color::rgb::operator color::hsv() const noexcept
		{
		float r{this->r / 255.f};
		float g{this->g / 255.f};
		float b{this->b / 255.f};
		float max = std::max(std::max(r, g), b), min = std::min(std::min(r, g), b);
		float h, s, v = max;

		float d = max - min;
		s = max == 0 ? 0 : d / max;

		if (max == min)
			{
			h = 0; // achromatic
			}
		else
			{
			/**/   if (max == r)   { h = (g - b) / d + (g < b ? 6.f : 0.f); }
			else   if (max == g)   { h = (b - r) / d + 2.f; }
			else /*if (max == b)*/ { h = (r - g) / d + 4.f; }
			h /= 6.f;
			}

		return {h * 360.f, s, v};
		}
	inline color::rgb::operator color::hsl() const noexcept
		{
		float r{this->r / 255.f};
		float g{this->g / 255.f};
		float b{this->b / 255.f};
		float max = std::max(std::max(r, g), b), min = std::min(std::min(r, g), b);
		float h, s, l = (max + min) / 2.f;

		if (max == min) { h = s = 0; }// achromatic
		else
			{
			float d = max - min;
			s = l > 0.5 ? d / (2 - max - min) : d / (max + min);
			/**/ if   (max == r)   { h = (g - b) / d + (g < b ? 6.f : 0.f); }
			else if   (max == g)   { h = (b - r) / d + 2.f; }
			else /*if (max == b)*/ { h = (r - g) / d + 4.f; }
			h /= 6.f;
			}

		return {h * 360.f, s, l};
		}
	
	inline color::hsl::operator color::rgb() const noexcept
		{
		float h{this->h / 360.f};
		float r, g, b;

		if (s == 0) { r = g = b = l; } // achromatic
		else
			{
			auto hue2rgb = [] (float p, float q, float t)
				{
				if (t < 0.f) t += 1.f;
				if (t > 1.f) t -= 1.f;
				if (t < 1.f / 6.f) return p + (q - p) * 6.f * t;
				if (t < 1.f / 2.f) return q;
				if (t < 2.f / 3.f) return p + (q - p) * (2.f / 3.f - t) * 6.f;
				return p;
				};

			float q = l < 0.5f ? l * (1.f + s) : l + s - l * s;
			float p = 2.f * l - q;
			r = hue2rgb(p, q, h + 1.f / 3.f);
			g = hue2rgb(p, q, h);
			b = hue2rgb(p, q, h - 1.f / 3.f);
			}

		return {r, g, b};
		}
	inline color::hsl::operator color::hsv() const noexcept { return hsl{color::rgb{*this}}; }

	inline color::hsv::operator color::rgb() const noexcept
		{
		float h{this->h / 360.f};
		float r, g, b;

		unsigned i = h * 6.f;
		float f = h * 6.f - i;
		float p = v * (1.f - s);
		float q = v * (1.f - f * s);
		float t = v * (1.f - (1.f - f) * s);

		switch (i % 6)
			{
			case 0: r = v, g = t, b = p; break;
			case 1: r = q, g = v, b = p; break;
			case 2: r = p, g = v, b = t; break;
			case 3: r = p, g = q, b = v; break;
			case 4: r = t, g = p, b = v; break;
			case 5: r = v, g = p, b = q; break; 
			default: //Actually unreachable, but the compiler doesn't realize that 0 <= i < 6, so it thinks r, g and b aren't always initialized
				r = 0, g = 0, b = 0;
			}
		
		return {r, g, b};
		}
	inline color::hsv::operator color::hsl() const noexcept { return static_cast<color::hsv>(static_cast<color::rgb>(*this)); }
	}