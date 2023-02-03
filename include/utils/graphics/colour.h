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

		#define utils_vec_hell_let_loose
		#ifdef utils_vec_hell_let_loose
		// While akshually UB, this usage of unions is explicitly supported by gcc and according to various posts it's used in MS headers, which would imply it's supported by MSVC as well.
		// The advantage is not only simplicity, but also that all operations are supported as you would naively expect them to.
		// For instance, offsetof(y) works as expected, while it doesn't with the other versions.

		template <typename T, size_t size>
		struct rgb_named { std::array<T, size> array; };

		utils_disable_warnings_begin
			utils_disable_warning_clang("-Wgnu-anonymous-struct")
			utils_disable_warning_clang("-Wnested-anon-types")
			utils_disable_warning_gcc("-Wpedantic")

			template<typename T> struct rgb_named<T, 1> { union { std::array<T, 1> array; struct { T r;          }; }; };
			template<typename T> struct rgb_named<T, 2> { union { std::array<T, 2> array; struct { T r, g;       }; }; };
			template<typename T> struct rgb_named<T, 3> { union { std::array<T, 3> array; struct { T r, g, b;    }; }; };
			template<typename T> struct rgb_named<T, 4> { union { std::array<T, 4> array; struct { T r, g, b, a; }; }; };
		utils_disable_warnings_end
		#endif
		}

	template<typename T, size_t size>
	class rgb :
		public details::rgb_named<T, size>,
		public utils::details::vec::common<T, size, rgb<T, size>>,
		public utils::details::vec::memberwise_operators<T, size, rgb, rgb<T, size>>,
		public utils::details::vec::output<details::colour_name, rgb<T, size>>
		{
		public:
			using derived_t = rgb<T, size>;

		private:

			using arr_t = std::array<T, size>;

		public:
			inline static constexpr const size_t static_size{size};
			using value_type = typename arr_t::value_type;
			inline static constexpr const T max_value{std::floating_point<T> ? static_cast<T>(1.) : std::numeric_limits<T>::max()};

#pragma region constructors
			constexpr rgb(base base, T components_multiplier = max_value, T alpha = max_value)
				{
				if constexpr (static_size >= 1) { this->r = components_multiplier * (base == base::white || base == base::red   || base == base::yellow || base == base::magenta); }
				if constexpr (static_size >= 2) { this->g = components_multiplier * (base == base::white || base == base::green || base == base::yellow || base == base::cyan); }
				if constexpr (static_size >= 3) { this->b = components_multiplier * (base == base::white || base == base::blue  || base == base::magenta || base == base::cyan); }
				if constexpr (static_size >= 4) { this->a = alpha; }
				}
			
			template <std::convertible_to<value_type>... Args>
				requires(sizeof...(Args) >= static_size)
			constexpr rgb(const Args&... args) : details::rgb_named<T, size>{.array{static_cast<value_type>(args)...}} {}

			template <std::convertible_to<value_type>... Args>
				requires(sizeof...(Args) < static_size)
			constexpr rgb(const Args&... args) : details::rgb_named<T, size>{.array{static_cast<value_type>(args)...}} 
				{
				for (size_t i = sizeof...(Args); i < static_size; i++)
					{
					if constexpr (sizeof...(Args)) { this->array[i] = this->array[sizeof...(Args) - 1]; }
					else { this->array[i] = T{0}; }
					}
				}

			template <concepts::rgb other_t>
				requires(std::convertible_to<typename other_t::value_type, value_type> && other_t::static_size == static_size)
			constexpr rgb(const other_t& other) : details::rgb_named<T, size>{.array{std::apply([](const auto&... values) { return std::array<value_type, size>{static_cast<value_type>(values)...}; }, other.array)}} {}
			
			template <concepts::rgb other_t>
				requires(std::convertible_to<typename other_t::value_type, value_type> && other_t::static_size != static_size && utils::concepts::default_constructible<value_type>)
			constexpr rgb(const other_t& other, value_type default_value = value_type{0})
				{
				size_t i{0};
				for (; i < std::min(static_size, other_t::static_size); i++)
					{
					this->array[i] = static_cast<value_type>(other[i]);
					}
				for (size_t i = other.size(); i < static_size; i++)
					{
					if constexpr (other_t::static_size) { this->array[i] = default_value; }
					else { this->array[i] = default_value; }
					}
				}
#pragma endregion constructors
		};

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