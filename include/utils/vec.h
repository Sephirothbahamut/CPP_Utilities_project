#pragma once
#include <cmath>
#include <array>
#include <numbers>
#include <ostream>
#include <algorithm>

#include "custom_operators.h"
#include "containers/array.h"

#ifdef COUT_CONTAINERS
#include "../cout_utilities.h"
#endif

namespace utils
	{
	template <typename T, size_t size> class vec;
	}

#ifdef COUT_CONTAINERS
namespace utils::cout
	{
	template <typename T, size_t size>
	inline std::ostream& operator<<(std::ostream& os, const utils::math::vec<T, size>& vec) noexcept
		{
		namespace ccu = cout::support;
		return os << ccu::type << "vec" << utils::math::vec<T, size>::size << ccu::brace << "(" << ccu::value << vec.x << ccu::separ << ", " << ccu::value << vec.y << ccu::brace << ")";
		}
	}
#endif

namespace utils
	{
	template <template<typename> class To, typename T, class From>
	inline static To<T> vec_cast(From from) { return {static_cast<T>(from.x), static_cast<T>(from.y)}; }

	template <typename T, size_t size>
	class vec
		{
		protected:
			std::array<T, size> _data;

		public:
			//TODO add AT functions and call AT in [] in debug mode
				  T& operator[](size_t index)       noexcept { return _data[index]; }
			const T& operator[](size_t index) const noexcept { return _data[index]; }

		public:
			inline static constexpr size_t size = size;

			vec() = default;

			template <typename other_t, size_t other_size>
			vec(vec<other_t, other_size> other)           noexcept { containers::array::copy(_data, other._data); } //TODO test

			/*vec<T, size> abs()                   const noexcept { vec<T, size> ret; for (size_t i{0}; i < size; i++) { ret[i] = std::abs(_data[i]); } return ret; }
			vec<T, size> min(vec<T, size> min)                   const noexcept { vec<T, size> ret; for (size_t i{0}; i < size; i++) { ret[i] = std::min(_data[i], min._data[i]); } return ret; }
			vec<T, size> max(vec<T, size> max)                   const noexcept { vec<T, size> ret; for (size_t i{0}; i < size; i++) { ret[i] = std::max(_data[i], max._data[i]); } return ret; }
			vec<T, size> clamp(vec<T, size> min, vec<T, size> max) const noexcept { vec<T, size> ret; for (size_t i{0}; i < size; i++) { ret[i] = std::clamp(_data[i], min._data[i], max._data[i]); } return ret; }

			vec<T, size>& abs_self() noexcept { return *this = abs(); }
			vec<T, size>& min_self(vec<T, size> min) noexcept { return *this = this->min(min); }
			vec<T, size>& max_self(vec<T, size> max) noexcept { return *this = this->max(max); }
			vec<T, size>& clamp_self(vec<T, size> min, vec<T, size> max) noexcept { return *this = clamp(min, max); }

			static T dot(const vec<T, size>& a, const vec<T, size>& b) noexcept { T ret{0}; for (size_t i{0}; i < size; i++) { ret += a[i] * b[i]; } return ret; }
			
			static vec<T, size> lerp(const vec<T, size>& a, const vec<T, size>& b, T t) noexcept
				{
				vec<T, size> ret;
				for (size_t i{0}; i < size; i++) { ret[i] = utils::lerp(a[i], b[i], t); }
				return ret;
				}
			static vec<T, size> slerp_fast(const vec<T, size>& a, const vec<T, size>& b, T t) noexcept
				{
				return lerp(a, b).normalize() * (utils::lerp(a.magnitude(), b.magnitude(), t));
				}
			static vec<T, size> tlerp_fast(const vec<T, size>& a, const vec<T, size>& b, T t) noexcept
				{
				return lerp(a, b).normalize() * std::sqrt(utils::lerp(a.magnitude2(), b.magnitude2(), t));
				}
			static vec<T, size> slerp(const vec<T, size>& a, const vec<T, size>& b, T t) noexcept //TODO test
				{
				T dot = std::clamp(vec::dot(a, b), -1.0f, 1.0f);
				T theta = std::acos(dot) * t;
				vec<T, size> relative_vec = (b - a * dot).normalized();
				return ((a * std::cos(theta)) + (relative_vec * std::sin(theta)));
				}*/
		};

	// OPERATORS
	template <typename T, size_t size, std::derived_from<vec<T, size>> vec_t> vec_t  operator-(const vec_t& v) noexcept { vec_t ret; for (size_t i{0}; i < size; i++) { ret[i] = -v[i]; }; return ret; }

	// VEC & SCALAR OPERATORS
	template <typename T, size_t size, std::derived_from<vec<T, size>> vec_t> vec_t  operator++(vec_t& v) noexcept { return *this += T(1); }
	template <typename T, size_t size, std::derived_from<vec<T, size>> vec_t> vec_t  operator--(vec_t& v) noexcept { return *this -= T(1); }

	template <typename T, size_t size, std::derived_from<vec<T, size>> vec_t> vec_t  operator+ (const vec_t& v, const T n) noexcept { vec_t ret; for (size_t i{0}; i < size; i++) { ret[i] = v[i] + n; }; return ret; }
	template <typename T, size_t size, std::derived_from<vec<T, size>> vec_t> vec_t  operator- (const vec_t& v, const T n) noexcept { vec_t ret; for (size_t i{0}; i < size; i++) { ret[i] = v[i] - n; }; return ret; }
	template <typename T, size_t size, std::derived_from<vec<T, size>> vec_t> vec_t  operator* (const vec_t& v, const T n) noexcept { vec_t ret; for (size_t i{0}; i < size; i++) { ret[i] = v[i] * n; }; return ret; }
	template <typename T, size_t size, std::derived_from<vec<T, size>> vec_t> vec_t  operator/ (const vec_t& v, const T n) noexcept { vec_t ret; for (size_t i{0}; i < size; i++) { ret[i] = v[i] / n; }; return ret; }

	template <typename T, size_t size, std::derived_from<vec<T, size>> vec_t> vec_t& operator+=(vec_t& v, const T n) noexcept { return v = v + n; }
	template <typename T, size_t size, std::derived_from<vec<T, size>> vec_t> vec_t& operator-=(vec_t& v, const T n) noexcept { return v = v - n; }
	template <typename T, size_t size, std::derived_from<vec<T, size>> vec_t> vec_t& operator*=(vec_t& v, const T n) noexcept { return v = v * n; }
	template <typename T, size_t size, std::derived_from<vec<T, size>> vec_t> vec_t& operator/=(vec_t& v, const T n) noexcept { return v = v / n; }

	// VEC & VEC OPERATORS
	//TODO vec&vec operators when other has a different size
	template <typename T, size_t size, std::derived_from<vec<T, size>> vec_t> vec_t  operator+ (const vec_t& v, const vec_t& oth) noexcept { auto ret{v}; for (size_t i{0}; i < size; i++) { ret[i] = v[i] + oth[i]; }; return ret; }
	template <typename T, size_t size, std::derived_from<vec<T, size>> vec_t> vec_t  operator- (const vec_t& v, const vec_t& oth) noexcept { auto ret{v}; for (size_t i{0}; i < size; i++) { ret[i] = v[i] - oth[i]; }; return ret; }
	template <typename T, size_t size, std::derived_from<vec<T, size>> vec_t> vec_t  operator* (const vec_t& v, const vec_t& oth) noexcept { auto ret{v}; for (size_t i{0}; i < size; i++) { ret[i] = v[i] * oth[i]; }; return ret; }
	template <typename T, size_t size, std::derived_from<vec<T, size>> vec_t> vec_t  operator/ (const vec_t& v, const vec_t& oth) noexcept { auto ret{v}; for (size_t i{0}; i < size; i++) { ret[i] = v[i] / oth[i]; }; return ret; }

	template <typename T, size_t size, std::derived_from<vec<T, size>> vec_t> vec_t& operator+=(vec_t& v, const vec_t& oth) noexcept { return v = v + oth; }
	template <typename T, size_t size, std::derived_from<vec<T, size>> vec_t> vec_t& operator-=(vec_t& v, const vec_t& oth) noexcept { return v = v - oth; }
	template <typename T, size_t size, std::derived_from<vec<T, size>> vec_t> vec_t& operator*=(vec_t& v, const vec_t& oth) noexcept { return v = v * oth; }
	template <typename T, size_t size, std::derived_from<vec<T, size>> vec_t> vec_t& operator/=(vec_t& v, const vec_t& oth) noexcept { return v = v / oth; }

	template <typename T, size_t size, std::derived_from<vec<T, size>> vec_t> bool          operator==(const vec_t& v, const vec_t& oth) noexcept { for (size_t i{0}; i < size; i++) { if (v[i] != oth[i]) { return false; } }; return true; }
	template <typename T, size_t size, std::derived_from<vec<T, size>> vec_t> bool          operator!=(const vec_t& v, const vec_t& oth) noexcept { return !(*this == oth); }

	template <typename T, size_t size>
	vec<T, size> abs(const vec<T, size> vec) noexcept { return vec.abs(); }

	namespace operators
		{
		inline constexpr struct _dot
			{
			template <typename T, size_t size>
			class _inner;

			template <typename T, size_t size>
			inline friend _inner<T, size> operator<(vec<T, size> lhs, _dot proxy) noexcept { return {lhs}; }

			template <typename T, size_t size>
			class _inner
				{
				public:
					T operator>(vec<T, size> rhs) const noexcept { return vec<T, size>::dot(lhs, rhs); }
					_inner(vec<T, size> lhs) noexcept : lhs{lhs} {}
				private:
					vec<T, size> lhs;
				};

			} dot;

		inline constexpr struct _cross
			{
			template <typename T, size_t size>
			class _inner;

			template <typename T, size_t size>
			inline friend _inner<T, size> operator<(vec<T, size> lhs, _cross proxy) noexcept { return {lhs}; }

			template <typename T, size_t size>
			class _inner
				{
				public:
					vec<T, size> operator>(vec<T, size> rhs) const noexcept { return lhs * rhs; }
					_inner(vec<T, size> lhs) noexcept : lhs{lhs} {}
				private:
					vec<T, size> lhs;
				};

			} cross;
		}
	}
namespace utils
	{
	template <typename T, size_t size, std::derived_from<vec<T, size>> vec_t>
	inline vec_t lerp(vec_t a, vec_t b, float t) { return vec_t::lerp(a, b, t); }
	template <typename T, size_t size, std::derived_from<vec<T, size>> vec_t>
	inline vec_t clamp(vec_t in, vec_t min, vec_t max) { return in.clamp(min, max); }
	template <typename T, size_t size, std::derived_from<vec<T, size>> vec_t>
	inline vec_t min(vec_t in, vec_t min) { return in.min(min); }
	template <typename T, size_t size, std::derived_from<vec<T, size>> vec_t>
	inline vec_t max(vec_t in, vec_t max) { return in.max(max); }
	}