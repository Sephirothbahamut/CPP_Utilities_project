#pragma once
#include <cmath>
#include <limits>
#include <cassert>
#include <concepts>
#include <algorithm>

#include "../compilation/warnings.h"
#include "../math/angle.h"
#include "../template_wrappers.h"

#include "../details/vec/common.h"
#include "../details/vec/output.h"
#include "../details/vec/memberwise_operators.h"

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

	template <typename T, bool has_alpha>
	class rgb;

	using rgb_f  = rgb<float  , false>;
	using rgb_d  = rgb<double , false>;
	using rgb_u  = rgb<uint8_t, false>;
	//using rgb  = rgb<float, 3>; //default 
	using rgba_f = rgb<float  , true>;
	using rgba_d = rgb<double , true>;
	using rgba_u = rgb<uint8_t, true>;
	using rgba   = rgb<float  , true>;

	template <std::floating_point T, bool has_alpha>
	struct hsv;

	namespace concepts
		{
		template <typename T, bool has_alpha = false>
		concept rgb = std::same_as<T, colour::rgb<typename T::value_type, has_alpha>>;

		template <typename T>
		concept rgba = rgb<T, true>;

		template <typename T>
		concept hsv = std::same_as<T, colour::hsv<typename T::value_type, T::has_alpha>>;

		template <typename T>
		concept colour = rgb<T> || rgba<T> || hsv<T>;
		}

	namespace details
		{
		template <typename T>
		struct alpha_field { T a; };

		struct empty {};

		inline extern constexpr const char colour_name[]{"rgb"};

		#define utils_vec_hell_let_loose
		#ifdef utils_vec_hell_let_loose

		template <typename T, bool has_alpha>
		struct rgb_named {};

		utils_disable_warnings_begin
			utils_disable_warning_msvc(4201)
			utils_disable_warning_clang("-Wgnu-anonymous-struct")
			utils_disable_warning_clang("-Wnested-anon-types")
			utils_disable_warning_gcc("-Wpedantic")
			template<typename T> struct rgb_named<T, false> { union { std::array<T, 3> array; struct { T r, g, b;    }; }; };
			template<typename T> struct rgb_named<T, true > { union { std::array<T, 4> array; struct { T r, g, b, a; }; }; };
		utils_disable_warnings_end
		#endif
		}

	template<typename T, bool HAS_ALPHA = false>
	class
	#ifdef utils_compiler_msvc
			//Other compilers make empty bases occupy 0, MSVC doesn't always do that without the following line:
			__declspec(empty_bases)
	#endif
	rgb :
		public details::rgb_named<T, HAS_ALPHA>,
		public utils::details::vec::common<T, HAS_ALPHA ? 4 : 3, rgb<T, HAS_ALPHA>, rgb<utils::remove_cvref_t<T>, HAS_ALPHA>>,
		public utils::details::vec::memberwise_operators<utils::details::vec::common<T, HAS_ALPHA ? 4 : 3, rgb<T, HAS_ALPHA>, rgb<utils::remove_cvref_t<T>, HAS_ALPHA>>>//,
		//public utils::details::vec::output<details::colour_name, rgb<T, HAS_ALPHA>>
		{
		public:
			inline static constexpr const size_t static_size{HAS_ALPHA ? 4 : 3};
			using derived_t = rgb<T, HAS_ALPHA>;

			inline static constexpr const bool has_alpha = HAS_ALPHA;
			using range = utils::math::type_based_numeric_range<T>;

		private:
			using arr_t = std::array<T, static_size>;

		public:
			using value_type = typename arr_t::value_type;

#pragma region constructors
			constexpr rgb(base base, T components_multiplier = range::full_value, T alpha = range::full_value)
				{
				this->r = components_multiplier * (base == base::white || base == base::red   || base == base::yellow  || base == base::magenta);
				this->g = components_multiplier * (base == base::white || base == base::green || base == base::yellow  || base == base::cyan);
				this->b = components_multiplier * (base == base::white || base == base::blue  || base == base::magenta || base == base::cyan);
				if constexpr (has_alpha) { this->a = alpha; }
				}

			constexpr rgb() noexcept requires(!has_alpha) : details::rgb_named<T, has_alpha>{.r{0}, .g{0}, .b{0}       } {}
			constexpr rgb() noexcept requires( has_alpha) : details::rgb_named<T, has_alpha>{.r{0}, .g{0}, .b{0}, .a{1}} {}

			template <std::convertible_to<value_type>... Args>
				requires(sizeof...(Args) == 4 && has_alpha)
			constexpr rgb(const Args&... args) : details::rgb_named<T, has_alpha>{.array{static_cast<value_type>(args)...}} {}
			
			template <std::convertible_to<value_type>... Args>
				requires(sizeof...(Args) == 3 && has_alpha)
			constexpr rgb(const Args&... args) : details::rgb_named<T, has_alpha>{.array{static_cast<value_type>(args)..., range::full_value}} {}

			template <std::convertible_to<value_type>... Args>
				requires(sizeof...(Args) == 3 && !has_alpha)
			constexpr rgb(const Args&... args) : details::rgb_named<T, has_alpha>{.array{static_cast<value_type>(args)...}} {}

			constexpr rgb(const value_type& value, const value_type& alpha = range::full_value)
				requires(static_size == 4)
				{
				for (size_t i{0}; i < 4; i++)
					{
					this->array[i] = value;
					}
				this->array[3] = alpha;
				}
			constexpr rgb(const value_type& value)
				requires(static_size <= 3)
				{
				for (size_t i{0}; i < static_size; i++)
					{
					this->array[i] = value;
					}
				}

			template <concepts::rgb other_t>
			constexpr rgb(const other_t& other)
				{
				for (size_t i{0}; i < 3; i++)
					{
					this->array[i] = other_t::range::cast<range>(other[i]);
					}
				if constexpr (has_alpha)
					{
					if constexpr (other_t::has_alpha) { this->a = other.a; }
					else { this->a = range::full_value; }
					}
				}
			template <concepts::rgb other_t>
			constexpr rgb(const other_t& other, T alpha)
				requires(has_alpha)
				{
				for (size_t i{0}; i < 3; i++)
					{
					this->array[i] = other_t::range::cast<range>(other[i]);
					}
				this->a = alpha;
				}
#pragma endregion constructors
				
			rgb<T, true> blend(const rgb<T, true>& foreground) noexcept
				requires(has_alpha)
				{
				rgb<T, true> ret;
				ret.a = 1.f - (1.f - foreground.a) * (1.f - this->a);
				if (ret.a < 1.0e-6f) { return ret; } // Fully transparent -- r,g,b not important
				ret.r = foreground.r * foreground.a / ret.a + this->r * this->a * (1.f - foreground.a) / ret.a;
				ret.g = foreground.g * foreground.a / ret.a + this->g * this->a * (1.f - foreground.a) / ret.a;
				ret.b = foreground.b * foreground.a / ret.a + this->b * this->a * (1.f - foreground.a) / ret.a;
				return ret;
				}

			colour::hsv<std::conditional_t<std::floating_point<T>, T, float>, HAS_ALPHA> hsv() const noexcept;
		};

	template <std::floating_point T, bool HAS_ALPHA>
	struct hsv : std::conditional_t<HAS_ALPHA, details::alpha_field<T>, details::empty>
		{
		inline static constexpr T max_value{std::floating_point<T> ? static_cast<T>(1) : std::numeric_limits<T>::max()};
		inline static constexpr T max_angle_value{std::floating_point<T> ? static_cast<T>(1) : static_cast<T>(360)};
		inline static constexpr bool has_alpha{HAS_ALPHA};
		using range = utils::math::type_based_numeric_range<T>;

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

		rgb<T, HAS_ALPHA> rgb() const noexcept;
		};


	template<typename T, bool HAS_ALPHA>
	hsv<std::conditional_t<std::floating_point<T>, T, float>, HAS_ALPHA> rgb<T, HAS_ALPHA>::hsv() const noexcept
		{//https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
		using floating_t = std::conditional_t<std::floating_point<T>, T, float>;
		using hsv_t = colour::hsv<floating_t, has_alpha>;
		hsv_t out;

		floating_t r{range::cast<hsv_t::range>(this->r)};
		floating_t g{range::cast<hsv_t::range>(this->g)};
		floating_t b{range::cast<hsv_t::range>(this->b)};

		floating_t min{std::min({r, g, b})};
		floating_t max{std::max({r, g, b})};

		out.v = max;                                // v
		floating_t delta{max - min};

		if (delta < 0.00001f)
			{
			out.s = 0.f;
			out.h = 0.f; // undefined, maybe nan?
			return out;
			}
		if (max > 0.0f) 
			{ // NOTE: if Max is == 0, this divide would cause a crash
			out.s = (delta / max);                  // s
			}
		else 
			{
			// if max is 0, then r = g = b = 0              
			// s = 0, h is undefined
			out.s = 0.0;
			out.h = NAN;                            // its now undefined
			return out;
			}
		if (r >= max)                           // > is bogus, just keeps compilor happy
			{
			out.h = (g - b) / delta;        // between yellow & magenta
			}
		else
			{
			if (g >= max)
				{
				out.h = 2.0f + (b - r) / delta;  // between cyan & yellow
				}
			else
				{
				out.h = 4.0f + (r - g) / delta;  // between magenta & cyan
				}
			}
		
		//hue_deg = out.h.value * 60.f; // degrees 0-360
		out.h.value = (out.h.value / 6.f) * hsv_t::max_angle_value; // range 0-1

		if (out.h.value < 0.0f)
			{
			out.h.value += hsv_t::max_angle_value;
			}

		return out;
		}

	template <std::floating_point T, bool has_alpha>
	rgb<T, has_alpha> hsv<T, has_alpha>::rgb() const noexcept
		{
		if (s == 0)
			{
			if constexpr (!has_alpha) { return {v, v, v}; }
			if constexpr ( has_alpha) { return {v, v, v, details::alpha_field<T>::a}; }
			}

		float r, g, b;

		float tmp_h{h.value / h.full_angle};
		tmp_h *= 6.f;

		unsigned i{static_cast<unsigned>(tmp_h)};
		float f{tmp_h - i                };
		float p{v * (1.f - s            )};
		float q{v * (1.f - s *        f )};
		float t{v * (1.f - s * (1.f - f))};

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

//namespace utils::output
//	{
//	namespace typeless
//		{
//		template <typename T, size_t SIZE>
//		inline ::std::ostream& operator<<(::std::ostream& os, const utils::graphics::colour::rgb<T, SIZE>& colour)
//			{
//			namespace ucc = utils::console::colour;
//
//			os << ucc::brace << "(";
//
//			if constexpr (SIZE >= 1) { os                       << ucc::foreground::red   << std::to_string(colour[0]); }
//			if constexpr (SIZE >= 2) { os << ucc::separ << ", " << ucc::foreground::green << std::to_string(colour[1]); }
//			if constexpr (SIZE >= 3) { os << ucc::separ << ", " << ucc::foreground::blue  << std::to_string(colour[2]); }
//			if constexpr (SIZE >= 4) { os << ucc::separ << ", " << ucc::foreground::white << std::to_string(colour[3]); }
//
//			os << ucc::brace << ")";
//
//			return os;
//			}
//		}
//
//	template <typename T, size_t SIZE>
//	inline ::std::ostream& operator<<(::std::ostream& os, const utils::graphics::colour::rgb<T, SIZE>& colour)
//		{
//		namespace ucc = utils::console::colour;
//		os << ucc::type << "rgb" << SIZE << typeid(T).name();
//		return utils::output::typeless::operator<<(os, colour);
//		}
//	}