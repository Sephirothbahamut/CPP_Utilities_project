#pragma once

#include <numeric>
#include <algorithm>
#include <array>

#include "math.h"
#include "angle.h"
#include "../cout_utilities.h"

namespace engine::core { template <typename T, size_t S> class Vec; }

namespace utils::cout
	{
	template <typename T, size_t S>
	inline std::ostream& operator<<(std::ostream& os, const engine::core::Vec<T, S>& vec) noexcept { return os << vec.arr; }
	}

namespace engine::core
	{
	template <typename T, size_t S>
	class Vec
		{
		protected:
			
			std::array<T, S> arr;

		public:
			Vec<T, S>() noexcept = default;
			Vec<T, S>(T val) noexcept { std::transform(std::begin(arr), std::end(arr), std::begin(arr), [val]() { return val; }); }

			template <typename ... Ts>
			Vec<T, S>(Ts ... args) : arr{args...} {};

			// ACCESS
			const T& operator[](size_t i) const noexcept { return arr[i]; }
			T& operator[](size_t i) noexcept { return arr[i]; }

			T magnitude2() const noexcept { return std::accumulate(arr.begin(), arr.end(), 0, [](T a, T b) { return a + b * b; }); }
			T magnitude() const noexcept { return std::sqrt(magnitude2()); }
			Vec<T, S>  normal() const noexcept { return magnitude() ? *this / magnitude() : *this; }
			Vec<T, S>& normalize() noexcept { return *this = normal(); }

			// OPERATORS
			Vec<T, S>  operator-() const noexcept { Vec<T, S> ret; std::transform(std::begin(arr), std::end(arr), std::begin(ret.arr), [](T value) { return -value; }); return ret; }

			// VEC & SCALAR OPERATORS
			Vec<T, S>  operator++() noexcept { return *this += T(1); }
			Vec<T, S>  operator--() noexcept { return *this -= T(1); }

			Vec<T, S>  operator+ (const T n) const noexcept { return {normal() * (magnitude() + n)}; }
			Vec<T, S>  operator- (const T n) const noexcept { return {normal() * (magnitude() - n)}; }
			Vec<T, S>  operator* (const T n) const noexcept { Vec<T, S> ret; std::transform(std::begin(arr), std::end(arr), std::begin(ret.arr), [n](T value) { return value * n; }); return ret; }
			Vec<T, S>  operator/ (const T n) const noexcept { Vec<T, S> ret; std::transform(std::begin(arr), std::end(arr), std::begin(ret.arr), [n](T value) { return value / n; }); return ret; }

			Vec<T, S>& operator+=(const T n) noexcept { return *this = *this + n; }
			Vec<T, S>& operator-=(const T n) noexcept { return *this = *this - n; }
			Vec<T, S>& operator*=(const T n) noexcept { return *this = *this * n; }
			Vec<T, S>& operator/=(const T n) noexcept { return *this = *this / n; }
			Vec<T, S>& operator= (const T n) noexcept { return normalize() *= n; }

			// VEC & VEC OPERATORS
			Vec<T, S>  operator+(const Vec<T, S>& oth) const noexcept { Vec<T, S> ret; std::transform(std::begin(arr), std::end(arr), std::begin(oth.arr), std::begin(ret.arr), [](const T& a, const T& b) { return a + b; }); return ret; }
			Vec<T, S>  operator-(const Vec<T, S>& oth) const noexcept { Vec<T, S> ret; std::transform(std::begin(arr), std::end(arr), std::begin(oth.arr), std::begin(ret.arr), [](const T& a, const T& b) { return a - b; }); return ret; }
			Vec<T, S>  operator*(const Vec<T, S>& oth) const noexcept { Vec<T, S> ret; std::transform(std::begin(arr), std::end(arr), std::begin(oth.arr), std::begin(ret.arr), [](const T& a, const T& b) { return a * b; }); return ret; }
			Vec<T, S>  operator/(const Vec<T, S>& oth) const noexcept { Vec<T, S> ret; std::transform(std::begin(arr), std::end(arr), std::begin(oth.arr), std::begin(ret.arr), [](const T& a, const T& b) { return a / b; }); return ret; }
			static T dot(const Vec<T, S>& a, const Vec<T, S>& b) noexcept { auto tmp = a * b; return std::accumulate(std::begin(tmp.arr), std::end(tmp.arr), 0); }
			static T distance2(const Vec<T, S>& a, const Vec<T, S>& b) noexcept
				{
				auto deltas = a - b;
				return std::accumulate(deltas.arr.begin(), deltas.arr.end(), 0, [](T a, T b) { return a + b * b; });
				}
			static T distance(const Vec<T, S>& a, const Vec<T, S>& b) noexcept { return std::sqrt(distance2(a, b)); }

			Vec<T, S>& operator+=(const Vec<T, S>& oth) noexcept { return *this = *this + oth; }
			Vec<T, S>& operator-=(const Vec<T, S>& oth) noexcept { return *this = *this - oth; }
			Vec<T, S>& operator*=(const Vec<T, S>& oth) noexcept { return *this = *this * oth; }
			Vec<T, S>& operator/=(const Vec<T, S>& oth) noexcept { return *this = *this / oth; }

			// OUTPUT
			friend std::ostream& utils::cout::operator<<(std::ostream& os, const Vec<T, S>& la);
		};

	}
