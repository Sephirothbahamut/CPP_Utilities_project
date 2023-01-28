#pragma once
#include <cmath>
#include <limits>
#include <cassert>
#include <concepts>
#include <algorithm>

#include "../compilation/warnings.h"
#include "../math/angle.h"
#include "../console/colour.h"
#include "../template_wrappers.h"

#include "../details/vec/common.h"
#include "../details/vec/output.h"
#include "../details/vec/memberwise_operators.h"
#include "../details/vec/factories.h"

// color conversions from: https://axonflux.com/handy-rgb-to-hsl-and-rgb-to-hsv-color-model-c

//TODO write test cases

namespace utils::graphics::colour
	{
	enum class base
		{
		black , white, 
		red   , green, blue   , 
		yellow, cyan , magenta, 
		};

	template <typename T, size_t size>
	struct rgb;

	using rgb_f  = rgb<float  , 3>;
	using rgb_d  = rgb<double , 3>;
	using rgb_u  = rgb<uint8_t, 3>;
	//using rgb  = rgb<float, 3>; //default 
	using rgba_f = rgb<float  , 4>;
	using rgba_d = rgb<double , 4>;
	using rgba_u = rgb<uint8_t, 4>;
	using rgba   = rgb<float  , 4>;

	template <std::floating_point T, bool has_alpha>
	struct hsv;

	namespace concepts
		{
		template <typename T>
		concept rgb = std::same_as<T, colour::rgb<typename T::value_type, T::static_size>>;

		template <typename T>
		concept hsv = std::same_as<T, colour::hsv<typename T::value_type, T::has_alpha>>;

		template <typename T>
		concept colour = rgb<T> || hsv<T>;
		}

	namespace details
		{
		template <typename T>
		struct alpha_field { T a; };

		struct empty {};

		inline extern constexpr const char colour_name[]{"rgb"};

		template<typename T, size_t SIZE, typename DERIVED_T>
		class colour_crtp :
			public utils::details::vec::array<T, SIZE, DERIVED_T>,
			public utils::details::vec::memberwise_operators<T, DERIVED_T>,
			public utils::details::vec::output<colour_name, DERIVED_T>,
			public utils::details::vec::factories<T, SIZE, DERIVED_T>
			{
			public:
				using derived_t = DERIVED_T;

			private:
				constexpr const derived_t& derived() const noexcept { return static_cast<const derived_t&>(*this); }
				constexpr       derived_t& derived()       noexcept { return static_cast<derived_t&>(*this); }
				constexpr const auto     & get_arr() const noexcept { return derived().array; }
				constexpr       auto     & get_arr()       noexcept { return derived().array; }

				using arr_t = std::array<T, SIZE>;

			public:
				inline static constexpr const size_t static_size{std::tuple_size_v<arr_t>};
				using value_type = typename arr_t::value_type;

				inline static constexpr T max_value{std::floating_point<T> ? static_cast<T>(1.) : std::numeric_limits<T>::max()};

				using utils::details::vec::factories<T, SIZE, DERIVED_T>::from;

				inline static constexpr derived_t from(base base, T components_multiplier = max_value, T alpha = max_value)
					{
					derived_t ret;
					if constexpr (static_size >= 1) { ret.r = components_multiplier * (base == base::white || base == base::red || base == base::yellow || base == base::magenta); }
					if constexpr (static_size >= 2) { ret.g = components_multiplier * (base == base::white || base == base::green || base == base::yellow || base == base::cyan); }
					if constexpr (static_size >= 3) { ret.b = components_multiplier * (base == base::white || base == base::blue || base == base::magenta || base == base::cyan); }
					if constexpr (static_size >= 4) { ret.a = alpha; }
					return ret;
					}
			};
		}

#define utils_vec_hell_let_loose

#ifdef utils_vec_hell_let_loose
	// While akshually UB, this usage of unions is explicitly supported by gcc and according to various posts it's used in MS headers, which would imply it's supported by MSVC as well.
	// The advantage is not only simplicity, but also that all operations are supported as you would naively expect them to.
	// For instance, offsetof(y) works as expected, while it doesn't with the other versions.

	// ...and yes I could write all the methods twice, one in the vec base class and the other in the colour base class
	// but why do that when you can overcomplicate your life and waste your time in a funny mess of CRTPs with multiple inheritance? :)

	template <typename T, size_t size>
	struct rgb : details::colour_crtp<T, size, rgb<T, size>> { std::array<T, size> array; };

	utils_disable_warnings_begin
		utils_disable_warning_clang("-Wgnu-anonymous-struct")
		utils_disable_warning_clang("-Wnested-anon-types")
		utils_disable_warning_gcc("-Wpedantic")

		template<typename T> struct rgb<T, 1> : details::colour_crtp<T, 1, rgb<T, 1>> { union { std::array<T, 1> array; struct { T r         ; }; }; };
		template<typename T> struct rgb<T, 2> : details::colour_crtp<T, 2, rgb<T, 2>> { union { std::array<T, 2> array; struct { T r, g      ; }; }; };
		template<typename T> struct rgb<T, 3> : details::colour_crtp<T, 3, rgb<T, 3>> { union { std::array<T, 3> array; struct { T r, g, b   ; }; }; };
		template<typename T> struct rgb<T, 4> : details::colour_crtp<T, 4, rgb<T, 4>> { union { std::array<T, 4> array; struct { T r, g, b, a; }; }; };

	utils_disable_warnings_end
#else
	#error A day may come when the alternatives to the Undefined Behaviour version will be written, when we forsake our bad habits and break all bonds of C, but it is not this day.
#endif

	template <std::floating_point T, bool HAS_ALPHA>
	struct hsv : std::conditional_t<HAS_ALPHA, details::alpha_field<T>, details::empty>
		{
		inline static constexpr T max_value{std::floating_point<T> ? static_cast<T>(1) : std::numeric_limits<T>::max()};
		inline static constexpr T max_angle_value{std::floating_point<T> ? static_cast<T>(1) : static_cast<T>(360)};
		inline static constexpr bool has_alpha{HAS_ALPHA};

		inline static constexpr hsv from(base base, T components_multiplier = max_value, T alpha = max_value)
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

		rgb<T, (HAS_ALPHA ? 4 : 3)> rgb() const noexcept;
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
	template <std::floating_point T, bool has_alpha>
	rgb<T, (has_alpha ? 4 : 3)> hsv<T, has_alpha>::rgb() const noexcept
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
		template <typename T, size_t SIZE>
		inline ::std::ostream& operator<<(::std::ostream& os, const utils::graphics::colour::rgb<T, SIZE>& colour)
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

	template <typename T, size_t SIZE>
	inline ::std::ostream& operator<<(::std::ostream& os, const utils::graphics::colour::rgb<T, SIZE>& colour)
		{
		namespace ucc = utils::console::colour;
		os << ucc::type << "rgb" << SIZE << typeid(T).name();
		return utils::output::typeless::operator<<(os, colour);
		}
	}