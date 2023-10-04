#pragma once
#include <ostream>
#include <stdint.h>
#include <cmath> //trigonometry
#include <algorithm> //clamp
#include <variant> //Angle class
#include <concepts>

#include "constants.h"
#include "../math/math.h"
#include "../template_wrappers.h"

//TODO write test cases

namespace utils::math::angle
	{
	template <std::floating_point T, T full_angle_value>
	class base;
	
	template <std::floating_point T = float>
	using deg = base<T, static_cast<T>(360.)>;

	template <std::floating_point T = float>
	using rad = base<T, T{static_cast<T>(2. * constants::PId)}>;

	using degf = deg<float >;
	using radf = rad<float >;
	using degd = deg<double>;
	using radd = rad<double>;

	namespace common
		{
		enum class direction
			{
			rr, ur, up, ul, ll, dl, dw, dr,
			right      = rr,
			up_right   = ur,
			up_left    = ul,
			left       = ll,
			down_left  = dl,
			down       = dw,
			down_right = dr
			};
		enum class hex_flat_top
			{
			ur, up, ul, dl, dw, dr,
			up_right   = ur,
			up_left    = ul,
			down_left  = dl,
			down       = dw,
			down_right = dr
			};
		enum class hex_pointy_top
			{
			rr, ur, ul, ll, dl, dr,
			right      = rr,
			up_right   = ur,
			up_left    = ul,
			left       = ll,
			down_left  = dl,
			down_right = dr
			};
		};

	namespace concepts
		{
		template <typename T>
		concept angle = std::same_as<std::remove_cvref_t<T>, utils::math::angle::base<typename T::value_type, T::full_angle>>;
		}

	template <std::floating_point T = float, T full_angle_value = 1.f >
	class base
		{
		public:
			using value_type = T;
			inline static constexpr value_type full_angle{full_angle_value};
			inline static constexpr value_type half_angle{full_angle / value_type{2}};

			value_type value{ 0.f };

			base() = default;
			base(T value) : value{value} {}

			base(common::direction      dir) : value{                              static_cast<T>(dir) * (full_angle_value / T{8} )} {}
			base(common::hex_flat_top   dir) : value{                              static_cast<T>(dir) * (full_angle_value / T{6} )} {}
			base(common::hex_pointy_top dir) : value{(full_angle_value / T{12}) + (static_cast<T>(dir) * (full_angle_value / T{6}))} {}
			
			// template <value_type other_full_angle>
			// base(const base<value_type, other_full_angle>& src) : value{ (src.value / other_full_angle) * full_angle } {}
			// template <>
			// base<value_type, full_angle_value>(const base<value_type, full_angle_value>& src) : value{src.value} {}

			template <value_type other_full_angle>
			operator base<value_type, other_full_angle>() const noexcept
				{
				if constexpr (other_full_angle == full_angle) { return {value}; }
				else { return {(value / full_angle) * other_full_angle}; }
				}

			base clamp() const noexcept
				{
				if constexpr (full_angle == 1.f) { return {value - std::floor(value)}; }
				else
					{
					value_type new_value{ std::fmod(value, full_angle) };
					if (new_value < 0) { new_value += full_angle; }
					return { new_value };
					}
				}
			base& clamp_self()       noexcept { *this = clamp(); return *this; }

			// Shouldn't be needed because...
			//template <value_type other_full_angle> base  operator+ (const base<other_full_angle> oth) const noexcept { return {value + static_cast<base<full_angle>>(oth).value}; }
			//template <value_type other_full_angle> base  operator- (const base<other_full_angle> oth) const noexcept { return {value - static_cast<base<full_angle>>(oth).value}; }
			//template <value_type other_full_angle> base& operator+=(const base<other_full_angle> oth)       noexcept { return *this = *this + oth; }
			//template <value_type other_full_angle> base& operator-=(const base<other_full_angle> oth)       noexcept { return *this = *this - oth; }

			// ...other should automatically cast here
			//base  operator+ (const base oth) const noexcept { return { value + oth.value }; }
			//base  operator- (const base oth) const noexcept { return { value - oth.value }; }
			//base& operator+=(const base oth)       noexcept { return *this = *this + oth; }
			//base& operator-=(const base oth)       noexcept { return *this = *this - oth; }
			// 
			//bool operator==(const base oth) const noexcept { return clamp().value == oth.clamp().value; }
			//bool operator!=(const base oth) const noexcept { return !(*this == oth); }

			// ...except it doesn't automatically cast, so here we go...
			template <T full_angle_value> constexpr base  operator+ (const base<T, full_angle_value> oth) const noexcept { return {value + static_cast<base<value_type, full_angle>>(oth).value}; }
			template <T full_angle_value> constexpr base  operator- (const base<T, full_angle_value> oth) const noexcept { return {value - static_cast<base<value_type, full_angle>>(oth).value}; }
			template <T full_angle_value> constexpr base& operator+=(const base<T, full_angle_value> oth)       noexcept { return *this = *this + oth; }
			template <T full_angle_value> constexpr base& operator-=(const base<T, full_angle_value> oth)       noexcept { return *this = *this - oth; }
			template <T full_angle_value> constexpr bool  operator==(const base<T, full_angle_value> oth) const noexcept { return clamp().value == static_cast<base<value_type, full_angle>>(oth).clamp().value; }
			template <T full_angle_value> constexpr bool  operator!=(const base<T, full_angle_value> oth) const noexcept { return !(*this == oth); }

			constexpr base  operator* (value_type oth) const noexcept { return { value * oth }; }
			constexpr base  operator/ (value_type oth) const noexcept { return { value / oth }; }
			constexpr base& operator*=(value_type oth)       noexcept { return *this = *this * oth; }
			constexpr base& operator/=(value_type oth)       noexcept { return *this = *this / oth; }

			constexpr base  operator-()const noexcept { return base{ value + half_angle }.clamp(); }

			/// <summary>
			/// Distance within a full angle. Disregards differences larger than one full angle.
			/// </summary>
			/// <param name="a"></param>
			/// <param name="b"></param>
			/// <returns></returns>
			static constexpr base min_distance(const base a, const base b) noexcept
				{
				if constexpr (std::is_integral_v<value_type>)
					{
					T d{(b.value - a.value) % full_angle};
					return d < -half_angle ? d + full_angle : d > half_angle ? d - full_angle : d;
					}
				else if constexpr (std::is_floating_point_v<value_type>)
					{
					T d{std::modf((b.value - a.value), full_angle)};
					return d < -half_angle ? d + full_angle : d > half_angle ? d - full_angle : d;
					}
				}

			constexpr value_type normalize_in_range(base min, base max) const noexcept
				{
				min.clamp();
				max.clamp();
				value_type next_max{ min.value <= max.value ? max.value : max.value + full_angle };
				value_type min_to_this{ (min.value <= value ? value : value + full_angle) - min.value };
				value_type min_to_max{ next_max - min.value };
				if (min_to_this < min_to_max) { return min_to_this / min_to_max; }

				base halfway{ (next_max + min.value) * .5f };
				base opposite{ -halfway };
				value_type next_opposite{ next_max < opposite.value ? opposite.value : opposite.value + full_angle };
				if (next_opposite < next_max) { next_opposite += full_angle; }

				value_type min_to_opposite{ next_opposite - min.value };
				return min_to_this < min_to_opposite ? min_to_this / min_to_max : (min_to_this - full_angle) / min_to_max;
				}

	#pragma region Trigonometry
			inline        constexpr value_type sin  (                          ) const noexcept { return std::sin(rad_value()); }
			inline        constexpr value_type cos  (                          ) const noexcept { return std::cos(rad_value()); }
			inline        constexpr value_type tan  (                          ) const noexcept { return std::tan(rad_value()); }
			inline static constexpr base       asin (value_type n              )       noexcept { return { base<value_type, value_type{2} * constants::PIf>{std::asin(n)} }; }
			inline static constexpr base       acos (value_type n              )       noexcept { return { base<value_type, value_type{2} * constants::PIf>{std::acos(n)} }; }
			inline static constexpr base       atan (value_type n              )       noexcept { return { base<value_type, value_type{2} * constants::PIf>{std::atan(n)} }; }
			inline static constexpr base       atan2(value_type a, value_type b)       noexcept { return { base<value_type, value_type{2} * constants::PIf>{std::atan2(a, b)} }; }
	#pragma endregion Trigonometry

		private:
			constexpr value_type rad_value() const noexcept { return static_cast<base<value_type, 2.f * constants::PIf>>(*this).value; }
		};
	
	namespace literals
		{
		inline degf operator""_deg  (         long double value) noexcept { return angle::degf{static_cast<float>(value)}; }
		inline degf operator""_deg  (unsigned long long   value) noexcept { return angle::degf{static_cast<float>(value)}; }
		inline radf operator""_rad  (         long double value) noexcept { return angle::radf{static_cast<float>(value)}; }
		inline radf operator""_rad  (unsigned long long   value) noexcept { return angle::radf{static_cast<float>(value)}; }
		inline radf operator""_radpi(         long double value) noexcept { return angle::radf{static_cast<float>(value) * constants::PIf}; }
		inline radf operator""_radpi(unsigned long long   value) noexcept { return angle::radf{static_cast<float>(value) * constants::PIf}; }
		}
	}

namespace utils::math
	{
	//template <float full_angle_value = 1.f>
	//inline math::angle::base_angle<full_angle_value> lerp(math::angle::base_angle<full_angle_value> a, math::angle::base_angle<full_angle_value> b, float t)
	//	{
	//	return { utils::lerp(a.value, b.value, t) };
	//	}
	template <utils::math::angle::concepts::angle angle_t>
	inline constexpr angle_t clamp(angle_t in, angle_t min_in, angle_t max_in)
		{
		const angle_t min{min_in.clamp()};
		const angle_t max{max_in.clamp()};

		using value_type = angle_t::value_type;

		value_type next_max   { min.value <= max.value ? max.value : max.value + angle_t::full_angle             };
		value_type min_to_this{(min.value <= in .value ? in.value  : in .value + angle_t::full_angle) - min.value};
		value_type min_to_max {next_max - min.value};
		if (min_to_this < min_to_max) { return in; }

		angle_t halfway{(next_max + min.value) * value_type{.5}};
		angle_t opposite{-halfway};
		value_type next_opposite{next_max < opposite.value ? opposite.value : opposite.value + angle_t::full_angle};
		if (next_opposite < next_max) { next_opposite += angle_t::full_angle; }

		value_type min_to_opposite{next_opposite - min.value};
		return min_to_this < min_to_opposite ? max : min;
		}
	
	namespace trigonometry
		{
		template <math::angle::concepts::angle T> inline constexpr float                                      sin  (const T& a      ) noexcept { return a.sin(); }
		template <math::angle::concepts::angle T> inline constexpr float                                      cos  (const T& a      ) noexcept { return a.cos(); }
		template <math::angle::concepts::angle T> inline constexpr float                                      tan  (const T& a      ) noexcept { return a.tan        (    ); }
		template <std ::floating_point         T> inline constexpr math::angle::base<T, 2.f * constants::PIf> asin (float n         ) noexcept { return angle::rad<T>::asin (n   ); }
		template <std ::floating_point         T> inline constexpr math::angle::base<T, 2.f * constants::PIf> acos (float n         ) noexcept { return angle::rad<T>::acos (n   ); }
		template <std ::floating_point         T> inline constexpr math::angle::base<T, 2.f * constants::PIf> atan (float n         ) noexcept { return angle::rad<T>::atan (n   ); }
		template <std ::floating_point         T> inline constexpr math::angle::base<T, 2.f * constants::PIf> atan2(float a, float b) noexcept { return angle::rad<T>::atan2(a, b); }
		}
	}

//namespace utils::output
//	{
//	namespace typeless
//		{
//		template <std::floating_point T, T full_angle_value>
//		inline ::std::ostream& operator<<(::std::ostream& os, const utils::math::angle::base<T, full_angle_value>& angle)
//			{
//			namespace ucc = utils::console::colour;
//
//			utils::output::operator<<(os, ucc::value); //TODO why doesn't work without explicit operator<<?
//
//			//os << ucc::value;
//			if constexpr (full_angle_value == 2.f * constants::PIf) { os << (angle.value / constants::PIf); }
//			else { os << angle.value; }
//
//			utils::output::operator<<(os, ucc::type); //TODO why doesn't work without explicit operator<<?
//			if      constexpr (full_angle_value == T{360})               { os << "deg"; }
//			else if constexpr (full_angle_value == 2.f * constants::PIf) { os << "pirad"; }
//			else                                               { os << "/" << full_angle_value; }
//			return os;
//			}
//		}
//	
//	template <std::floating_point T, T full_angle_value>
//	inline ::std::ostream& operator<<(::std::ostream& os, const utils::math::angle::base<T, full_angle_value>& angle)
//		{
//		namespace ucc = utils::console::colour;
//		utils::output::typeless::operator<<(os, angle); 
//		return os << "_" << typeid(T).name();
//		}
//	}