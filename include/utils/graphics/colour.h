#pragma once
#include <stdint.h>
#include <cassert>
#include <algorithm>
#include <cmath>
#include <concepts>

#include "../math/angle.h"
#include "../console/colour.h"
#include "../template_wrappers.h"
#include "../containers/memberwise_operators_array.h"

// color conversions from: https://axonflux.com/handy-rgb-to-hsl-and-rgb-to-hsv-color-model-c

//TODO write test cases

namespace utils::graphics::colour
	{
	enum base
		{
		black , white, 
		red   , green, blue   , 
		yellow, cyan , magenta, 
		};

	template <typename T = float, size_t size = 3, utils::template_wrapper::number<T> MAX_VALUE = utils::template_wrapper::number<T>{std::floating_point<T> ? static_cast<T>(1) : std::numeric_limits<T>::max()} >
		requires(size >= 1 && size <= 4)
	struct rgb;
	
	using rgb_f  = rgb<float  , 3>;
	using rgb_d  = rgb<double , 3>;
	using rgb_u  = rgb<uint8_t, 3>;
	//using rgb  = rgb<float, 3>; //default 
	using rgba_f = rgb<float  , 4>;
	using rgba_d = rgb<double , 4>;
	using rgba_u = rgb<uint8_t, 4>;
	using rgba   = rgb<float  , 4>;

	template <std::floating_point T, bool has_alpha, utils::template_wrapper::number<T> MAX_VALUE = utils::template_wrapper::number<T>{std::floating_point<T> ? static_cast<T>(1) : std::numeric_limits<T>::max()}, utils::template_wrapper::number<T> max_angle_value = MAX_VALUE >
	struct hsv;

	namespace details
		{
		template <typename T>
		struct alpha_field { T a; };

		struct empty {};
		}

	template <typename T, size_t SIZE, utils::template_wrapper::number<T> MAX_VALUE>
		requires(SIZE >= 1 && SIZE <= 4)
	struct rgb : utils::containers::memberwise_operators::arr<T, SIZE>
		{
		using value_type = utils::containers::memberwise_operators::arr<T, SIZE>::value_type;
		inline static constexpr size_t static_size{utils::containers::memberwise_operators::arr<T, SIZE>::static_size};

		inline static constexpr T max_value = MAX_VALUE;

		rgb(T r = T{0}, T g = T{0}, T b = T{0}, T a = T{max_value})
			{
			if constexpr (static_size >= 1) { this->r = r; }
			if constexpr (static_size >= 2) { this->g = g; }
			if constexpr (static_size >= 3) { this->b = b; }
			if constexpr (static_size >= 4) { this->a = a; }
			}

		rgb(base base, T components_multiplier = max_value, T alpha = max_value) requires(static_size >= 4)
			{
			if constexpr (static_size >= 1) { r = components_multiplier * (base == base::white || base == base::red   || base == base::yellow  || base == base::magenta); }
			if constexpr (static_size >= 2) { g = components_multiplier * (base == base::white || base == base::green || base == base::yellow  || base == base::cyan   ); }
			if constexpr (static_size >= 3) { b = components_multiplier * (base == base::white || base == base::blue  || base == base::magenta || base == base::cyan   ); }
			if constexpr (static_size >= 4) { a = alpha; }
			}

#pragma region fields
		      T& get_r(              )       noexcept requires(static_size >= 1) { return (*this)[0]; }
		const T& get_r(              ) const noexcept requires(static_size >= 1) { return (*this)[0]; }
		      T& set_r(const T& value)       noexcept requires(static_size >= 1) { return (*this)[0] = value; }

		__declspec(property(get = get_r, put = set_r)) T r;

		      T& get_g(              )       noexcept requires(static_size >= 2) { return (*this)[1]; }
		const T& get_g(              ) const noexcept requires(static_size >= 2) { return (*this)[1]; }
		      T& set_g(const T& value)       noexcept requires(static_size >= 2) { return (*this)[1] = value; }

		__declspec(property(get = get_g, put = set_g)) T g;

		      T& get_b(              )       noexcept requires(static_size >= 3) { return (*this)[2]; }
		const T& get_b(              ) const noexcept requires(static_size >= 3) { return (*this)[2]; }
		      T& set_b(const T& value)       noexcept requires(static_size >= 3) { return (*this)[2] = value; }

		__declspec(property(get = get_b, put = set_b)) T b;

		      T& get_a(              )       noexcept requires(static_size >= 4) { return (*this)[3]; }
		const T& get_a(              ) const noexcept requires(static_size >= 4) { return (*this)[3]; }
		      T& set_a(const T& value)       noexcept requires(static_size >= 4) { return (*this)[3] = value; }

		__declspec(property(get = get_a, put = set_a)) T a;
#pragma endregion fields

		//hsv<T, std::greater_equal<size_t>(static_size, 4), max_value, max_value> hsv() const noexcept requires(static_size >= 3);
		};

	template <std::floating_point T, bool has_alpha, utils::template_wrapper::number<T> MAX_VALUE, utils::template_wrapper::number<T> max_angle_value>
	struct hsv : std::conditional_t<has_alpha, details::alpha_field<T>, details::empty>
		{
		inline static constexpr T max_value = MAX_VALUE;

		hsv() = default;
		hsv(T h, T s, T v, T a) requires( has_alpha) : h{h}, s{s}, v{v}, details::alpha_field<T>{a} {}
		hsv(T h, T s, T v     )                      : h{h}, s{s}, v{v}                             {}

		hsv(base base, T components_multiplier = max_value, T alpha = max_value)
			{
			utils::math::angle::degf angle;
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
			h = {angle};

			s = max_value; //TODO make correct calculation
			v = max_value; //TODO make correct calculation
			if constexpr (has_alpha) { details::alpha_field<T>::a = alpha; }
			}

#pragma region fields
		utils::math::angle::base<T, max_angle_value> h;
		T s;
		T v;
#pragma endregion fields

		rgb<T, (has_alpha ? 4 : 3), MAX_VALUE> rgb() const noexcept;
		};

	////TODO test
	//template <typename T, size_t SIZE, T MAX_VALUE> requires(SIZE >= 1 && SIZE <= 4)
	//hsv<T, (SIZE >= 4), MAX_VALUE, MAX_VALUE> rgb<T, SIZE, MAX_VALUE>::hsv() const noexcept
	//	{
	//	float f_r{this->r / MAX_VALUE};
	//	float f_g{this->g / MAX_VALUE};
	//	float f_b{this->b / MAX_VALUE};
	//	float max = std::max(std::max(f_r, f_g), f_b), min = std::min(std::min(f_r, f_g), f_b);
	//	float f_h, f_s, f_v = max;
	//
	//	float d = max - min;
	//	f_s = max == 0 ? 0 : d / max;
	//
	//	if (max == min)
	//		{
	//		f_h = 0; // achromatic
	//		}
	//	else
	//		{
	//		/**/   if (max == f_r) { f_h = (f_g - f_b) / d + (f_g < f_b ? 6.f : 0.f); }
	//		else   if (max == f_g) { f_h = (f_b - f_r) / d + 2.f; }
	//		else /*if (max == b)*/ { f_h = (f_r - f_g) / d + 4.f; }
	//		f_h /= 6.f;
	//		}
	//
	//	if constexpr (size == 3) { return {f_h, f_s, f_v}; }
	//	if constexpr (size >= 4) { return {f_h, f_s, f_v, a}; }
	//	}
	
	//TODO test
	template <std::floating_point T, bool has_alpha, utils::template_wrapper::number<T> MAX_VALUE, utils::template_wrapper::number<T> max_angle_value>
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
		if constexpr ( has_alpha) { return {r, g, b, details::alpha_field<T>::a}; }
		}
	};
	

namespace utils::output
	{
	namespace typeless
		{
		template <typename T, size_t SIZE, T MAX_VALUE>
		inline ::std::ostream& operator<<(::std::ostream& os, const utils::graphics::colour::rgb<T, SIZE, MAX_VALUE>& colour)
			{
			namespace ucc = utils::console::colour;

			os << ucc::brace << "(";

			if constexpr (SIZE >= 1) { os                       << ucc::foreground::red   << std::to_string(colour[0]); }
			if constexpr (SIZE >= 2) { os << ucc::separ << ", " << ucc::foreground::green << std::to_string(colour[1]); }
			if constexpr (SIZE >= 3) { os << ucc::separ << ", " << ucc::foreground::blue  << std::to_string(colour[2]); }
			if constexpr (SIZE >= 4) { os << ucc::separ << ", " << ucc::foreground::white << std::to_string(colour[3]); }

			os << ucc::brace << ")";

			return os;
			}
		}

	template <typename T, size_t SIZE, T MAX_VALUE>
	inline ::std::ostream& operator<<(::std::ostream& os, const utils::graphics::colour::rgb<T, SIZE, MAX_VALUE>& colour)
		{
		namespace ucc = utils::console::colour;
		os << ucc::type << "rgb" << SIZE << typeid(T).name();
		return utils::output::typeless::operator<<(os, colour);
		}
	}