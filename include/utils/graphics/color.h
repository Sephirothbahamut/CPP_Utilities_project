#pragma once
#include <stdint.h>
#include <cassert>
#include <algorithm>
#include <cmath>
#include <concepts>
#include "../math/angle.h"
#include "../vec.h"

// color conversions from: https://axonflux.com/handy-rgb-to-hsl-and-rgb-to-hsv-color-model-c

namespace utils::beta::graphics::colors
	{
	namespace details 
		{
		template <typename T>
		struct alpha_field { T a; };

		struct empty {};
		}

	enum base
		{
		black , white, 
		red   , green, blue   , 
		yellow, cyan , magenta, 
		};

	template <typename            T, size_t size, T MAX_VALUE>
		requires(size >= 1 && size <= 4)
	class rgb;

	template <std::floating_point T, bool has_alpha, T MAX_VALUE, T max_angle_value>
	struct hsv;

	template <typename T, size_t size, T MAX_VALUE = (std::floating_point<T> ? T{1.} : std::numeric_limits<T>::max())>
		requires(size >= 1 && size <= 4)
	class rgb : public utils::vec<T, size>
		{
		inline static constexpr T max_value = MAX_VALUE;

		using utils::vec<T, size>::vec;
		rgb(base base, T components_multiplier = max_value, T alpha = max_value)
			{
			if constexpr (size >= 1) { r = components_multiplier * (base == base::white || base == base::red   || base == base::yellow  || base == base::magenta); }
			if constexpr (size >= 2) { r = components_multiplier * (base == base::white || base == base::green || base == base::yellow  || base == base::cyan   ); }
			if constexpr (size >= 3) { r = components_multiplier * (base == base::white || base == base::blue  || base == base::magenta || base == base::cyan   ); }
			if constexpr (size >= 4) { a = alpha; }
			}

#pragma region fields
		      T& get_r(              )       noexcept requires(size >= 1) { return (*this)[0]; }
		const T& get_r(              ) const noexcept requires(size >= 1) { return (*this)[0]; }
		      T& set_r(const T& value)       noexcept requires(size >= 1) { return (*this)[0] = value; }

		__declspec(property(get = get_r, put = set_r)) T r;

		      T& get_g(              )       noexcept requires(size >= 2) { return (*this)[1]; }
		const T& get_g(              ) const noexcept requires(size >= 2) { return (*this)[1]; }
		      T& set_g(const T& value)       noexcept requires(size >= 2) { return (*this)[1] = value; }

		__declspec(property(get = get_g, put = set_g)) T g;

		      T& get_b(              )       noexcept requires(size >= 3) { return (*this)[2]; }
		const T& get_b(              ) const noexcept requires(size >= 3) { return (*this)[2]; }
		      T& set_b(const T& value)       noexcept requires(size >= 3) { return (*this)[2] = value; }

		__declspec(property(get = get_b, put = set_b)) T b;

		      T& get_a(              )       noexcept requires(size >= 4) { return (*this)[3]; }
		const T& get_a(              ) const noexcept requires(size >= 4) { return (*this)[3]; }
		      T& set_a(const T& value)       noexcept requires(size >= 4) { return (*this)[3] = value; }

		__declspec(property(get = get_a, put = set_a)) T a;
#pragma endregion fields

		hsv<T, (size >= 4), MAX_VALUE, MAX_VALUE> hsv() const noexcept requires(size >= 3);
		};


	template <std::floating_point T, bool has_alpha, T MAX_VALUE = (std::floating_point<T> ? T{1.} : std::numeric_limits<T>::max()), T max_angle_value = MAX_VALUE>
	struct hsv : std::conditional_t<has_alpha, details::alpha_field<T>, details::empty>
		{
		inline static constexpr T max_value = MAX_VALUE;

		hsv() = default;
		hsv(T h, T s, T v, T a) requires( has_alpha) : h{h}, s{s}, v{v}, details::alpha_field<T>{a} {}
		hsv(T h, T s, T v     )                      : h{h}, s{s}, v{v}                             {}

		hsv(base base, T components_multiplier = max_value, T alpha = max_value)
			{
			utils::math::angle::deg angle;
			using namespace utils::math::angle::literals;
			switch (base)
				{
				case base::black  : angle =   0_deg; break;
				case base::white  : angle =   0_deg; break;
				case base::red    : angle =   0_deg; break;
				case base::green  : angle = 120_deg; break;
				case base::blue   : angle = 240_deg; break;
				case base::yellow : angle =  60_deg; break;
				case base::cyan   : angle = 180_deg; break;
				case base::magenta: angle = 300_deg; break;
				}
			h = components_multiplier * (base == base::white || base == base::red || base == base::yellow || base == base::magenta); 

			s = max_value; //TODO calc
			v = max_value; //TODO calc
			if constexpr (has_alpha) { details::alpha_field<T>::a = alpha; }
			}

#pragma region fields
		utils::math::angle::base_angle<T, max_angle_value> h;
		T s;
		T v;

		int  get_a(         ) const noexcept requires(has_alpha) { return details::alpha_field<T>::a        ; }
		void set_a(int value)       noexcept requires(has_alpha) {        details::alpha_field<T>::a = value; }

		__declspec(property(get = get_a, put = set_a)) int a;
#pragma endregion fields

		rgb<T, (has_alpha ? 4 : 3), MAX_VALUE> rgb() const noexcept;
		};

	//TODO test
	template <typename T, size_t size, T MAX_VALUE> requires(size >= 1 && size <= 4)
	hsv<T, (size >= 4), MAX_VALUE, MAX_VALUE> rgb<T, size, MAX_VALUE>::hsv() const noexcept requires(size >= 3)
		{
		float f_r{this->r / MAX_VALUE};
		float f_g{this->g / MAX_VALUE};
		float f_b{this->b / MAX_VALUE};
		float max = std::max(std::max(f_r, f_g), f_b), min = std::min(std::min(f_r, f_g), f_b);
		float f_h, f_s, f_v = max;

		float d = max - min;
		f_s = max == 0 ? 0 : d / max;

		if (max == min)
			{
			f_h = 0; // achromatic
			}
		else
			{
			/**/   if (max == f_r) { f_h = (f_g - f_b) / d + (f_g < f_b ? 6.f : 0.f); }
			else   if (max == f_g) { f_h = (f_b - f_r) / d + 2.f; }
			else /*if (max == b)*/ { f_h = (f_r - f_g) / d + 4.f; }
			f_h /= 6.f;
			}

		if constexpr (size == 3) { return {f_h, f_s, f_v}; }
		if constexpr (size >= 4) { return {f_h, f_s, f_v, a}; }
		}

	//TODO test
	template <std::floating_point T, bool has_alpha, T MAX_VALUE, T max_angle_value>
	rgb<T, (has_alpha ? 4 : 3), MAX_VALUE> hsv<T, has_alpha, MAX_VALUE, max_angle_value>::rgb() const noexcept
		{
		float h{this->h.value / 360.f};
		float r, g, b;

		unsigned i = static_cast<unsigned>(h * 6.f);
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

		if constexpr (!has_alpha) { return {r, g, b}; }
		if constexpr ( has_alpha) { return {r, g, b, a}; }
		}
	};




namespace utils::graphics
	{
	class color
		{
		using deg = utils::math::angle::deg;
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

				template <std::floating_point T>
				rgb(T r, T g, T b) noexcept : rgb{static_cast<uint8_t>(r * 255.f), static_cast<uint8_t>(g * 255.f), static_cast<uint8_t>(b * 255.f)}
					{
					assert(r >= 0.f && r <= 1.f);
					assert(g >= 0.f && g <= 1.f);
					assert(b >= 0.f && b <= 1.f);
					}

				/// <summary>
				/// Redirect integral types to uint8_t constructor
				/// </summary>
				template <std::integral T>
				rgb(T r, T g, T b) : rgb{static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b)}
					{
					assert(r >= T{0} && r <= T{255});
					assert(g >= T{0} && g <= T{255});
					assert(b >= T{0} && b <= T{255});
					}
				template <>
				rgb(uint8_t r, uint8_t g, uint8_t b) : r{r}, g{g}, b{b}
					{
					assert(r >= uint8_t{0} && r <= uint8_t{255});
					assert(g >= uint8_t{0} && g <= uint8_t{255});
					assert(b >= uint8_t{0} && b <= uint8_t{255});
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
				deg h{0};
				float s{0}, v{0};

				hsv(utils::math::angle::deg h, float s, float v) noexcept : h{h}, s{s}, v{v} { assert(validate()); }

				bool validate() const noexcept
					{
					return h == h.clamp()
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
				deg h{0};
				float s{0}, l{0};

				hsl(utils::math::angle::deg h, float s, float l) noexcept : h{h}, s{s}, l{l} { assert(validate()); }

				bool validate() const noexcept
					{
					return h == h.clamp()
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

			color& operator=(rgb rgb) noexcept { inner = rgb; return *this; }
			color& operator=(hsv hsv) noexcept { inner = hsv; return *this; }
			color& operator=(hsl hsl) noexcept { inner = hsl; return *this; }

			rgb   as_rgb() const noexcept { return inner; }
			hsv   as_hsv() const noexcept { return inner; }
			hsl   as_hsl() const noexcept { return inner; }
			operator rgb() const noexcept { return inner; }
			operator hsv() const noexcept { return inner; }
			operator hsl() const noexcept { return inner; }

			uint8_t get_r    () const noexcept { return inner.r; }
			uint8_t get_g    () const noexcept { return inner.g; }
			uint8_t get_b    () const noexcept { return inner.b; }
			deg     get_h    () const noexcept { return as_hsv().h; }
			float   get_hsv_s() const noexcept { return as_hsv().s; }
			float   get_v    () const noexcept { return as_hsv().v; }
			float   get_hsl_s() const noexcept { return as_hsl().s; }
			float   get_l    () const noexcept { return as_hsl().l; }
			float   get_fa   () const noexcept { return static_cast<float>(a) / 255.f; }
			void set_r    (uint8_t r) noexcept { inner.r = r; }
			void set_g    (uint8_t g) noexcept { inner.g = g; }
			void set_b    (uint8_t b) noexcept { inner.b = b; }
			void set_h    (deg     h) noexcept { auto tmp{as_hsv()}; tmp.h = h; *this = tmp; }
			void set_hsv_s(float   s) noexcept { auto tmp{as_hsv()}; tmp.s = s; *this = tmp; }
			void set_v    (float   v) noexcept { auto tmp{as_hsv()}; tmp.v = v; *this = tmp; }
			void set_hsl_s(float   s) noexcept { auto tmp{as_hsl()}; tmp.s = s; *this = tmp; }
			void set_l    (float   l) noexcept { auto tmp{as_hsl()}; tmp.l = l; *this = tmp; }
			void set_fa   (float   a) noexcept { this->a = static_cast<uint8_t>(a * 255.f);  }

			__declspec(property(get = get_r    , put = set_r    )) uint8_t r;
			__declspec(property(get = get_g    , put = set_g    )) uint8_t g;
			__declspec(property(get = get_b    , put = set_b    )) uint8_t b;
			__declspec(property(get = get_h    , put = set_h    )) deg     h;
			__declspec(property(get = get_hsv_s, put = set_hsv_s)) float   hsv_s;
			__declspec(property(get = get_v    , put = set_v    )) float   v;
			__declspec(property(get = get_hsl_s, put = set_hsl_s)) float   hsl_s;
			__declspec(property(get = get_l    , put = set_l    )) float   l;
			__declspec(property(get = get_fa   , put = set_fa   )) float   float_a;

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
		float f_r{this->r / 255.f};
		float f_g{this->g / 255.f};
		float f_b{this->b / 255.f};
		float max = std::max(std::max(f_r, f_g), f_b), min = std::min(std::min(f_r, f_g), f_b);
		float f_h, f_s, f_v = max;

		float d = max - min;
		f_s = max == 0 ? 0 : d / max;

		if (max == min)
			{
			f_h = 0; // achromatic
			}
		else
			{
			/**/   if (max == f_r)   { f_h = (f_g - f_b) / d + (f_g < f_b ? 6.f : 0.f); }
			else   if (max == f_g)   { f_h = (f_b - f_r) / d + 2.f; }
			else /*if (max == b)*/ { f_h = (f_r - f_g) / d + 4.f; }
			f_h /= 6.f;
			}

		return {f_h * 360.f, f_s, f_v};
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
		float h{this->h.value / 360.f};
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
		float h{this->h.value / 360.f};
		float r, g, b;

		unsigned i = static_cast<unsigned>(h * 6.f);
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