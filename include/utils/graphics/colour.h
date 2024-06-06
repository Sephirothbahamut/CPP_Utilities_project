#pragma once
#include <cmath>
#include <limits>
#include <cassert>
#include <concepts>
#include <algorithm>

#include "../compilation/warnings.h"
#include "../compilation/gpu.h"
#include "../math/math.h"
#include "../math/angle.h"
#include "../template_wrappers.h"
#include "../oop/conditional_inheritance.h"

#include "../details/vec/all.h"

// colour conversions from: https://axonflux.com/handy-rgb-to-hsl-and-rgb-to-hsv-color-model-c

//TODO write test cases

namespace utils::graphics::colour
	{
	enum class base
		{
		black , white, 
		red   , green, blue   , 
		yellow, cyan , magenta, 
		};

	namespace details
		{
		template <utils::math::concepts::undecorated_number T, size_t size>
		struct rgb;

		template <utils::math::concepts::undecorated_floating_point T, size_t size>
		struct hsv;
		}

	template <utils::math::concepts::undecorated_number T = float, bool has_alpha = false>
	using rgb    = details::rgb<T, has_alpha ? 4 : 3>;
	using rgba   = rgb<float  , true >;
	using rgb_f  = rgb<float  , false>;
	using rgb_d  = rgb<double , false>;
	using rgb_u  = rgb<uint8_t, false>;
	using rgba_f = rgb<float  , true >;
	using rgba_d = rgb<double , true >;
	using rgba_u = rgb<uint8_t, true >;

	template <utils::math::concepts::undecorated_floating_point T = float, bool has_alpha = false>
	using hsv    = details::hsv<T, has_alpha ? 4 : 3>;
	using hsva   = hsv<float , true >;
	using hsv_f  = hsv<float , false>;
	using hsva_f = hsv<float , true >;
	using hsv_d  = hsv<double, false>;
	using hsva_d = hsv<double, true >;

	namespace concepts
		{
		template <typename T>
		concept rgb = std::same_as<T, colour::rgb<typename T::value_type, T::static_has_alpha>>;
		template <typename T>
		concept rgba = std::same_as<T, colour::rgb<typename T::value_type, true>>;

		template <typename T>
		concept hsv = std::same_as<T, colour::hsv<typename T::value_type, T::static_has_alpha>>;
		template <typename T>
		concept hsva = std::same_as<T, colour::hsv<typename T::value_type, true>>;

		template <typename T>
		concept colour = rgb<T> || hsv<T>;
		}

	namespace details
		{
		inline extern constexpr const char name_rgb[]{"rgb"};
		inline extern constexpr const char name_hsv[]{"rgb"};

		template<utils::math::concepts::undecorated_number T, size_t size>
		struct utils_oop_empty_bases rgb : ::utils::details::vector::base<T, size, rgb, details::name_rgb>
			{
			static_assert(size == 3 || size == 4);

			using base_t = ::utils::details::vector::base<T, size, rgb, details::name_rgb>;
			using base_t::static_size; 
			using base_t::static_value_is_reference;
			using typename base_t::self_t;
			using typename base_t::value_type;
			using typename base_t::nonref_value_type;
			using typename base_t::nonref_self_t;

			using range = utils::math::type_based_numeric_range<nonref_value_type>;
			inline static constexpr bool static_has_alpha{size == 4};

			using base_t::base;

			utils_gpu_available constexpr const nonref_value_type& r() const noexcept { return (*this)[0]; }
			utils_gpu_available constexpr       nonref_value_type& r()       noexcept { return (*this)[0]; }
			utils_gpu_available constexpr const nonref_value_type& g() const noexcept { return (*this)[1]; }
			utils_gpu_available constexpr       nonref_value_type& g()       noexcept { return (*this)[1]; }
			utils_gpu_available constexpr const nonref_value_type& b() const noexcept { return (*this)[2]; }
			utils_gpu_available constexpr       nonref_value_type& b()       noexcept { return (*this)[2]; }
			utils_gpu_available constexpr const nonref_value_type& a() const noexcept { if constexpr (static_size == 4) { return (*this)[3]; } else { return range::full_value; } }
			utils_gpu_available constexpr       nonref_value_type& a()       noexcept requires(static_has_alpha) { return (*this)[3]; }

			utils_gpu_available constexpr rgb(base base, nonref_value_type components_multiplier = range::full_value, nonref_value_type alpha = range::full_value) noexcept
				requires(!static_value_is_reference)
				{
				r() = components_multiplier * (base == base::white || base == base::red   || base == base::yellow  || base == base::magenta);
				g() = components_multiplier * (base == base::white || base == base::green || base == base::yellow  || base == base::cyan);
				b() = components_multiplier * (base == base::white || base == base::blue  || base == base::magenta || base == base::cyan);
				if constexpr (static_has_alpha) { a() = alpha; }
				}

			utils_gpu_available constexpr rgb(const concepts::rgb auto& other) noexcept requires(!static_value_is_reference)
				{
				for (size_t i = 0; i < 3; i++)
					{
					using other_range = std::remove_cvref_t<decltype(other)>::range;
					(*this)[i] = other_range::template cast_to<range>(other[i]);
					}
				if constexpr (static_has_alpha) { a() = other.a(); }
				}

			utils_gpu_available constexpr rgb(const concepts::hsv auto& hsv) noexcept requires(!static_value_is_reference);

			utils_gpu_available constexpr nonref_self_t blend(const concepts::colour auto& foreground) const noexcept
				requires(std::remove_cvref_t<decltype(foreground)>::static_has_alpha)
				{
				using floating_t = std::conditional_t<math::concepts::undecorated_floating_point<nonref_value_type>, nonref_value_type, float>;
				using rgba_t = rgb<floating_t, 4>;
				const rgba_t remapped_foreground{foreground};
				const rgba_t remapped_self{*this};

				rgba_t remapped_ret;

				const nonref_value_type multiplier_a{[this]() -> nonref_value_type
					{
					if constexpr (static_has_alpha) 
						{
						return (static_cast<nonref_value_type>(1) - a()); 
						}
					else 
						{ 
						return static_cast<nonref_value_type>(1);
						} 
					}()};

				const floating_t ret_a{floating_t{1.f} - (floating_t{1.f} - remapped_foreground.a()) * (floating_t{1.f} - remapped_self.a())};
				if (ret_a < utils::math::constants::epsilon) { return {}; } // Fully transparent -- r,g,b not important

				for (size_t i{0}; i < 3; i++)
					{
					remapped_ret[i] = remapped_foreground[i] * remapped_foreground.a() / ret_a + remapped_self[i] * remapped_self.a() * (1.f - remapped_foreground.a()) / ret_a;
					}
				if constexpr (static_has_alpha) { remapped_ret.a() = ret_a; }

				const nonref_self_t ret{remapped_ret};
				return ret;
				}

			//utils_gpu_available constexpr rgb<T, 4> test_blend(const rgb<T, 4>& foreground) const noexcept
			//	requires(has_alpha)
			//	{
			//	rgb<T, 4> ret;
			//	ret.a() = utils::math::min(1.f, a() + foreground.a());
			//	const float remaining_a{1.f - foreground.a()};
			//	const float background_a{utils::math::min(remaining_a, a())};
			//
			//	for (size_t i{0}; i < 3; i++)
			//		{
			//		ret[i] = (foreground[i] * foreground.a()) + (background_a * (*this)[i]);
			//		}
			//
			//	return ret;
			//	}
			};

		template <utils::math::concepts::undecorated_floating_point T, size_t size>
		struct hsv : ::utils::details::vector::base<T, size, hsv, details::name_hsv>
			{
			static_assert(size == 3 || size == 4);

			using base_t = ::utils::details::vector::base<T, size, hsv, details::name_hsv>;
			using base_t::static_size;
			using base_t::static_value_is_reference;
			using typename base_t::self_t;
			using typename base_t::value_type;
			using typename base_t::nonref_value_type;
			using typename base_t::nonref_self_t;

			using range = utils::math::type_based_numeric_range<T>;
			inline static constexpr bool static_has_alpha{size == 4};

			using base_t::base;

			utils_gpu_available constexpr hsv(base base, T components_multiplier = range::full_value, T alpha = range::full_value)
				requires(!static_value_is_reference)
				{
				if (base != base::black && components_multiplier != T{0.f}) 
					{
					utils::math::angle::degf angle{0.f};
					using namespace utils::math::angle::literals;
					switch (base)
						{
						case base::black  : h() =   0_deg; break;
						case base::white  : h() =   0_deg; break;
						case base::red    : h() =   0_deg; break;
						case base::green  : h() = 120_deg; break;
						case base::blue   : h() = 240_deg; break;
						case base::yellow : h() =  60_deg; break;
						case base::cyan   : h() = 180_deg; break;
						case base::magenta: h() = 300_deg; break;
						}

					s() = range::full_value; //TODO make correct calculation
					v() = range::full_value; //TODO make correct calculation
					}
				else { h() = value_type{0.f}; s() = value_type{0.f}; v() = value_type{0.f}; }
				if constexpr (static_has_alpha) { a() = alpha; }
				}

			utils_gpu_available constexpr hsv(const concepts::hsv auto& other) noexcept requires(!static_value_is_reference)
				{
				for (size_t i = 0; i < 3; i++)
					{
					using other_range = std::remove_cvref_t<decltype(other)>::range;
					(*this)[i] = other_range::template convert_to<range>(other[i]);
					}
				if constexpr (static_has_alpha) { a() = other.a(); }
				}

			utils_gpu_available constexpr hsv(const concepts::rgb auto& rgb) noexcept requires(!static_value_is_reference);
			
			utils_gpu_available constexpr ::utils::math::angle::base<const nonref_value_type&, range::full_value> h() const noexcept { return {(*this)[0]}; }
			utils_gpu_available constexpr ::utils::math::angle::base<      nonref_value_type&, range::full_value> h()       noexcept { return {(*this)[0]}; }
			utils_gpu_available constexpr const nonref_value_type& s() const noexcept { return (*this)[1]; }
			utils_gpu_available constexpr       nonref_value_type& s()       noexcept { return (*this)[1]; }
			utils_gpu_available constexpr const nonref_value_type& v() const noexcept { return (*this)[2]; }
			utils_gpu_available constexpr       nonref_value_type& v()       noexcept { return (*this)[2]; }
			utils_gpu_available constexpr const nonref_value_type& a() const noexcept { if constexpr (static_size == 4) { return (*this)[3]; } else { return range::full_value; } }
			utils_gpu_available constexpr       nonref_value_type& a()       noexcept requires(static_has_alpha) { return (*this)[3]; }
			};

		template<utils::math::concepts::undecorated_number T, size_t size>
		utils_gpu_available constexpr rgb<T, size>::rgb(const concepts::hsv auto& hsv) noexcept requires(!static_value_is_reference)
			{
			if (hsv.s() == 0)
				{
				for (size_t i{0}; i < 3; i++)
					{
					(*this) = hsv.v();
					}
				}
			else
				{
				float tmp_r, tmp_g, tmp_b;

				float tmp_h{hsv.h().value};
				tmp_h *= 6.f;

				const unsigned i{static_cast<unsigned>(tmp_h)};
				const float f{tmp_h - i                };
				const float v{hsv.v()};
				const float p{v * (1.f - hsv.s()            )};
				const float q{v * (1.f - hsv.s() *        f )};
				const float t{v * (1.f - hsv.s() * (1.f - f))};

				switch (i % 6)
					{
					case 0: tmp_r = v, tmp_g = t, tmp_b = p; break;
					case 1: tmp_r = q, tmp_g = v, tmp_b = p; break;
					case 2: tmp_r = p, tmp_g = v, tmp_b = t; break;
					case 3: tmp_r = p, tmp_g = q, tmp_b = v; break;
					case 4: tmp_r = t, tmp_g = p, tmp_b = v; break;
					case 5: tmp_r = v, tmp_g = p, tmp_b = q; break;
					default: //Actually unreachable, but the compiler doesn't realize that 0 <= i < 6, so it thinks tmp_r, tmp_g and tmp_b aren't always initialized
						tmp_r = 0, tmp_g = 0, tmp_b = 0;
					}

				r() = tmp_r;
				g() = tmp_g;
				b() = tmp_b;
				}

			if constexpr (static_has_alpha) { a() = hsv.a(); }
			}

		template<utils::math::concepts::undecorated_floating_point T, size_t size>
		utils_gpu_available constexpr hsv<T, size>::hsv(const concepts::rgb auto& rgb) noexcept requires(!static_value_is_reference)
			{//https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
			using from_t = std::remove_cvref_t<decltype(rgb)>;
			const colour::rgb<value_type, static_has_alpha> remapped_rgb{rgb};

			const value_type min{std::min({remapped_rgb.r(), remapped_rgb.g(), remapped_rgb.b()})};
			const value_type max{std::max({remapped_rgb.r(), remapped_rgb.g(), remapped_rgb.b()})};

			v() = max;                                // v
			const value_type delta{max - min};

			value_type tmp_hue{static_cast<value_type>(0.)};

			if (delta < 0.00001f)
				{
				s() = 0.f;
				tmp_hue = 0.f; // undefined, maybe nan?
				}
			if (max > 0.0f) 
				{ // NOTE: if Max is == 0, this divide would cause a crash
				s() = (delta / max);                  // s
				}
			else 
				{
				// if max is 0, then r = g = b = 0              
				// s = 0, h is undefined
				s() = 0.0;
				tmp_hue = std::numeric_limits<value_type>::quiet_NaN(); // its now undefined
				}
			if (remapped_rgb.r() >= max)                           // > is bogus, just keeps compilor happy
				{
				tmp_hue = (remapped_rgb.g() - remapped_rgb.b()) / delta;        // between yellow & magenta
				}
			else
				{
				if (remapped_rgb.g() >= max)
					{
					tmp_hue = static_cast<value_type>(2.) + (remapped_rgb.b() - remapped_rgb.r()) / delta;  // between cyan & yellow
					}
				else
					{
					tmp_hue = static_cast<value_type>(4.) + (remapped_rgb.r() - remapped_rgb.g()) / delta;  // between magenta & cyan
					}
				}
		
			tmp_hue = (tmp_hue / 6.f) * range::full_value;
			h().value = tmp_hue;

			h().clamp_self();

			if constexpr (static_has_alpha) { a() = rgb.a(); }
			}
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