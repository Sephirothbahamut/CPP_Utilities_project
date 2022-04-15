#pragma once
#include <ostream>
#include <cmath>
#include <numbers>
#include <algorithm>

#include "../custom_operators.h"
#include "math.h"

#include "angle.h"

#ifdef COUT_CONTAINERS
#include "../cout_utilities.h"
#endif

namespace utils::math
	{
	template <typename T> class vec2;
	}

#ifdef COUT_CONTAINERS
namespace utils::cout
	{
	template <typename T>
	inline std::ostream& operator<<(std::ostream& os, const utils::math::vec2<T>& vec) noexcept
		{
		namespace ccu = cout::support;
		return os << ccu::type << "vec2" << ccu::brace << "(" << ccu::value << vec.x << ccu::separ << ", " << ccu::value << vec.y << ccu::brace << ")";
		}
	}
#endif

namespace utils::math
	{
	template <template<typename> class To, typename T, template<typename> class From, typename F>
	inline static To<T> vec_cast(From<F> from) { return {static_cast<T>(from.x), static_cast<T>(from.y)}; }

	//fast typenames
	using vec2i   = vec2<int>;
	using vec2i8  = vec2<int8_t>;
	using vec2i16 = vec2<int16_t>;
	using vec2i32 = vec2<int32_t>;
	using vec2i64 = vec2<int64_t>;

	using vec2u   = vec2<unsigned>;
	using vec2u8  = vec2<uint8_t>;
	using vec2u16 = vec2<uint16_t>;
	using vec2u32 = vec2<uint32_t>;
	using vec2u64 = vec2<uint64_t>;

	using vec2s = vec2<size_t>;
	//using vec2f = vec2<float>;
	typedef vec2<float> vec2f;
	using vec2d = vec2<double>;

	template <typename T>
	class vec2
		{
		public:
			vec2<T>()                              noexcept = default;
			vec2<T>(T x, T y)                      noexcept : x{x}, y{y} {};
			vec2<T>(angle::deg angle, T magnitude) noexcept : x{angle.cos() * magnitude}, y{angle.sin() * magnitude} {} //TODO test
			vec2<T>(angle::rad angle, T magnitude) noexcept : x{angle.cos() * magnitude}, y{angle.sin() * magnitude} {} //TODO test
			vec2<T>(angle::deg angle)              noexcept : x{angle.cos()            }, y{angle.sin()            } {} //TODO test
			vec2<T>(angle::rad angle)              noexcept : x{angle.cos()            }, y{angle.sin()            } {} //TODO test
			template <typename other_t>
			vec2<T>(vec2<other_t> other)           noexcept : x{static_cast<T>(other.x)}, y{static_cast<T>(other.y)} {} //TODO test


			static vec2<T> rr()    noexcept { return {T{ 1}, T{ 0}}; }
			static vec2<T> ll()    noexcept { return {T{-1}, T{ 0}}; }
			static vec2<T> up()    noexcept { return {T{ 0}, T{-1}}; }
			static vec2<T> dw()    noexcept { return {T{ 0}, T{ 1}}; }
			static vec2<T> right() noexcept { return rr(); }
			static vec2<T> left()  noexcept { return ll(); }
			static vec2<T> down()  noexcept { return dw(); }
			static vec2<T> zero()  noexcept { return {}; }

			T x = 0, y = 0;

			T magnitude2()        const noexcept { return x * x + y * y; }
			T magnitude ()        const noexcept { return std::sqrt(magnitude2()); }
			vec2<T>  normalized() const noexcept { return magnitude() ? *this / magnitude() : *this; }
			vec2<T>& normalize ()       noexcept { return *this = normalized(); }

			// CASTS
			template <template<typename> class To, typename T_t>
			inline To<T_t> vec_cast() { return {static_cast<T_t>(x), static_cast<T_t>(y)}; }

			//float to_angle() const noexcept { return (std::atan2f(x, y) * 180.f / static_cast<float>(std::acos(-1)/*numbers::pi*/)) + 180.f; }
			operator math::angle::deg() const noexcept { return math::angle::trigonometry::atan2(y, x); } //TODO test
			operator math::angle::rad() const noexcept { return math::angle::trigonometry::atan2(y, x); } //TODO test
			operator T         () const noexcept { return magnitude(); }

			template <typename other_t>
			operator vec2<other_t>() const noexcept { return {static_cast<other_t>(x), static_cast<other_t>(y)}; }

			// OPERATORS
			vec2<T>  operator-() const noexcept { return {-x, -y}; }

			// VEC & SCALAR OPERATORS
			vec2<T>  operator++() noexcept { return *this += T(1); }
			vec2<T>  operator--() noexcept { return *this -= T(1); }

			vec2<T>  operator+ (const T n) const noexcept { return {normalized() * (magnitude() + n)}; }
			vec2<T>  operator- (const T n) const noexcept { return {normalized() * (magnitude() - n)}; }
			vec2<T>  operator* (const T n) const noexcept { return {x * n, y * n}; }
			vec2<T>  operator/ (const T n) const noexcept { return {x / n, y / n}; }

			vec2<T>& operator+=(const T n)       noexcept { return *this = *this + n; }
			vec2<T>& operator-=(const T n)       noexcept { return *this = *this - n; }
			vec2<T>& operator*=(const T n)       noexcept { return *this = *this * n; }
			vec2<T>& operator/=(const T n)       noexcept { return *this = *this / n; }
			vec2<T>& operator= (const T n)       noexcept { return normalize() *= n; }

			// VEC & ANGLE OPERATIONS
#ifndef UTILS_NOANGLE
			vec2<T>  operator+ (const utils::math::angle::deg angle) const noexcept { utils::math::angle::rad rngle{angle}; return {x * rngle.cos() - y * rngle.sin(), x * rngle.sin() + y * rngle.cos()}; }
			vec2<T>  operator+ (const utils::math::angle::rad angle) const noexcept { return {x * angle.cos() - y * angle.sin(), x * angle.sin() + y * angle.cos()}; }
			vec2<T>  operator- (const utils::math::angle::deg angle) const noexcept { utils::math::angle::rad rngle{angle}; return {x * rngle.cos() - y * rngle.sin(), x * rngle.sin() + y * rngle.cos()}; }
			vec2<T>  operator- (const utils::math::angle::rad angle) const noexcept { return {x * angle.cos() - y * angle.sin(), x * angle.sin() + y * angle.cos()}; }
			vec2<T>& operator+=(const utils::math::angle::deg angle)       noexcept { return *this = *this + angle; }
			vec2<T>& operator+=(const utils::math::angle::rad angle)       noexcept { return *this = *this + angle; }
			vec2<T>& operator-=(const utils::math::angle::deg angle)       noexcept { return *this = *this - angle; }
			vec2<T>& operator-=(const utils::math::angle::rad angle)       noexcept { return *this = *this - angle; }
			vec2<T>& operator= (const utils::math::angle::deg angle)       noexcept { return *this = angle.to_rad(); }
			vec2<T>& operator= (const utils::math::angle::rad angle)       noexcept { return *this ={angle.cos() * magnitude(), angle.sin() * magnitude()}; }
#endif
			// VEC & VEC OPERATORS
			vec2<T>  operator+ (const vec2<T>& oth) const noexcept { return {x + oth.x, y + oth.y}; }
			vec2<T>  operator- (const vec2<T>& oth) const noexcept { return {x - oth.x, y - oth.y}; }
			vec2<T>  operator* (const vec2<T>& oth) const noexcept { return {x * oth.x, y * oth.y}; }
			vec2<T>  operator/ (const vec2<T>& oth) const noexcept { return {x / oth.x, y / oth.y}; }

			vec2<T>& operator+=(const vec2<T>& oth)       noexcept { return *this = *this + oth; }
			vec2<T>& operator-=(const vec2<T>& oth)       noexcept { return *this = *this - oth; }
			vec2<T>& operator*=(const vec2<T>& oth)       noexcept { return *this = *this * oth; }
			vec2<T>& operator/=(const vec2<T>& oth)       noexcept { return *this = *this / oth; }

			bool     operator==(const vec2<T>& oth) const noexcept { return x == oth.x && y == oth.y; }
			bool     operator!=(const vec2<T>& oth) const noexcept { return !(*this == oth); }

			static T dot(const vec2<T>& a, const vec2<T>& b) noexcept { return a.x * b.x + a.y * b.y; }
			static T distance2(const vec2<T>& a, const vec2<T>& b) noexcept
				{
				T dx = a.x - b.x;
				T dy = a.y - b.y;
				return dx * dx + dy * dy;
				}
			static T distance(const vec2<T>& a, const vec2<T>& b) noexcept { return std::sqrt(distance2(a, b)); }

			// OTHER
			vec2<T> perpendicular_right()            const noexcept { return { y, -x}; }
			vec2<T> perpendicular_left()             const noexcept { return {-y,  x}; }
			vec2<T> perpendicular_clockwise()        const noexcept { return perpendicular_right(); }
			vec2<T> perpendicular_counterclockwise() const noexcept { return perpendicular_left(); }

			static vec2<T> lerp(const vec2<T>& a, const vec2<T>& b, T t) noexcept
				{
				return {utils::lerp(a.x, b.x, t), utils::lerp(a.y, b.y, t)};
				}
			static vec2<T> slerp_fast(const vec2<T>& a, const vec2<T>& b, T t) noexcept
				{
				return vec2{utils::lerp(a.x, b.x, t), utils::lerp(a.y, b.y, t)}.normalize()
					* (utils::lerp(a.magnitude(), b.magnitude(), t));
				}
			static vec2<T> tlerp_fast(const vec2<T>& a, const vec2<T>& b, T t) noexcept
				{
				return vec2{utils::lerp(a.x, b.x, t), utils::lerp(a.y, b.y, t)}.normalize()
					* std::sqrt(utils::lerp(a.magnitude2(), b.magnitude2(), t));
				}
			static vec2<T> slerp(const vec2<T>& a, const vec2<T>& b, T t) noexcept
				{
				T dot = std::clamp(vec2::dot(a, b), -1.0f, 1.0f);
				T theta = std::acos(dot) * t;
				vec2<T> relative_vec = (b - a * dot).normalized();
				return ((a * std::cos(theta)) + (relative_vec * std::sin(theta)));
				}
		};
	
	template <typename T>
	vec2<T> abs(const vec2<T> vec) { return {std::abs(vec.x), std::abs(vec.y)}; }

	namespace operators
		{
		inline constexpr struct _dot
			{
			template <typename T>
			class _inner;

			template <typename T>
			inline friend _inner<T> operator<(vec2<T> lhs, _dot proxy) noexcept { return {lhs}; }

			template <typename T>
			class _inner
				{
				public:
					T operator>(vec2<T> rhs) const noexcept { return vec2<T>::dot(lhs, rhs); }
					_inner(vec2<T> lhs) noexcept : lhs{lhs} {}
				private:
					vec2<T> lhs;
				};

			} dot;

		inline constexpr struct _cross
			{
			template <typename T>
			class _inner;

			template <typename T>
			inline friend _inner<T> operator<(vec2<T> lhs, _cross proxy) noexcept { return {lhs}; }

			template <typename T>
			class _inner
				{
				public:
					vec2<T> operator>(vec2<T> rhs) const noexcept { return lhs * rhs; }
					_inner(vec2<T> lhs) noexcept : lhs{lhs} {}
				private:
					vec2<T> lhs;
				};

			} cross;
		}

	}
