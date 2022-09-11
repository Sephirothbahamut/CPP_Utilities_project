#pragma once
#include <cmath>
#include <array>
#include <numbers>
#include <ostream>
#include <algorithm>

#include "containers/array.h"

//#ifdef COUT_CONTAINERS
#include "cout_utilities.h"
#include "cout_containers.h"
//#endif

#include "math/math.h"

namespace utils
	{
	template <typename T, size_t size> class vec;
	}

//#ifdef COUT_CONTAINERS
namespace utils::cout
	{
	template <typename T, size_t size>
	inline std::ostream& operator<<(std::ostream& os, const utils::vec<T, size>& vec) noexcept
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

	template <typename T, size_t SIZE>
	class vec
		{
		public:
			inline static constexpr size_t size = SIZE;
			using value_type = T;
			using root_t = vec<T, size>;

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

			static T dot(const root_t& a, const root_t& b) noexcept { T ret{0}; for (size_t i{0}; i < size; i++) { ret += a[i] * b[i]; } return ret; }
		};
	}

namespace utils::concepts
	{
	template <typename T>
	concept vec = std::derived_from<T, utils::vec<typename T::value_type, T::size>>;

	//TODO when standard allows concepts within types replace the current concept with the following one inside vec class
	//template <typename T>
	//concept is_compatible = std::derived_from<T, vec<typename value_type, size>>;
	template <typename T, typename other>
	concept vec_compatible = vec<other> && std::derived_from<std::remove_cvref_t<T>, utils::vec<typename other::value_type, other::size>>;
	}

namespace utils::operators
	{
	template <concepts::vec vec_t> bool operator==(const vec_t& a, const vec_t& b) noexcept { for (size_t i{0}; i < vec_t::size; i++) { if (a[i] != b[i]) { return false; } }; return true; }
	template <concepts::vec vec_t> bool operator!=(const vec_t& a, const vec_t& b) noexcept { return !(a == b); }

	
	// OPERATORS
	template <concepts::vec vec_t> vec_t  operator- (const vec_t& v) noexcept { vec_t ret; for (size_t i{ 0 }; i < vec_t::size; i++) { ret[i] = -v[i]; }; return ret; }
// 
	// VEC & SCALAR OPERATORS

	template <concepts::vec vec_t> vec_t  operator++(const vec_t& v) noexcept { return (*this) += typename vec_t::value_type{1}; }
	template <concepts::vec vec_t> vec_t  operator--(const vec_t& v) noexcept { return (*this) -= typename vec_t::value_type{1}; }
	
	template <concepts::vec vec_t> vec_t  operator+ (const vec_t& a, const typename vec_t::value_type& b) noexcept { vec_t ret; for (size_t i{0}; i < vec_t::size; i++) { ret[i] = a[i] + b; }; return ret; }
	template <concepts::vec vec_t> vec_t  operator- (const vec_t& a, const typename vec_t::value_type& b) noexcept { vec_t ret; for (size_t i{0}; i < vec_t::size; i++) { ret[i] = a[i] - b; }; return ret; }
	template <concepts::vec vec_t> vec_t  operator* (const vec_t& a, const typename vec_t::value_type& b) noexcept { vec_t ret; for (size_t i{0}; i < vec_t::size; i++) { ret[i] = a[i] * b; }; return ret; }
	template <concepts::vec vec_t> vec_t  operator/ (const vec_t& a, const typename vec_t::value_type& b) noexcept { vec_t ret; for (size_t i{0}; i < vec_t::size; i++) { ret[i] = a[i] / b; }; return ret; }
	
	template <concepts::vec vec_t> vec_t& operator+=(      vec_t& a, const typename vec_t::value_type& b) noexcept { return a = a + b; }
	template <concepts::vec vec_t> vec_t& operator-=(      vec_t& a, const typename vec_t::value_type& b) noexcept { return a = a - b; }
	template <concepts::vec vec_t> vec_t& operator*=(      vec_t& a, const typename vec_t::value_type& b) noexcept { return a = a * b; }
	template <concepts::vec vec_t> vec_t& operator/=(      vec_t& a, const typename vec_t::value_type& b) noexcept { return a = a / b; }

	// SCALAR & VEC OPERATORS
	template <concepts::vec vec_t> vec_t  operator- (const typename vec_t::value_type& a, const vec_t& b) noexcept { vec_t ret; for (size_t i{0}; i < vec_t::size; i++) { ret[i] = a - b[i]; }; return ret; }
	template <concepts::vec vec_t> vec_t  operator/ (const typename vec_t::value_type& a, const vec_t& b) noexcept { vec_t ret; for (size_t i{0}; i < vec_t::size; i++) { ret[i] = a / b[i]; }; return ret; }

	// VEC & VEC OPERATORS
	//TODO vec&vec operators when other has a different size

	template <concepts::vec vec_t> vec_t  operator+ (const vec_t& a, const vec_t& b) noexcept { vec_t ret{}; for (size_t i{0}; i < vec_t::size; i++) { ret[i] = a[i] + b[i]; }; return ret; }
	template <concepts::vec vec_t> vec_t  operator- (const vec_t& a, const vec_t& b) noexcept { vec_t ret{}; for (size_t i{0}; i < vec_t::size; i++) { ret[i] = a[i] - b[i]; }; return ret; }
	template <concepts::vec vec_t> vec_t  operator* (const vec_t& a, const vec_t& b) noexcept { vec_t ret{}; for (size_t i{0}; i < vec_t::size; i++) { ret[i] = a[i] * b[i]; }; return ret; }
	template <concepts::vec vec_t> vec_t  operator/ (const vec_t& a, const vec_t& b) noexcept { vec_t ret{}; for (size_t i{0}; i < vec_t::size; i++) { ret[i] = a[i] / b[i]; }; return ret; }
	
	template <concepts::vec vec_t> vec_t& operator+=(      vec_t& a, const vec_t& b) noexcept { return a = a + b; }
	template <concepts::vec vec_t> vec_t& operator-=(      vec_t& a, const vec_t& b) noexcept { return a = a - b; }
	template <concepts::vec vec_t> vec_t& operator*=(      vec_t& a, const vec_t& b) noexcept { return a = a * b; }
	template <concepts::vec vec_t> vec_t& operator/=(      vec_t& a, const vec_t& b) noexcept { return a = a / b; }

	inline constexpr struct _dot
		{
		template <concepts::vec vec_t>
		class _inner;

		template <concepts::vec vec_t>
		inline friend _inner<vec_t> operator<(const vec_t& lhs, _dot proxy) noexcept { return {lhs}; }

		template <concepts::vec vec_t>
		class _inner
			{
			public:
				vec_t::value_type operator>(const vec_t& rhs) const noexcept { return vec_t::dot(lhs, rhs); }
				_inner(vec_t lhs) noexcept : lhs{lhs} {}
			private:
				const vec_t& lhs;
			};

		} dot;

	inline constexpr struct _cross
		{
		template <concepts::vec vec_t>
		class _inner;

		template <concepts::vec vec_t>
		inline friend _inner<vec_t> operator<(const vec_t& lhs, _cross proxy) noexcept { return {lhs}; }

		template <concepts::vec vec_t>
		class _inner
			{
			public:
				vec_t operator>(const vec_t& rhs) const noexcept { return lhs * rhs; }
				_inner(vec_t lhs) noexcept : lhs{lhs} {}
			private:
				const vec_t& lhs;
			};

		} cross;
	}

namespace utils::math
	{
	template <concepts::vec vec_t> vec_t abs(const vec_t& v) noexcept { vec_t ret; for (size_t i{0}; i < vec_t::size; i++) { ret[i] = std::abs(v[i]); } return ret; }

	template <concepts::vec vec_t>
	inline vec_t lerp(const vec_t& a, const vec_t& b, float t)
		{
		vec_t ret;
		for (size_t i{0}; i < vec_t::size; i++) { ret[i] = utils::lerp(a[i], b[i], t); } //TODO decomment
		return ret;
		}

	template <concepts::vec vec_t>
	inline vec_t clamp(const vec_t& in, const vec_t& min, const vec_t& max) { vec_t ret; for (size_t i{0}; i < vec_t::size; i++) { ret[i] = utils::clamp(in[i], min[i], max[i]); } return ret; }

	template <concepts::vec vec_t>
	inline vec_t min(const vec_t& a, const vec_t& b) { vec_t ret; for (size_t i{0}; i < vec_t::size; i++) { ret[i] = utils::min(a[i], b._data[i]); } return ret; }

	template <concepts::vec vec_t>
	inline vec_t max(const vec_t& a, const vec_t& b) { vec_t ret; for (size_t i{0}; i < vec_t::size; i++) { ret[i] = utils::max(a[i], b._data[i]); } return ret; }
	}