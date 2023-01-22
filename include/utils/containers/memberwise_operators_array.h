#pragma once
#include <cmath>
#include <array>
#include <numbers>
#include <ostream>
#include <algorithm>

#include "../concepts.h"
#include "../math/math.h"
#include "../math/identity_numbers.h"
#include "../output/std_containers.h"

namespace utils::containers::memberwise_operators
	{
	template <typename T, size_t SIZE>
	class arr;

	namespace concepts
		{
		template <typename T>
		concept arr = std::derived_from<T, utils::containers::memberwise_operators::arr<typename T::value_type, T::static_size>>;
		}

	template <typename T, size_t SIZE>
	class arr : public std::array<T, SIZE>
		{
		public:
			using value_type = T;
			using root_t = arr<T, SIZE>;
			inline static constexpr size_t static_size{SIZE};

			arr() noexcept requires(utils::concepts::default_constructible<value_type>) = default;

			template<std::same_as<T> ...Ts>
				requires(sizeof...(Ts) == SIZE)
			arr(Ts ...values) noexcept : std::array<T, SIZE>{ values... } {}

			// If there are less parameters, the last one is used as filler for the rest of the array
			template<std::same_as<T> ...Ts>
				requires(sizeof...(Ts) < SIZE)
			arr(Ts ...values) noexcept : std::array<T, SIZE>{ values... } 
				{
				for (size_t i = sizeof...(Ts); i < SIZE; i++)
					{
					(*this)[i] = (*this)[sizeof...(Ts) - 1];
					}
				}

			template <concepts::arr T>
			arr(T copy, value_type default_value = utils::math::identity::sum<value_type>()) noexcept
				requires std::convertible_to<typename T::value_type, value_type>
				{
				if constexpr (copy.size() >= static_size)
					{
					for (size_t i = 0; i < static_size; i++) { (*this)[i] = copy[i]; }
					}
				else
					{
					size_t i = 0;
					for (; i < copy .size(); i++) { (*this)[i] = copy[i]      ; }
					for (; i < this->size(); i++) { (*this)[i] = default_value; }
					}
				}
		};

	//TODO concept lambda F should have the following signature:
	// (const value_type&, const value_type&)->value_type
	template <concepts::arr a_t, concepts::arr b_t, typename F>
	constexpr a_t operation(const a_t& a, const b_t& b, F f) noexcept
		requires std::convertible_to<decltype(f(a[0], b[0])), typename a_t::value_type>
		{
		constexpr size_t min_size{std::min(a_t::static_size, b_t::static_size)};
		a_t ret;

		size_t i{0};
		for (; i < min_size; i++)
			{
			ret[i] = f(a[i], b[i]);
			}
		for (; i < ret.size(); i++)
			{
			ret[i] = a[i];
			}

		return ret;
		}

	//TODO concept lambda F should have the following signature:
	// (value_type&, const value_type&)->value_type&
	template <concepts::arr a_t, concepts::arr b_t, typename F>
	constexpr a_t& operation_self_assign(a_t& a, const b_t& b, F f) noexcept
		requires std::convertible_to<decltype(f(a[0], b[0])), typename a_t::value_type>
		{
		constexpr size_t min_size{std::min(a_t::static_size, b_t::static_size)};

		for (size_t i{0}; i < min_size; i++)
			{
			f(a[i], b[i]);
			}

		return a;
		}

#pragma region operators
	template <concepts::arr a_t> a_t  operator- (const a_t& v) noexcept { a_t ret; for (size_t i{ 0 }; i < v.size(); i++) { ret[i] = -v[i]; }; return ret; }

#pragma region scalar
	template <concepts::arr a_t> a_t  operator++(const a_t& v) noexcept { return a_t{v} += typename a_t::value_type{1}; }
	template <concepts::arr a_t> a_t  operator--(const a_t& v) noexcept { return a_t{v} -= typename a_t::value_type{1}; }

	template <concepts::arr a_t> a_t  operator+ (const a_t& a, const typename a_t::value_type& b) noexcept { a_t ret; for (size_t i{0}; i < a.size(); i++) { ret[i] = a[i] + b; }; return ret; }
	template <concepts::arr a_t> a_t  operator- (const a_t& a, const typename a_t::value_type& b) noexcept { a_t ret; for (size_t i{0}; i < a.size(); i++) { ret[i] = a[i] - b; }; return ret; }
	template <concepts::arr a_t> a_t  operator* (const a_t& a, const typename a_t::value_type& b) noexcept { a_t ret; for (size_t i{0}; i < a.size(); i++) { ret[i] = a[i] * b; }; return ret; }
	template <concepts::arr a_t> a_t  operator/ (const a_t& a, const typename a_t::value_type& b) noexcept { a_t ret; for (size_t i{0}; i < a.size(); i++) { ret[i] = a[i] / b; }; return ret; }

	template <concepts::arr a_t> a_t& operator+=(      a_t& a, const typename a_t::value_type& b) noexcept { return a = a + b; }
	template <concepts::arr a_t> a_t& operator-=(      a_t& a, const typename a_t::value_type& b) noexcept { return a = a - b; }
	template <concepts::arr a_t> a_t& operator*=(      a_t& a, const typename a_t::value_type& b) noexcept { return a = a * b; }
	template <concepts::arr a_t> a_t& operator/=(      a_t& a, const typename a_t::value_type& b) noexcept { return a = a / b; }


	template <concepts::arr a_t> a_t  operator- (const typename a_t::value_type& a, const a_t& b) noexcept { a_t ret; for (size_t i{0}; i < b.size(); i++) { ret[i] = a - b[i]; }; return ret; }
	template <concepts::arr a_t> a_t  operator/ (const typename a_t::value_type& a, const a_t& b) noexcept { a_t ret; for (size_t i{0}; i < b.size(); i++) { ret[i] = a / b[i]; }; return ret; }
#pragma endregion scalar

#pragma region memberwise_operators_array
	template <concepts::arr a_t, concepts::arr b_t> a_t  operator+ (const a_t& a, const b_t& b) noexcept { return operation            (a, b, [](const a_t::value_type& a, const b_t::value_type& b) { return a +  b; }); }
	template <concepts::arr a_t, concepts::arr b_t> a_t  operator- (const a_t& a, const b_t& b) noexcept { return operation            (a, b, [](const a_t::value_type& a, const b_t::value_type& b) { return a -  b; }); }
	template <concepts::arr a_t, concepts::arr b_t> a_t  operator* (const a_t& a, const b_t& b) noexcept { return operation            (a, b, [](const a_t::value_type& a, const b_t::value_type& b) { return a *  b; }); }
	template <concepts::arr a_t, concepts::arr b_t> a_t  operator/ (const a_t& a, const b_t& b) noexcept { return operation            (a, b, [](const a_t::value_type& a, const b_t::value_type& b) { return a /  b; }); }

	template <concepts::arr a_t, concepts::arr b_t> a_t& operator+=(      a_t& a, const b_t& b) noexcept { return operation_self_assign(a, b, [](      a_t::value_type& a, const b_t::value_type& b) { return a += b; }); }
	template <concepts::arr a_t, concepts::arr b_t> a_t& operator-=(      a_t& a, const b_t& b) noexcept { return operation_self_assign(a, b, [](      a_t::value_type& a, const b_t::value_type& b) { return a -= b; }); }
	template <concepts::arr a_t, concepts::arr b_t> a_t& operator*=(      a_t& a, const b_t& b) noexcept { return operation_self_assign(a, b, [](      a_t::value_type& a, const b_t::value_type& b) { return a *= b; }); }
	template <concepts::arr a_t, concepts::arr b_t> a_t& operator/=(      a_t& a, const b_t& b) noexcept { return operation_self_assign(a, b, [](      a_t::value_type& a, const b_t::value_type& b) { return a /= b; }); }

#pragma endregion memberwise_operators_array
#pragma endregion operators
	}

namespace utils::math
	{
	template <utils::containers::memberwise_operators::concepts::arr a_t> 
	a_t abs(const a_t& v) noexcept { a_t ret; for (size_t i{0}; i < a_t::static_size; i++) { ret[i] = std::abs(v[i]); } return ret; }
	
	template <utils::containers::memberwise_operators::concepts::arr a_t>
	inline a_t lerp(const a_t& a, const a_t& b, float t)
		{
		a_t ret;
		for (size_t i{0}; i < a_t::static_size; i++) { ret[i] = utils::math::lerp(a[i], b[i], t); } //TODO decomment
		return ret;
		}
	
	template <utils::containers::memberwise_operators::concepts::arr a_t>
	inline a_t clamp(const a_t& in, const a_t& min, const a_t& max) 
		{
		a_t ret;
		for (size_t i = 0; i < a_t::static_size; i++)
			{
			ret[i] = utils::math::clamp(in[i], min[i], max[i]);
			}
		return ret;
		}

	template <utils::containers::memberwise_operators::concepts::arr a_t, utils::containers::memberwise_operators::concepts::arr b_t>
	inline a_t min(const a_t& a, const b_t& b) { return utils::containers::memberwise_operators::operation(a, b, [](const a_t::value_type& a, const b_t::value_type& b) { return utils::math::min(a, b); }); }

	template <utils::containers::memberwise_operators::concepts::arr a_t, utils::containers::memberwise_operators::concepts::arr b_t>
	inline a_t max(const a_t& a, const b_t& b) { return utils::containers::memberwise_operators::operation(a, b, [](const a_t::value_type& a, const b_t::value_type& b) { return utils::math::max(a, b); }); }
	}
