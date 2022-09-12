#pragma once
#include <cmath>
#include <ranges>
#include <numbers>
#include <ostream>
#include <algorithm>
#include <execution>

#include "concepts.h"
#include "../../math/math.h"
#include "../../math/identity_numbers.h"
#include "../../output/std_containers.h"

namespace utils::containers::memberwise_operators::par
	{
	//TODO concept lambda F should have the following signature:
	// (const value_type&, const value_type&)->value_type
	template <concepts::stdvecarr a_t, concepts::stdvecarr b_t, typename F>
	constexpr a_t operation(const a_t& a, const b_t& b, F f) noexcept
		requires std::convertible_to<decltype(f(a[0], b[0])), typename a_t::value_type>
		{
		size_t min_size{std::min(a.size(), b.size())};
		a_t ret;
		if constexpr (concepts::stdvec<a_t>) { ret.resize(a.size()); }

		std::for_each(std::execution::par_unseq, ret.begin(), ret.end(), [&](a_t::value_type& ret_value)
			{
			auto index{static_cast<size_t>(&ret_value - ret.data())};
			if (index < min_size) { ret_value = f(a[index], b[index]); }
			else { ret_value = a[index]; }
			});

		return ret;
		}

	//TODO concept lambda F should have the following signature:
	// (value_type&, const value_type&)->value_type&
	template <concepts::stdvecarr a_t, concepts::stdvecarr b_t, typename F>
	constexpr a_t& operation_self_assign(a_t& a, const b_t& b, F f) noexcept
		requires std::convertible_to<decltype(f(a[0], b[0])), typename a_t::value_type>
		{
		size_t min_size{std::min(a.size(), b.size())};

		std::for_each(std::execution::par_unseq, a.begin(), a.end(), [&](a_t::value_type& a_value)
			{
			auto index{static_cast<size_t>(&a_value - a.data())};
			if (index < min_size) { f(a_value, b[index]); }
			});

		return a;
		}
		
	//TODO concept lambda F should have the following signature:
	// (const value_type&, const value_type&)->value_type
	template <concepts::stdvecarr a_t, typename F>
	constexpr a_t operation(const a_t& a, const typename a_t::value_type& b, F f) noexcept
		requires std::convertible_to<decltype(f(a[0], b)), typename a_t::value_type>
		{
		a_t ret;
		if constexpr (concepts::stdvec<a_t>) { ret.resize(a.size()); }

		std::for_each(std::execution::par_unseq, ret.begin(), ret.end(), [&](a_t::value_type& ret_value)
			{
			auto index{static_cast<size_t>(&ret_value - ret.data())};
			ret_value = f(a[index], b);
			});

		return ret;
		}

	//TODO concept lambda F should have the following signature:
	// (value_type&, const value_type&)->value_type&
	template <concepts::stdvecarr a_t, typename F>
	constexpr a_t& operation_self_assign(a_t& a, const typename a_t::value_type& b, F f) noexcept
		requires std::convertible_to<decltype(f(a[0], b)), typename a_t::value_type>
		{
		std::for_each(std::execution::par_unseq, a.begin(), a.end(), [&](a_t::value_type& a_value)
			{
			auto index{static_cast<size_t>(&a_value - a.data())};
			f(a_value, b);
			});

		return a;
		}

#pragma region operators
	template <concepts::stdvecarr a_t> a_t  operator- (const a_t& v) noexcept { a_t ret; for (size_t i{0}; i < v.size(); i++) { ret[i] = -v[i]; }; return ret; }

#pragma region scalar
	template <concepts::stdvecarr a_t> a_t  operator++(const a_t& v) noexcept { return (*this) += typename a_t::value_type{1}; }
	template <concepts::stdvecarr a_t> a_t  operator--(const a_t& v) noexcept { return (*this) -= typename a_t::value_type{1}; }

	template <concepts::stdvecarr a_t> a_t  operator+ (const a_t& a, const typename a_t::value_type& b) noexcept { return operation            (a, b, [](const a_t::value_type& a, const a_t::value_type& b) { return a +  b; }); }
	template <concepts::stdvecarr a_t> a_t  operator- (const a_t& a, const typename a_t::value_type& b) noexcept { return operation            (a, b, [](const a_t::value_type& a, const a_t::value_type& b) { return a -  b; }); }
	template <concepts::stdvecarr a_t> a_t  operator* (const a_t& a, const typename a_t::value_type& b) noexcept { return operation            (a, b, [](const a_t::value_type& a, const a_t::value_type& b) { return a *  b; }); }
	template <concepts::stdvecarr a_t> a_t  operator/ (const a_t& a, const typename a_t::value_type& b) noexcept { return operation            (a, b, [](const a_t::value_type& a, const a_t::value_type& b) { return a /  b; }); }

	template <concepts::stdvecarr a_t> a_t& operator+=(      a_t& a, const typename a_t::value_type& b) noexcept { return operation_self_assign(a, b, [](      a_t::value_type& a, const a_t::value_type& b) { return a += b; }); }
	template <concepts::stdvecarr a_t> a_t& operator-=(      a_t& a, const typename a_t::value_type& b) noexcept { return operation_self_assign(a, b, [](      a_t::value_type& a, const a_t::value_type& b) { return a -= b; }); }
	template <concepts::stdvecarr a_t> a_t& operator*=(      a_t& a, const typename a_t::value_type& b) noexcept { return operation_self_assign(a, b, [](      a_t::value_type& a, const a_t::value_type& b) { return a *= b; }); }
	template <concepts::stdvecarr a_t> a_t& operator/=(      a_t& a, const typename a_t::value_type& b) noexcept { return operation_self_assign(a, b, [](      a_t::value_type& a, const a_t::value_type& b) { return a /= b; }); }


	template <concepts::stdvecarr a_t> a_t  operator- (const typename a_t::value_type& a, const a_t& b) noexcept { return operation            (b, a, [](const a_t::value_type& b, const a_t::value_type& a) { return a -  b; }); }
	template <concepts::stdvecarr a_t> a_t  operator/ (const typename a_t::value_type& a, const a_t& b) noexcept { return operation            (b, a, [](const a_t::value_type& b, const a_t::value_type& a) { return a /  b; }); }
#pragma endregion scalar

#pragma region container
	template <concepts::stdvecarr a_t, concepts::stdvecarr b_t> a_t  operator+ (const a_t& a, const b_t& b) noexcept { return operation            (a, b, [](const a_t::value_type& a, const b_t::value_type& b) { return a + b; }); }
	template <concepts::stdvecarr a_t, concepts::stdvecarr b_t> a_t  operator- (const a_t& a, const b_t& b) noexcept { return operation            (a, b, [](const a_t::value_type& a, const b_t::value_type& b) { return a - b; }); }
	template <concepts::stdvecarr a_t, concepts::stdvecarr b_t> a_t  operator* (const a_t& a, const b_t& b) noexcept { return operation            (a, b, [](const a_t::value_type& a, const b_t::value_type& b) { return a * b; }); }
	template <concepts::stdvecarr a_t, concepts::stdvecarr b_t> a_t  operator/ (const a_t& a, const b_t& b) noexcept { return operation            (a, b, [](const a_t::value_type& a, const b_t::value_type& b) { return a / b; }); }

	template <concepts::stdvecarr a_t, concepts::stdvecarr b_t> a_t& operator+=(      a_t& a, const b_t& b) noexcept { return operation_self_assign(a, b, [](      a_t::value_type& a, const b_t::value_type& b) { return a += b; }); }
	template <concepts::stdvecarr a_t, concepts::stdvecarr b_t> a_t& operator-=(      a_t& a, const b_t& b) noexcept { return operation_self_assign(a, b, [](      a_t::value_type& a, const b_t::value_type& b) { return a -= b; }); }
	template <concepts::stdvecarr a_t, concepts::stdvecarr b_t> a_t& operator*=(      a_t& a, const b_t& b) noexcept { return operation_self_assign(a, b, [](      a_t::value_type& a, const b_t::value_type& b) { return a *= b; }); }
	template <concepts::stdvecarr a_t, concepts::stdvecarr b_t> a_t& operator/=(      a_t& a, const b_t& b) noexcept { return operation_self_assign(a, b, [](      a_t::value_type& a, const b_t::value_type& b) { return a /= b; }); }

#pragma endregion container
#pragma endregion operators
	}