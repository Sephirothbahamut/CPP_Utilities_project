#pragma once
#include <ostream>
#include <stdint.h>
#include <cmath> //trigonometry
#include <algorithm> //clamp
#include <variant> //Angle class
#include <concepts>

#include "../cout_utilities.h"
#include "../math/math.h"
#include "constants.h"


namespace utils::math::angle
	{
	template <std::floating_point T = float, T full_angle_value = 1.f>
	class base_angle
		{
		public:
			using value_type = T;
			inline static constexpr value_type full_angle = full_angle_value;
			inline static constexpr value_type half_angle = full_angle / value_type{2};

			value_type value{ 0.f };

			base_angle() = default;
			base_angle(value_type value) : value{ value } {}
			
			template <value_type other_full_angle>
			base_angle(const base_angle<value_type, other_full_angle>& src) : value{ (src.value / other_full_angle) * full_angle } {}
			template <>
			base_angle<value_type, full_angle_value>(const base_angle<value_type, full_angle_value>& src) : value{src.value} {}

			template <value_type other_full_angle>
			operator base_angle<value_type, other_full_angle>() const noexcept { return { (value / full_angle) * other_full_angle }; }

			base_angle clamp() const noexcept
				{
				if constexpr (full_angle == 1.f) { return {value - std::floor(value)}; }
				else
					{
					value_type new_value{ std::fmod(value, full_angle) };
					if (new_value < 0) { new_value += full_angle; }
					return { new_value };
					}
				}
			base_angle& clamp_self()       noexcept { *this = this->clamped(); return *this; }

			// Shouldn't be needed because...
			//template <value_type other_full_angle> base_angle  operator+ (const base_angle<other_full_angle> oth) const noexcept { return {value + static_cast<base_angle<full_angle>>(oth).value}; }
			//template <value_type other_full_angle> base_angle  operator- (const base_angle<other_full_angle> oth) const noexcept { return {value - static_cast<base_angle<full_angle>>(oth).value}; }
			//template <value_type other_full_angle> base_angle& operator+=(const base_angle<other_full_angle> oth)       noexcept { return *this = *this + oth; }
			//template <value_type other_full_angle> base_angle& operator-=(const base_angle<other_full_angle> oth)       noexcept { return *this = *this - oth; }

			// ...other should automatically cast here
			//base_angle  operator+ (const base_angle oth) const noexcept { return { value + oth.value }; }
			//base_angle  operator- (const base_angle oth) const noexcept { return { value - oth.value }; }
			//base_angle& operator+=(const base_angle oth)       noexcept { return *this = *this + oth; }
			//base_angle& operator-=(const base_angle oth)       noexcept { return *this = *this - oth; }
			// 
			//bool operator==(const base_angle oth) const noexcept { return clamp().value == oth.clamp().value; }
			//bool operator!=(const base_angle oth) const noexcept { return !(*this == oth); }

			// ...except it doesn't automatically cast, so here we go...
			template <value_type other_full_angle> base_angle  operator+ (const base_angle<value_type, other_full_angle> oth) const noexcept { return {value + static_cast<base_angle<value_type, full_angle>>(oth).value}; }
			template <value_type other_full_angle> base_angle  operator- (const base_angle<value_type, other_full_angle> oth) const noexcept { return {value - static_cast<base_angle<value_type, full_angle>>(oth).value}; }
			template <value_type other_full_angle> base_angle& operator+=(const base_angle<value_type, other_full_angle> oth)       noexcept { return *this = *this + oth; }
			template <value_type other_full_angle> base_angle& operator-=(const base_angle<value_type, other_full_angle> oth)       noexcept { return *this = *this - oth; }

			template <value_type other_full_angle> bool operator==(const base_angle<value_type, other_full_angle> oth) const noexcept { return clamp().value == static_cast<base_angle<value_type, full_angle>>(oth).clamp().value; }
			template <value_type other_full_angle> bool operator!=(const base_angle<value_type, other_full_angle> oth) const noexcept { return !(*this == oth); }

			base_angle  operator* (value_type oth) const noexcept { return { value * oth }; }
			base_angle  operator/ (value_type oth) const noexcept { return { value / oth }; }
			base_angle& operator*=(value_type oth)       noexcept { return *this = *this * oth; }
			base_angle& operator/=(value_type oth)       noexcept { return *this = *this / oth; }

			base_angle  operator-()const noexcept { return base_angle{ value + half_angle }.clamp(); }

			value_type normalize_in_range(base_angle min, base_angle max) const noexcept
				{
				min.clamp();
				max.clamp();
				value_type next_max{ min.value <= max.value ? max.value : max.value + full_angle };
				value_type min_to_this{ (min.value <= value ? value : value + full_angle) - min.value };
				value_type min_to_max{ next_max - min.value };
				if (min_to_this < min_to_max) { return min_to_this / min_to_max; }

				base_angle halfway{ (next_max + min.value) * .5f };
				base_angle opposite{ -halfway };
				value_type next_opposite{ next_max < opposite.value ? opposite.value : opposite.value + full_angle };
				if (next_opposite < next_max) { next_opposite += full_angle; }

				value_type min_to_opposite{ next_opposite - min.value };
				return min_to_this < min_to_opposite ? min_to_this / min_to_max : (min_to_this - full_angle) / min_to_max;
				}

	#pragma region Trigonometry
			constexpr value_type sin() const noexcept { return std::sin(rad_value()); }
			constexpr value_type cos() const noexcept { return std::cos(rad_value()); }
			constexpr value_type tan() const noexcept { return std::tan(rad_value()); }
			inline static constexpr base_angle asin (value_type n)               noexcept { return { base_angle<value_type, value_type{2} * constants::PIf>{std::asin(n)} }; }
			inline static constexpr base_angle acos (value_type n)               noexcept { return { base_angle<value_type, value_type{2} * constants::PIf>{std::acos(n)} }; }
			inline static constexpr base_angle atan (value_type n)               noexcept { return { base_angle<value_type, value_type{2} * constants::PIf>{std::atan(n)} }; }
			inline static constexpr base_angle atan2(value_type a, value_type b) noexcept { return { base_angle<value_type, value_type{2} * constants::PIf>{std::atan2(a, b)} }; }
	#pragma endregion Trigonometry

		private:
			constexpr value_type rad_value() const noexcept { return static_cast<base_angle<value_type, 2.f * constants::PIf>>(*this).value; }
		};

	namespace concepts
		{
		template <typename T>
		concept is_angle = std::same_as<T, base_angle<typename T::value_type, T::full_angle>>;
		}

	using degf = base_angle<float , 360.f>;
	using radf = base_angle<float , 2.f * constants::PIf>;
	using degd = base_angle<double, 360. >;
	using radd = base_angle<double, 2.  * constants::PId>;
	using deg = degf;
	using rad = radf;

	namespace trigonometry
		{
		template <concepts::is_angle       T> inline float  sin  (const T& a      ) noexcept { return a.sin        (    ); }
		template <concepts::is_angle       T> inline float  cos  (const T& a      ) noexcept { return a.cos        (    ); }
		template <concepts::is_angle       T> inline float  tan  (const T& a      ) noexcept { return a.tan        (    ); }
		template <std     ::floating_point T> inline base_angle<T, 2.f * constants::PIf> asin (float n         ) noexcept { return rad<T>::asin (n   ); }
		template <std     ::floating_point T> inline base_angle<T, 2.f * constants::PIf> acos (float n         ) noexcept { return rad<T>::acos (n   ); }
		template <std     ::floating_point T> inline base_angle<T, 2.f * constants::PIf> atan (float n         ) noexcept { return rad<T>::atan (n   ); }
		template <std     ::floating_point T> inline base_angle<T, 2.f * constants::PIf> atan2(float a, float b) noexcept { return rad<T>::atan2(a, b); }
		}

	namespace literals
		{
		inline deg operator""_deg  (         long double value) noexcept { return angle::deg{static_cast<float>(value)}; }
		inline deg operator""_deg  (unsigned long long   value) noexcept { return angle::deg{static_cast<float>(value)}; }
		inline rad operator""_rad  (         long double value) noexcept { return angle::rad{static_cast<float>(value)}; }
		inline rad operator""_rad  (unsigned long long   value) noexcept { return angle::rad{static_cast<float>(value)}; }
		inline rad operator""_radpi(         long double value) noexcept { return angle::rad{static_cast<float>(value) * constants::PIf}; }
		inline rad operator""_radpi(unsigned long long   value) noexcept { return angle::rad{static_cast<float>(value) * constants::PIf}; }
		}
	}

namespace utils
	{
	//template <float full_angle_value = 1.f>
	//inline math::angle::base_angle<full_angle_value> lerp(math::angle::base_angle<full_angle_value> a, math::angle::base_angle<full_angle_value> b, float t)
	//	{
	//	return { utils::lerp(a.value, b.value, t) };
	//	}
	template <utils::math::angle::concepts::is_angle angle_t, utils::math::angle::concepts::is_angle min_t, utils::math::angle::concepts::is_angle max_t>
	inline angle_t clamp(angle_t in, min_t min_in, max_t max_in)
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
	}

namespace utils::cout
	{
	template <utils::math::angle::concepts::is_angle angle_t>
	inline std::ostream& operator<<(std::ostream& os, const angle_t& angle)   noexcept { namespace ccu = utils::cout::support; return os << ccu::value << static_cast<math::angle::deg>(angle).value << ccu::type << "°"; }
	}

// Older
/*
namespace utils::math::angle
	{
	using constants::PIf;
	using constants::PId;
	using constants::PI;

	inline float deg_to_rad(float d) noexcept { return d * (PIf / 180.f); }
	inline float rad_to_deg(float r) noexcept { return r * (180.f / PIf); }

	class deg;
	class rad;

	namespace literals//@section MISSING DEFINITIONS
		{
		inline deg operator""_deg(long double value)        noexcept;
		inline deg operator""_deg(unsigned long long value) noexcept;
		inline rad operator""_rad(long double value)        noexcept;
		inline rad operator""_rad(unsigned long long value) noexcept;
		inline rad operator""_radpi(long double value)        noexcept;
		inline rad operator""_radpi(unsigned long long value) noexcept;
		}
	
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== =====                 DEG                 ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	class deg
		{
		friend class rad;
		friend deg literals::operator""_deg(long double value) noexcept;
		friend deg literals::operator""_deg(unsigned long long value) noexcept;
		private:

		public:
			deg() noexcept = default;
			deg(float value) noexcept : value(value) {}
			float value{0.f};

			deg(const rad r) noexcept;//@section MISSING DEFINITIONS : value(rad_to_deg(r.value)) {}
			deg& operator=(const rad r) noexcept;//@section MISSING DEFINITIONS { value = rad_to_deg(r.value); return *this; };
			rad to_rad() const noexcept;//@section MISSING DEFINITIONS { return rad{*this}; }

			deg clamped() const noexcept 
				{
				float v = value;
				while (v < 0) { v += 360; }
				while (v >= 360) { v -= 360; }
				return deg{v};
				}
			void clamp() noexcept { *this = this->clamped(); }

			deg  operator+ (const deg oth) const noexcept { return deg{value + oth.value}; }
			deg  operator- (const deg oth) const noexcept { return deg{value - oth.value}; }
			deg& operator+=(const deg oth)       noexcept { return *this = *this + oth; }
			deg& operator-=(const deg oth)       noexcept { return *this = *this - oth; }
			deg  operator+ (const rad oth) const noexcept;//@section MISSING DEFINITIONS
			deg  operator- (const rad oth) const noexcept;//@section MISSING DEFINITIONS
			deg& operator+=(const rad oth)       noexcept;//@section MISSING DEFINITIONS
			deg& operator-=(const rad oth)       noexcept;//@section MISSING DEFINITIONS

			deg  operator* (float oth) const noexcept { return deg{value * oth}; }
			deg  operator/ (float oth) const noexcept { return deg{value / oth}; }
			deg& operator*=(float oth)       noexcept { return *this = *this * oth; }
			deg& operator/=(float oth)       noexcept { return *this = *this / oth; }

			deg  operator- () const noexcept { return {-value}; };

			bool operator==(const deg oth) const noexcept { return clamped().value == oth.clamped().value; }
			bool operator!=(const deg oth) const noexcept { return !(*this == oth); }
			bool operator==(const rad oth) const noexcept;//@section MISSING DEFINITIONS
			bool operator!=(const rad oth) const noexcept;//@section MISSING DEFINITIONS

				   float sin()      const noexcept { return std::sin(deg_to_rad(value)); }
				   float cos()      const noexcept { return std::cos(deg_to_rad(value)); }
				   float tan()      const noexcept { return std::tan(deg_to_rad(value)); }
			static float sin(const deg a) noexcept { return a.sin(); }
			static float cos(const deg a) noexcept { return a.cos(); }
			static float tan(const deg a) noexcept { return a.tan(); }

			static deg lerp(deg a, deg b, float t)
				{
				if (a.value <= b.value) { return std::lerp(a.value, b.value, t); }
				//TODO
				}
		};


	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== =====                 RAD                 ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	class rad
		{
		friend class deg;
		friend rad literals::operator""_rad(long double value) noexcept;
		friend rad literals::operator""_rad(unsigned long long value) noexcept;
		private:

		public:
			rad() noexcept = default;
			rad(float value) noexcept : value(value) {}
			float value{0.f};

			rad(const deg d) noexcept : value(deg_to_rad(d.value)) {}
			rad& operator=(const deg& d) noexcept { value = deg_to_rad(d.value); return *this; };
			deg to_deg() const noexcept { return deg{*this}; }

			deg clamped() const noexcept
				{
				float v = value;
				while (v < 0) { v += 2 * PIf; }
				while (v >= 2*PIf) { v -= 2 * PIf; }
				return deg{v};
				}
			void clamp() noexcept { *this = this->clamped(); }

			rad  operator+ (const rad oth) const noexcept { return {value + oth.value}; }
			rad  operator- (const rad oth) const noexcept { return {value - oth.value}; }
			rad& operator+=(const rad oth)       noexcept { return *this = *this + oth; }
			rad& operator-=(const rad oth)       noexcept { return *this = *this - oth; }
			rad  operator+ (const deg oth) const noexcept { return {value + deg_to_rad(oth.value)}; }
			rad  operator- (const deg oth) const noexcept { return {value - deg_to_rad(oth.value)}; }
			rad& operator+=(const deg oth)       noexcept { return *this = *this + oth; }
			rad& operator-=(const deg oth)       noexcept { return *this = *this - oth; }

			rad  operator* (float oth) const noexcept { return rad{value * oth}; }
			rad  operator/ (float oth) const noexcept { return rad{value / oth}; }
			rad& operator*=(float oth)       noexcept { return *this = *this * oth; }
			rad& operator/=(float oth)       noexcept { return *this = *this / oth; }

			rad  operator- ()          const noexcept { return {value - PI}; };

			bool operator==(const rad oth) const noexcept { return clamped().value == oth.clamped().value; }
			bool operator!=(const rad oth) const noexcept { return !(*this == oth); } 
			bool operator==(const deg oth) const noexcept { return *this == oth.to_rad(); }
			bool operator!=(const deg oth) const noexcept { return !(*this == oth); }

				   float sin()      const noexcept { return std::sin(value); }
				   float cos()      const noexcept { return std::cos(value); }
				   float tan()      const noexcept { return std::tan(value); }
			static float sin(const rad a) noexcept { return a.sin(); }
			static float cos(const rad a) noexcept { return a.cos(); }
			static float tan(const rad a) noexcept { return a.tan(); }
			static rad asin(float n)      noexcept { return {std::asin(n)}; }
			static rad acos(float n)      noexcept { return {std::acos(n)}; }
			static rad atan(float n)      noexcept { return {std::atan(n)}; }
			static rad atan2(float a, float b) noexcept { return{std::atan2(a, b)}; }
		};



	namespace trigonometry
		{
		inline float sin(const rad a) noexcept { return a.sin(); }
		inline float cos(const rad a) noexcept { return a.cos(); }
		inline float tan(const rad a) noexcept { return a.tan(); }
		inline float sin(const deg a) noexcept { return rad{a}.sin(); }
		inline float cos(const deg a) noexcept { return rad{a}.cos(); }
		inline float tan(const deg a) noexcept { return rad{a}.tan(); }
		inline rad asin(float n) noexcept { return rad::asin(n); }
		inline rad acos(float n) noexcept { return rad::acos(n); }
		inline rad atan(float n) noexcept { return rad::atan(n); }
		inline rad atan2(float a, float b) noexcept { return rad::atan2(a, b); }
		}

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== =====         MISSING DEFINITIONS         ===== ===== ===== ===== ===== ===== =====
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	inline deg::deg(const rad r) noexcept : value(rad_to_deg(r.value)) {}
	inline deg& deg::operator=(const rad r) noexcept { value = rad_to_deg(r.value); return *this; };
	inline rad deg::to_rad() const noexcept { return rad{*this}; }

	inline deg  deg::operator+ (const rad oth) const noexcept { return deg{value + rad_to_deg(oth.value)}; }
	inline deg  deg::operator- (const rad oth) const noexcept { return deg{value - rad_to_deg(oth.value)}; }
	inline deg& deg::operator+=(const rad oth)       noexcept { return *this = *this + oth; }
	inline deg& deg::operator-=(const rad oth)       noexcept { return *this = *this - oth; }
	inline bool deg::operator==(const rad oth) const noexcept { return *this == oth.to_deg(); }
	inline bool deg::operator!=(const rad oth) const noexcept { return !(*this == oth); }
	
	namespace literals
		{
		inline deg operator""_deg(long double        value) noexcept { return deg{float(value)}; }
		inline deg operator""_deg(unsigned long long value) noexcept { return deg{float(value)}; }
		inline rad operator""_rad(long double        value) noexcept { return rad{float(value)}; }
		inline rad operator""_rad(unsigned long long value) noexcept { return rad{float(value)}; }
		inline rad operator""_radpi(long double        value) noexcept { return rad{float(value) * PIf}; }
		inline rad operator""_radpi(unsigned long long value) noexcept { return rad{float(value) * PIf}; }
		}

	template <typename T>
	concept angle_type = std::is_same_v<T, deg> || std::is_same_v<T, rad>;
	}

namespace utils::cout
	{
	inline std::ostream& operator<<(std::ostream& os, const math::angle::deg& d)   noexcept { namespace ccu = utils::cout::support; return os << ccu::value << d.value << ccu::type << "d"; }
	inline std::ostream& operator<<(std::ostream& os, const math::angle::rad& r)   noexcept { namespace ccu = utils::cout::support; return os << ccu::value << r.value << ccu::type << "r"; }
	}
*/


