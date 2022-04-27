#pragma once
#include <ostream>
#include <stdint.h>
#include <cmath> //trigonometry
#include <algorithm> //clamp
#include <variant> //Angle class
#include <concepts>

#include "../cout_utilities.h"
#include "constants.h"


namespace utils::beta::math::angle
	{
	// TODO complete & test
	template <float full_angle_value = 1.f>
	class base_angle
		{
		public:
			inline static constexpr float full_angle = full_angle_value;
			inline static constexpr float half_angle = full_angle / 2.f;

			float value{ 0.f };

			base_angle() = default;
			base_angle(float value) : value{ value } {}

			template <float other_full_angle>
			base_angle(const base_angle<other_full_angle>& src) : value{ (src.value / other_full_angle) * full_angle } {}

			template <float other_full_angle>
			operator base_angle<other_full_angle>() const noexcept { return { (value / full_angle) * other_full_angle }; }


			base_angle  clamped() const noexcept
				{
				if (false) {}//constepxr (full_angle == 1.f) { return {value - std::floor(value)}; }
				else
					{
					float new_value{ std::fmod(value, full_angle) };
					if (new_value < 0) { new_value += full_angle; }
					return { new_value };
					}
				}
			base_angle& clamp()       noexcept { *this = this->clamped(); return *this; }

			// Shouldn't be needed because...
			//template <float other_full_angle> base_angle  operator+ (const base_angle<other_full_angle> oth) const noexcept { return {value + static_cast<base_angle<full_angle>>(oth).value}; }
			//template <float other_full_angle> base_angle  operator- (const base_angle<other_full_angle> oth) const noexcept { return {value - static_cast<base_angle<full_angle>>(oth).value}; }
			//template <float other_full_angle> base_angle& operator+=(const base_angle<other_full_angle> oth)       noexcept { return *this = *this + oth; }
			//template <float other_full_angle> base_angle& operator-=(const base_angle<other_full_angle> oth)       noexcept { return *this = *this - oth; }

			// ...other should automatically cast here
			base_angle  operator+ (const base_angle oth) const noexcept { return { value + oth.value }; }
			base_angle  operator- (const base_angle oth) const noexcept { return { value - oth.value }; }
			base_angle& operator+=(const base_angle oth)       noexcept { return *this = *this + oth; }
			base_angle& operator-=(const base_angle oth)       noexcept { return *this = *this - oth; }

			base_angle  operator* (float oth) const noexcept { return { value * oth }; }
			base_angle  operator/ (float oth) const noexcept { return { value / oth }; }
			base_angle& operator*=(float oth)       noexcept { return *this = *this * oth; }
			base_angle& operator/=(float oth)       noexcept { return *this = *this / oth; }

			base_angle  operator-()const noexcept { return base_angle{ value + half_angle }.clamped(); }

			float normalize_in_range(base_angle min, base_angle max) const noexcept
				{
				min.clamp();
				max.clamp();
				float next_max{ min.value <= max.value ? max.value : max.value + full_angle };
				float min_to_this{ (min.value <= value ? value : value + full_angle) - min.value };
				float min_to_max{ next_max - min.value };
				if (min_to_this < min_to_max) { return min_to_this / min_to_max; }

				base_angle halfway{ (next_max + min.value) * .5f };
				base_angle opposite{ -halfway };
				float next_opposite{ next_max < opposite.value ? opposite.value : opposite.value + full_angle };
				if (next_opposite < next_max) { next_opposite += full_angle; }

				float min_to_opposite{ next_opposite - min.value };
				return min_to_this < min_to_opposite ? min_to_this / min_to_max : (min_to_this - full_angle) / min_to_max;
				}

			base_angle clamped(base_angle min, base_angle max) const noexcept
				{
				min.clamp();
				max.clamp();
				float next_max{ min.value <= max.value ? max.value : max.value + full_angle };
				float min_to_this{ (min.value <= value ? value : value + full_angle) - min.value };
				float min_to_max{ next_max - min.value };
				if (min_to_this < min_to_max) { return *this; }

				base_angle halfway{ (next_max + min.value) * .5f };
				base_angle opposite{ -halfway };
				float next_opposite{ next_max < opposite.value ? opposite.value : opposite.value + full_angle };
				if (next_opposite < next_max) { next_opposite += full_angle; }

				float min_to_opposite{ next_opposite - min.value };
				return min_to_this < min_to_opposite ? max : min;
				}

			base_angle& clamp(base_angle min, base_angle max)       noexcept { *this = this->clamped(min, max); return *this; }

			inline static base_angle lerp(base_angle a, base_angle b) noexcept
				{
				return {std::lerp(a.value, b.value)};
				}

			bool operator==(const base_angle oth) const noexcept { return clamped().value == oth.clamped().value; }
			bool operator!=(const base_angle oth) const noexcept { return !(*this == oth); }

	#pragma region Trigonometry
			constexpr float sin() const noexcept { return std::sin(rad_value()); }
			constexpr float cos() const noexcept { return std::cos(rad_value()); }
			constexpr float tan() const noexcept { return std::tan(rad_value()); }
			inline static constexpr base_angle asin(float n)          noexcept { return { base_angle<2.f * constants::PIf>{std::asin(n)} }; }
			inline static constexpr base_angle acos(float n)          noexcept { return { base_angle<2.f * constants::PIf>{std::acos(n)} }; }
			inline static constexpr base_angle atan(float n)          noexcept { return { base_angle<2.f * constants::PIf>{std::atan(n)} }; }
			inline static constexpr base_angle atan2(float a, float b) noexcept { return { base_angle<2.f * constants::PIf>{std::atan2(a, b)} }; }
	#pragma endregion Trigonometry

		private:
			constexpr float rad_value() const noexcept { return static_cast<base_angle<2.f * constants::PIf>>(*this).value; }
		};

	using deg = base_angle<360.f>;
	using rad = base_angle<2.f * constants::PIf>;
	

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

	namespace literals
		{
		inline deg operator""_deg  (         long double value) noexcept { return angle::deg{value}; }
		inline deg operator""_deg  (unsigned long long   value) noexcept { return angle::deg{value}; }
		inline rad operator""_rad  (         long double value) noexcept { return angle::rad{value}; }
		inline rad operator""_rad  (unsigned long long   value) noexcept { return angle::rad{value}; }
		inline rad operator""_radpi(         long double value) noexcept { return angle::rad{value * constants::PIf}; }
		inline rad operator""_radpi(unsigned long long   value) noexcept { return angle::rad{value * constants::PIf}; }
		}
	}

namespace utils::cout
	{
	template <float full_angle>
	inline std::ostream& operator<<(std::ostream& os, const beta::math::angle::base_angle<full_angle>& angle)   noexcept { namespace ccu = utils::cout::support; return os << ccu::value << static_cast<deg>(angle).value << ccu::type << "°"; }
	}

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



