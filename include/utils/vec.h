#pragma once
#include <cmath>
#include <array>
#include <numbers>
#include <ostream>
#include <algorithm>

#include "custom_operators.h"
#include "containers/array.h"

//#ifdef COUT_CONTAINERS
#include "cout_utilities.h"
#include "cout_containers.h"
//#endif

#include "math/math.h"

namespace utils
	{
	template <typename T, size_t size, typename leaf_t> class vec;
	}

//#ifdef COUT_CONTAINERS
namespace utils::cout
	{
	template <typename T, size_t size, typename leaf_t>
	inline std::ostream& operator<<(std::ostream& os, const utils::vec<T, size, leaf_t>& vec) noexcept
		{
		namespace ccu = cout::support;
		return os << ccu::type << "vec" << size << vec._data;
		}
	}
//#endif

namespace utils
	{
	template <template<typename> class To, typename T, class From>
	inline static To<T> vec_cast(From from) { return { static_cast<T>(from.x), static_cast<T>(from.y) }; }

	template <typename T, size_t SIZE, typename LEAF_T = void>
	class vec
		{
		public:
			inline static constexpr size_t size = SIZE;
			using leaf_t = LEAF_T;
			using root_t = vec<T, size, leaf_t>;

			vec() = default;

			template<std::same_as<T> ...Ts>
				requires(sizeof...(Ts) == size)
			vec(Ts ...values) : _data{ values... } {}

			//template <typename other_t, size_t other_size>
			//vec(vec<other_t, other_size> other)           noexcept { containers::array::copy(_data, other._data); } 

			std::array<T, size> _data;

			//TODO add AT functions and call AT in [] in debug mode
			T& operator[](size_t index)       noexcept { return _data[index]; }
			const T& operator[](size_t index) const noexcept { return _data[index]; }

#pragma region operators
#pragma region on root
			bool          operator==(const root_t& oth) const noexcept { for (size_t i{ 0 }; i < size; i++) { if (_data[i] != oth[i]) { return false; } }; return true; }
			bool          operator!=(const root_t& oth) const noexcept { return !(*this == oth); }
			static T dot(const root_t& a, const root_t& b) noexcept { T ret{ 0 }; for (size_t i{ 0 }; i < size; i++) { ret += a[i] * b[i]; } return ret; }

#pragma endregion on root
#pragma region on leaf
			// OPERATORS
			leaf_t  operator-() const noexcept { leaf_t ret; for (size_t i{ 0 }; i < size; i++) { ret[i] = -static_cast<const leaf_t&>(*this)[i]; }; return ret; }

			// VEC & SCALAR OPERATORS
			leaf_t  operator++()       noexcept { return static_cast<leaf_t&>(*this) += T(1); }
			leaf_t  operator--()       noexcept { return static_cast<leaf_t&>(*this) -= T(1); }

			//leaf_t  operator+ (const T n) const noexcept { leaf_t ret; for (size_t i{ 0 }; i < size; i++) { ret[i] = static_cast<const leaf_t&>(*this)[i] + n; }; return ret; }
			//leaf_t  operator- (const T n) const noexcept { leaf_t ret; for (size_t i{ 0 }; i < size; i++) { ret[i] = static_cast<const leaf_t&>(*this)[i] - n; }; return ret; }
			leaf_t  operator* (const T n) const noexcept { leaf_t ret; for (size_t i{ 0 }; i < size; i++) { ret[i] = static_cast<const leaf_t&>(*this)[i] * n; }; return ret; }
			leaf_t  operator/ (const T n) const noexcept { leaf_t ret; for (size_t i{ 0 }; i < size; i++) { ret[i] = static_cast<const leaf_t&>(*this)[i] / n; }; return ret; }

			leaf_t& operator+=(const T n)       noexcept { return static_cast<leaf_t&>(*this) = static_cast<leaf_t&>(*this) + n; }
			leaf_t& operator-=(const T n)       noexcept { return static_cast<leaf_t&>(*this) = static_cast<leaf_t&>(*this) - n; }
			leaf_t& operator*=(const T n)       noexcept { return static_cast<leaf_t&>(*this) = static_cast<leaf_t&>(*this) * n; }
			leaf_t& operator/=(const T n)       noexcept { return static_cast<leaf_t&>(*this) = static_cast<leaf_t&>(*this) / n; }

			// VEC & VEC OPERATORS
			//TODO vec&vec operators when other has a different size

			leaf_t  operator+ (const leaf_t& oth) const noexcept { auto ret{ static_cast<const leaf_t&>(*this) }; for (size_t i{ 0 }; i < size; i++) { ret[i] = static_cast<const leaf_t&>(*this)[i] + oth[i]; }; return ret; }
			leaf_t  operator- (const leaf_t& oth) const noexcept { auto ret{ static_cast<const leaf_t&>(*this) }; for (size_t i{ 0 }; i < size; i++) { ret[i] = static_cast<const leaf_t&>(*this)[i] - oth[i]; }; return ret; }
			leaf_t  operator* (const leaf_t& oth) const noexcept { auto ret{ static_cast<const leaf_t&>(*this) }; for (size_t i{ 0 }; i < size; i++) { ret[i] = static_cast<const leaf_t&>(*this)[i] * oth[i]; }; return ret; }
			leaf_t  operator/ (const leaf_t& oth) const noexcept { auto ret{ static_cast<const leaf_t&>(*this) }; for (size_t i{ 0 }; i < size; i++) { ret[i] = static_cast<const leaf_t&>(*this)[i] / oth[i]; }; return ret; }

			leaf_t& operator+=(const leaf_t& oth)       noexcept { return static_cast<leaf_t&>(*this) = static_cast<leaf_t&>(*this) + oth; }
			leaf_t& operator-=(const leaf_t& oth)       noexcept { return static_cast<leaf_t&>(*this) = static_cast<leaf_t&>(*this) - oth; }
			leaf_t& operator*=(const leaf_t& oth)       noexcept { return static_cast<leaf_t&>(*this) = static_cast<leaf_t&>(*this) * oth; }
			leaf_t& operator/=(const leaf_t& oth)       noexcept { return static_cast<leaf_t&>(*this) = static_cast<leaf_t&>(*this) / oth; }

#pragma endregion on leaf
#pragma endregion operators

			leaf_t  abs() const noexcept { leaf_t ret; for (size_t i{ 0 }; i < size; i++) { ret[i] = std::abs(_data[i]); } return ret; }
			leaf_t  min(leaf_t min) const noexcept { leaf_t ret; for (size_t i{ 0 }; i < size; i++) { ret[i] = std::min(_data[i], min._data[i]); } return ret; }
			leaf_t  max(leaf_t max) const noexcept { leaf_t ret; for (size_t i{ 0 }; i < size; i++) { ret[i] = std::max(_data[i], max._data[i]); } return ret; }
			leaf_t  clamp(leaf_t min, leaf_t max) const noexcept { leaf_t ret; for (size_t i{ 0 }; i < size; i++) { ret[i] = std::clamp(_data[i], min._data[i], max._data[i]); } return ret; }

			leaf_t& abs_self()       noexcept { return static_cast<leaf_t&>(*this) = abs(); }
			leaf_t& min_self(leaf_t min)       noexcept { return static_cast<leaf_t&>(*this) = this->min(min); }
			leaf_t& max_self(leaf_t max)       noexcept { return static_cast<leaf_t&>(*this) = this->max(max); }
			leaf_t& clamp_self(leaf_t min, leaf_t max)       noexcept { return static_cast<leaf_t&>(*this) = clamp(min, max); }


			static leaf_t lerp(const leaf_t& a, const leaf_t& b, T t) noexcept
				{
				leaf_t ret;
				for (size_t i{ 0 }; i < size; i++) { ret[i] = utils::lerp(a[i], b[i], t); } //TODO decomment
				return ret;
				}
		};


	template <typename T, size_t size, typename leaf_t>
	vec<T, size, leaf_t> abs(const vec<T, size, leaf_t>& vec) noexcept { return vec.abs(); }

	namespace operators
		{
		inline constexpr struct _dot
			{
			template <typename T, size_t size, typename leaf_t>
			class _inner;

			template <typename T, size_t size, typename leaf_t>
			inline friend _inner<T, size, leaf_t> operator<(vec<T, size, leaf_t> lhs, _dot proxy) noexcept { return { lhs }; }

			template <typename T, size_t size, typename leaf_t>
			class _inner
				{
				public:
					T operator>(vec<T, size, leaf_t> rhs) const noexcept { return vec<T, size, leaf_t>::dot(lhs, rhs); }
					_inner(vec<T, size, leaf_t> lhs) noexcept : lhs{ lhs } {}
				private:
					vec<T, size, leaf_t> lhs;
				};

			} dot;

		inline constexpr struct _cross
			{
			template <typename T, size_t size, typename leaf_t>
			class _inner;

			template <typename T, size_t size, typename leaf_t>
			inline friend _inner<T, size, leaf_t> operator<(vec<T, size, leaf_t> lhs, _cross proxy) noexcept { return { lhs }; }

			template <typename T, size_t size, typename leaf_t>
			class _inner
				{
				public:
					leaf_t operator>(vec<T, size, leaf_t> rhs) const noexcept { return lhs * rhs; }
					_inner(vec<T, size, leaf_t> lhs) noexcept : lhs{ lhs } {}
				private:
					vec<T, size, leaf_t> lhs;
				};

			} cross;
		}

	template <typename T, size_t size, typename leaf_t>
	inline leaf_t lerp(const vec<T, size, leaf_t>& a, const vec<T, size, leaf_t>& b, float t) { return vec<T, size, leaf_t>::lerp(a, b, t); }

	template <typename T, size_t size, typename leaf_t>
	inline vec<T, size, leaf_t> clamp(const vec<T, size, leaf_t>& in, const vec<T, size, leaf_t>& min, const vec<T, size, leaf_t>& max) { return in.clamp(min, max); }

	template <typename T, size_t size, typename leaf_t>
	inline leaf_t min(const vec<T, size, leaf_t>& in, const vec<T, size, leaf_t>& min) { return in.min(min); }

	template <typename T, size_t size, typename leaf_t>
	inline leaf_t max(const vec<T, size, leaf_t>& in, const vec<T, size, leaf_t>& max) { return in.max(max); }
	}