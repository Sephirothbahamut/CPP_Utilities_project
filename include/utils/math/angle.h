#pragma once
#include <ostream>
#include <stdint.h>
#include <cmath> //trigonometry
#include <algorithm> //clamp
#include <variant> //Angle class
#include <concepts>

#include "../compilation/gpu.h"
#include "constants.h"
#include "math.h"
#include "../template_wrappers.h"
#include "../memory.h"

//TODO write test cases

namespace utils::math::angle
	{
	template <utils::math::concepts::undecorated_number T, utils::remove_reference_t<T> full_angle_value>
	class base;
	
	template <utils::math::concepts::undecorated_number T = float>
	using deg = base<T, static_cast<utils::remove_reference_t<T>>(360.)>;

	template <utils::math::concepts::undecorated_number T = float>
	using rad = base<T, T{static_cast<utils::remove_reference_t<T>>(2. * constants::PId)}>;

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

		template <typename T, typename to>
		concept compatible_angle = angle<T> && angle<to> && std::convertible_to<typename T::nonref_value_type, typename to::nonref_value_type>;
		}

	template <utils::math::concepts::undecorated_number T = float, utils::remove_reference_t<T> full_angle_value = 1.f >
	class base
		{
		public:
			using value_type = T;
			using self_t = base<T, full_angle_value>;
			using nonref_value_type = utils::remove_reference_t<value_type>;
			using nonref_self_t = base<nonref_value_type, full_angle_value>;

			inline static constexpr const bool static_value_is_reference{utils::concepts::reference<T>};

			inline static constexpr nonref_value_type full_angle{full_angle_value};
			inline static constexpr nonref_value_type half_angle{full_angle / static_cast<nonref_value_type>(2.)};

			value_type value{ 0.f };

			utils_gpu_available constexpr base() requires(!static_value_is_reference) = default;
			utils_gpu_available constexpr base(const nonref_value_type& value) requires(!static_value_is_reference) : value{value} {}

			utils_gpu_available constexpr base(common::direction      dir) requires(!static_value_is_reference) : value{                                                           static_cast<nonref_value_type>(dir) * (full_angle_value / static_cast<nonref_value_type>(8)) } {}
			utils_gpu_available constexpr base(common::hex_flat_top   dir) requires(!static_value_is_reference) : value{                                                           static_cast<nonref_value_type>(dir) * (full_angle_value / static_cast<nonref_value_type>(6)) } {}
			utils_gpu_available constexpr base(common::hex_pointy_top dir) requires(!static_value_is_reference) : value{(full_angle_value / static_cast<nonref_value_type>(12)) + (static_cast<nonref_value_type>(dir) * (full_angle_value / static_cast<nonref_value_type>(6)))} {}

			utils_gpu_available constexpr base(nonref_value_type& value) requires(static_value_is_reference) : value{value} {}

			// template <value_type other_full_angle>
			// base(const base<value_type, other_full_angle>& src) : value{ (src.value / other_full_angle) * full_angle } {}
			// template <>
			// base<value_type, full_angle_value>(const base<value_type, full_angle_value>& src) : value{src.value} {}

			template <nonref_value_type other_full_angle>
			utils_gpu_available constexpr base(const base<nonref_value_type, other_full_angle>& other) noexcept
				requires(!static_value_is_reference)
				{
				if constexpr (other_full_angle == full_angle) 
					{
					value = other.value; 
					}
				else 
					{
					value = (other.value / other_full_angle) * full_angle; 
					}
				}

			template <nonref_value_type other_full_angle>
			utils_gpu_available constexpr self_t& operator=(const base<nonref_value_type, other_full_angle>& other) noexcept
				{
				const nonref_self_t remapped_other{other};
				value = remapped_other.value;
				return *this;
				}

			utils_gpu_available constexpr nonref_self_t clamp() const noexcept
				{
				if constexpr (full_angle == static_cast<nonref_value_type>(1.f)) { return {value - std::floor(value)}; }
				else
					{
					nonref_value_type new_value{ std::fmod(value, full_angle) };
					if (new_value < 0) { new_value += full_angle; }
					return { new_value };
					}
				}
			utils_gpu_available constexpr self_t& clamp_self() noexcept { value = clamp().value; return *this; }

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
			utils_gpu_available constexpr nonref_self_t  operator+ (const concepts::compatible_angle<self_t> auto& other) const noexcept { return {value + nonref_self_t{other}.value}; }
			utils_gpu_available constexpr nonref_self_t  operator- (const concepts::compatible_angle<self_t> auto& other) const noexcept { return {value - nonref_self_t{other}.value}; }
			utils_gpu_available constexpr self_t       & operator+=(const concepts::compatible_angle<self_t> auto& other)       noexcept { return *this = *this + other; }
			utils_gpu_available constexpr self_t       & operator-=(const concepts::compatible_angle<self_t> auto& other)       noexcept { return *this = *this - other; }
			utils_gpu_available constexpr bool           operator==(const concepts::compatible_angle<self_t> auto& other) const noexcept { return clamp().value == nonref_self_t{other}.clamp().value; }
			utils_gpu_available constexpr bool           operator!=(const concepts::compatible_angle<self_t> auto& other) const noexcept { return !(*this == other); }
			
			utils_gpu_available constexpr nonref_self_t  operator+ (const nonref_value_type& other) const noexcept { return { value + other }; }
			utils_gpu_available constexpr nonref_self_t  operator- (const nonref_value_type& other) const noexcept { return { value - other }; }
			utils_gpu_available constexpr nonref_self_t  operator* (const nonref_value_type& other) const noexcept { return { value * other }; }
			utils_gpu_available constexpr nonref_self_t  operator/ (const nonref_value_type& other) const noexcept { return { value / other }; }
			utils_gpu_available constexpr self_t       & operator+=(const nonref_value_type& other)       noexcept { return *this = *this + other; }
			utils_gpu_available constexpr self_t       & operator-=(const nonref_value_type& other)       noexcept { return *this = *this - other; }
			utils_gpu_available constexpr self_t       & operator*=(const nonref_value_type& other)       noexcept { return *this = *this * other; }
			utils_gpu_available constexpr self_t       & operator/=(const nonref_value_type& other)       noexcept { return *this = *this / other; }

			utils_gpu_available constexpr nonref_self_t operator-()const noexcept { return nonref_self_t{value + half_angle}.clamp(); }

			/// <summary>
			/// Distance within a full angle. Disregards differences larger than one full angle.
			/// </summary>
			/// <param name="a"></param>
			/// <param name="b"></param>
			/// <returns></returns>
			utils_gpu_available static constexpr nonref_self_t min_distance(const self_t& a, const concepts::compatible_angle<self_t> auto& b) noexcept
				{
				const nonref_self_t converted_b{b};
				if constexpr (std::is_integral_v<nonref_value_type>)
					{
					nonref_value_type d{(converted_b.value - a.value) % full_angle};
					return d < -half_angle ? d + full_angle : d > half_angle ? d - full_angle : d;
					}
				else if constexpr (std::is_floating_point_v<nonref_value_type>)
					{
					nonref_value_type d{std::modf((converted_b.value - a.value), full_angle)};
					return d < -half_angle ? d + full_angle : d > half_angle ? d - full_angle : d;
					}
				}

			utils_gpu_available constexpr nonref_value_type normalize_in_range(const concepts::compatible_angle<self_t> auto& min, const concepts::compatible_angle<self_t> auto& max) const noexcept
				{
				nonref_self_t converted_min{min};
				nonref_self_t converted_max{max};
				converted_min.clamp();
				converted_max.clamp();
				nonref_value_type next_max{ converted_min.value <= converted_max.value ? converted_max.value : converted_max.value + full_angle };
				nonref_value_type min_to_this{ (converted_min.value <= value ? value : value + full_angle) - converted_min.value };
				nonref_value_type min_to_max{ next_max - converted_min.value };
				if (min_to_this < min_to_max) { return min_to_this / min_to_max; }

				nonref_self_t halfway{ (next_max + converted_min.value) * .5f };
				nonref_self_t opposite{ -halfway };
				nonref_value_type next_opposite{ next_max < opposite.value ? opposite.value : opposite.value + full_angle };
				if (next_opposite < next_max) { next_opposite += full_angle; }

				nonref_value_type min_to_opposite{ next_opposite - converted_min.value };
				return min_to_this < min_to_opposite ? min_to_this / min_to_max : (min_to_this - full_angle) / min_to_max;
				}

	#pragma region Trigonometry
			utils_gpu_available inline        constexpr nonref_value_type sin  (                                        ) const noexcept { return std::sin(rad_value()); }
			utils_gpu_available inline        constexpr nonref_value_type cos  (                                        ) const noexcept { return std::cos(rad_value()); }
			utils_gpu_available inline        constexpr nonref_value_type tan  (                                        ) const noexcept { return std::tan(rad_value()); }
			utils_gpu_available inline static constexpr nonref_self_t     asin (nonref_value_type n                     )       noexcept { return { base<nonref_value_type, static_cast<nonref_value_type>(2. * constants::PId)>{std::asin(n)} }; }
			utils_gpu_available inline static constexpr nonref_self_t     acos (nonref_value_type n                     )       noexcept { return { base<nonref_value_type, static_cast<nonref_value_type>(2. * constants::PId)>{std::acos(n)} }; }
			utils_gpu_available inline static constexpr nonref_self_t     atan (nonref_value_type n                     )       noexcept { return { base<nonref_value_type, static_cast<nonref_value_type>(2. * constants::PId)>{std::atan(n)} }; }
			utils_gpu_available inline static constexpr nonref_self_t     atan2(nonref_value_type a, nonref_value_type b)       noexcept { return { base<nonref_value_type, static_cast<nonref_value_type>(2. * constants::PId)>{std::atan2(a, b)} }; }
	#pragma endregion Trigonometry

		private:
			utils_gpu_available constexpr nonref_value_type rad_value() const noexcept { return static_cast<base<nonref_value_type, static_cast<nonref_value_type>(2. * constants::PId)>>(*this).value; }
		};
	
	namespace literals
		{
		utils_gpu_available inline degf operator""_deg   (         long double value) noexcept { return angle::degf{static_cast<float> (value)                 }; }
		utils_gpu_available inline degf operator""_deg   (unsigned long long   value) noexcept { return angle::degf{static_cast<float> (value)                 }; }
		utils_gpu_available inline degf operator""_degf  (         long double value) noexcept { return angle::degf{static_cast<float> (value)                 }; }
		utils_gpu_available inline degf operator""_degf  (unsigned long long   value) noexcept { return angle::degf{static_cast<float> (value)                 }; }
		utils_gpu_available inline degd operator""_degd  (         long double value) noexcept { return angle::degd{static_cast<double>(value)                 }; }
		utils_gpu_available inline degd operator""_degd  (unsigned long long   value) noexcept { return angle::degd{static_cast<double>(value)                 }; }
		utils_gpu_available inline radf operator""_rad   (         long double value) noexcept { return angle::radf{static_cast<float> (value)                 }; }
		utils_gpu_available inline radf operator""_rad   (unsigned long long   value) noexcept { return angle::radf{static_cast<float> (value)                 }; }
		utils_gpu_available inline radf operator""_radf  (         long double value) noexcept { return angle::radf{static_cast<float> (value)                 }; }
		utils_gpu_available inline radf operator""_radf  (unsigned long long   value) noexcept { return angle::radf{static_cast<float> (value)                 }; }
		utils_gpu_available inline radd operator""_radd  (         long double value) noexcept { return angle::radd{static_cast<double>(value)                 }; }
		utils_gpu_available inline radd operator""_radd  (unsigned long long   value) noexcept { return angle::radd{static_cast<double>(value)                 }; }
		utils_gpu_available inline radf operator""_radpi (         long double value) noexcept { return angle::radf{static_cast<float> (value) * constants::PIf}; }
		utils_gpu_available inline radf operator""_radpi (unsigned long long   value) noexcept { return angle::radf{static_cast<float> (value) * constants::PIf}; }
		utils_gpu_available inline radf operator""_radfpi(         long double value) noexcept { return angle::radf{static_cast<float> (value) * constants::PIf}; }
		utils_gpu_available inline radf operator""_radfpi(unsigned long long   value) noexcept { return angle::radf{static_cast<float> (value) * constants::PIf}; }
		utils_gpu_available inline radd operator""_raddpi(         long double value) noexcept { return angle::radd{static_cast<double>(value) * constants::PId}; }
		utils_gpu_available inline radd operator""_raddpi(unsigned long long   value) noexcept { return angle::radd{static_cast<double>(value) * constants::PId}; }
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
	utils_gpu_available inline constexpr angle_t clamp(angle_t in, angle_t min_in, angle_t max_in)
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
		template <math::angle::concepts::angle T> utils_gpu_available inline constexpr float                                      sin  (const T& a      ) noexcept { return a.sin(); }
		template <math::angle::concepts::angle T> utils_gpu_available inline constexpr float                                      cos  (const T& a      ) noexcept { return a.cos(); }
		template <math::angle::concepts::angle T> utils_gpu_available inline constexpr float                                      tan  (const T& a      ) noexcept { return a.tan        (    ); }
		template <std ::floating_point         T> utils_gpu_available inline constexpr math::angle::base<T, static_cast<T>(2. * constants::PI)> asin (float n         ) noexcept { return angle::rad<T>::asin (n   ); }
		template <std ::floating_point         T> utils_gpu_available inline constexpr math::angle::base<T, static_cast<T>(2. * constants::PI)> acos (float n         ) noexcept { return angle::rad<T>::acos (n   ); }
		template <std ::floating_point         T> utils_gpu_available inline constexpr math::angle::base<T, static_cast<T>(2. * constants::PI)> atan (float n         ) noexcept { return angle::rad<T>::atan (n   ); }
		template <std ::floating_point         T> utils_gpu_available inline constexpr math::angle::base<T, static_cast<T>(2. * constants::PI)> atan2(float a, float b) noexcept { return angle::rad<T>::atan2(a, b); }
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