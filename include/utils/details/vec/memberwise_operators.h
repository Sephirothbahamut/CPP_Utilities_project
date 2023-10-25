#pragma once

#include <concepts>

#include "common.h"
#include "../../memory.h"
#include "../../compilation/gpu.h"

namespace utils::details::vec
	{
			
	template<typename COMMON_T>
	class memberwise_operators;

	namespace concepts
		{
		template <typename T>
		concept memberwise_operators = std::derived_from<T, vec::memberwise_operators<typename T::common_t>>;
		template <typename T, typename to>
		concept compatible_memberwise = memberwise_operators<T> && memberwise_operators<to> && std::convertible_to<typename T::nonref_value_type, typename to::nonref_value_type>;
		template <typename T, typename to>
		concept compatible_scalar = memberwise_operators<to> && std::convertible_to<T, typename to::nonref_value_type>;
		}

	template<typename COMMON_T>
	class memberwise_operators
		{
		public:
			using common_t          = COMMON_T;
			using derived_t         = common_t::derived_t;
			using value_type        = common_t::value_type;
			using nonref_derived_t  = common_t::nonref_derived_t;
			using nonref_value_type = common_t::nonref_value_type;
		private:
			utils_gpu_available constexpr const derived_t& derived() const noexcept { return static_cast<const derived_t&>(*this); }
			utils_gpu_available constexpr       derived_t& derived()       noexcept { return static_cast<      derived_t&>(*this); }

		public:
			template <auto callback>
			utils_gpu_available derived_t& for_each()
				{
				for (size_t i{0}; i < derived().size(); i++)
					{
					callback(derived()[i]);
					}
				return derived();
				}
			template <auto callback>
			utils_gpu_available derived_t for_each_to_new() const
				{
				nonref_derived_t ret;
				for (size_t i{0}; i < derived().size(); i++)
					{
					ret[i] = callback(derived()[i]);
					}
				return ret;
				}
		};

	template<auto callback, concepts::memberwise_operators a_t, concepts::compatible_scalar<a_t> b_t>
	utils_gpu_available constexpr a_t& operator_scalar_self_assign(a_t& a, const b_t& b) noexcept
		{
		for (size_t i{0}; i < a.size(); i++)
			{
			callback(a[i], b);
			}
		return a;
		}
	template<auto callback, concepts::memberwise_operators a_t, concepts::compatible_scalar<a_t> b_t>
	utils_gpu_available constexpr typename a_t::nonref_derived_t operator_scalar(const a_t& a, const b_t& b) noexcept
		{
		typename a_t::nonref_derived_t ret;
		for (size_t i{0}; i < a.size(); i++)
			{
			ret[i] = callback(a[i], b);
			}
		return ret;
		}

	template<auto callback, concepts::memberwise_operators a_t, concepts::compatible_memberwise<a_t> b_t>
	utils_gpu_available constexpr a_t& operator_vector_self_assign(a_t& a, const b_t& b) noexcept
		{
		size_t i{0};
		for (; i < std::min(a.size(), b.size()); i++)
			{
			callback(a[i], b[i]);
			}
		return a;
		}

	template<auto callback, concepts::memberwise_operators a_t, concepts::compatible_memberwise<a_t> b_t>
	utils_gpu_available constexpr typename get_larger<a_t, b_t>::type::nonref_derived_t operator_vector(const a_t& a, const b_t& b) noexcept
		{
		using ret_t = typename get_larger<a_t, b_t>::type::nonref_derived_t;
		ret_t ret;

		size_t i{0};
		for (; i < std::min(a.size(), b.size()); i++)
			{
			ret[i] = callback(a[i], b[i]);
			}
		
		if constexpr (a_t::static_size > b_t::static_size) { for (; i < a.size(); i++) { ret[i] = a[i]; } }
		else
		if constexpr (a_t::static_size < b_t::static_size) { for (; i < b.size(); i++) { ret[i] = b[i]; } }

		return ret;
		}

#pragma region scalar
	template<concepts::memberwise_operators a_t, concepts::compatible_scalar<a_t> b_t> utils_gpu_available constexpr a_t& operator+=(      a_t& a, const b_t& b) noexcept { return operator_scalar_self_assign<[](auto& a, const auto& b) { a += b; }>(a, b); }
	template<concepts::memberwise_operators a_t, concepts::compatible_scalar<a_t> b_t> utils_gpu_available constexpr a_t& operator-=(      a_t& a, const b_t& b) noexcept { return operator_scalar_self_assign<[](auto& a, const auto& b) { a -= b; }>(a, b); }
	template<concepts::memberwise_operators a_t, concepts::compatible_scalar<a_t> b_t> utils_gpu_available constexpr a_t& operator*=(      a_t& a, const b_t& b) noexcept { return operator_scalar_self_assign<[](auto& a, const auto& b) { a *= b; }>(a, b); }
	template<concepts::memberwise_operators a_t, concepts::compatible_scalar<a_t> b_t> utils_gpu_available constexpr a_t& operator/=(      a_t& a, const b_t& b) noexcept { return operator_scalar_self_assign<[](auto& a, const auto& b) { a /= b; }>(a, b); }
	template<concepts::memberwise_operators a_t, concepts::compatible_scalar<a_t> b_t> utils_gpu_available constexpr a_t& operator|=(      a_t& a, const b_t& b) noexcept { return operator_scalar_self_assign<[](auto& a, const auto& b) { a |= b; }>(a, b); }
	template<concepts::memberwise_operators a_t, concepts::compatible_scalar<a_t> b_t> utils_gpu_available constexpr a_t& operator&=(      a_t& a, const b_t& b) noexcept { return operator_scalar_self_assign<[](auto& a, const auto& b) { a &= b; }>(a, b); }
	//utils_gpu_available template<concepts::memberwise_operators a_t, concepts::compatible_scalar<a_t> b_t> constexpr a_t& operator =(a_t& a, const b_t& b) noexcept { for (size_t i{0}; i < a.size(); i++) { a[i] =  b; } return derived(); }

	template<concepts::memberwise_operators a_t, concepts::compatible_scalar<a_t> b_t> utils_gpu_available constexpr auto operator+ (const a_t& a, const b_t& b) noexcept { return operator_scalar<[](const auto& a, const auto& b) { return a + b; }>(a, b); }
	template<concepts::memberwise_operators a_t, concepts::compatible_scalar<a_t> b_t> utils_gpu_available constexpr auto operator- (const a_t& a, const b_t& b) noexcept { return operator_scalar<[](const auto& a, const auto& b) { return a - b; }>(a, b); }
	template<concepts::memberwise_operators a_t, concepts::compatible_scalar<a_t> b_t> utils_gpu_available constexpr auto operator* (const a_t& a, const b_t& b) noexcept { return operator_scalar<[](const auto& a, const auto& b) { return a * b; }>(a, b); }
	template<concepts::memberwise_operators a_t, concepts::compatible_scalar<a_t> b_t> utils_gpu_available constexpr auto operator/ (const a_t& a, const b_t& b) noexcept { return operator_scalar<[](const auto& a, const auto& b) { return a / b; }>(a, b); }
	template<concepts::memberwise_operators a_t, concepts::compatible_scalar<a_t> b_t> utils_gpu_available constexpr auto operator| (const a_t& a, const b_t& b) noexcept { return operator_scalar<[](const auto& a, const auto& b) { return a | b; }>(a, b); }
	template<concepts::memberwise_operators a_t, concepts::compatible_scalar<a_t> b_t> utils_gpu_available constexpr auto operator& (const a_t& a, const b_t& b) noexcept { return operator_scalar<[](const auto& a, const auto& b) { return a & b; }>(a, b); }
#pragma endregion scalar

#pragma region self
	template<concepts::memberwise_operators a_t> utils_gpu_available constexpr a_t::nonref_derived_t operator! (const a_t& a) noexcept { typename a_t::nonref_derived_t ret{a}; for (size_t i{0}; i < a.size(); i++) { ret[i] = !ret[i]; } return ret; }
	template<concepts::memberwise_operators a_t> utils_gpu_available constexpr a_t::nonref_derived_t operator- (const a_t& a) noexcept { typename a_t::nonref_derived_t ret{a}; for (size_t i{0}; i < a.size(); i++) { ret[i] = -ret[i]; } return ret; }
#pragma endregion self

#pragma region array
	template <concepts::memberwise_operators a_t, concepts::compatible_memberwise<a_t> b_t> utils_gpu_available constexpr a_t& operator+=(      a_t& a, const b_t& b) noexcept { return operator_vector_self_assign<[](auto& a, const auto& b) { a += b; }>(a, b); }
	template <concepts::memberwise_operators a_t, concepts::compatible_memberwise<a_t> b_t> utils_gpu_available constexpr a_t& operator-=(      a_t& a, const b_t& b) noexcept { return operator_vector_self_assign<[](auto& a, const auto& b) { a -= b; }>(a, b); }
	template <concepts::memberwise_operators a_t, concepts::compatible_memberwise<a_t> b_t> utils_gpu_available constexpr a_t& operator*=(      a_t& a, const b_t& b) noexcept { return operator_vector_self_assign<[](auto& a, const auto& b) { a *= b; }>(a, b); }
	template <concepts::memberwise_operators a_t, concepts::compatible_memberwise<a_t> b_t> utils_gpu_available constexpr a_t& operator/=(      a_t& a, const b_t& b) noexcept { return operator_vector_self_assign<[](auto& a, const auto& b) { a /= b; }>(a, b); }
	template <concepts::memberwise_operators a_t, concepts::compatible_memberwise<a_t> b_t> utils_gpu_available constexpr a_t& operator|=(      a_t& a, const b_t& b) noexcept { return operator_vector_self_assign<[](auto& a, const auto& b) { a |= b; }>(a, b); }
	template <concepts::memberwise_operators a_t, concepts::compatible_memberwise<a_t> b_t> utils_gpu_available constexpr a_t& operator&=(      a_t& a, const b_t& b) noexcept { return operator_vector_self_assign<[](auto& a, const auto& b) { a &= b; }>(a, b); }
	template <concepts::memberwise_operators a_t, concepts::compatible_memberwise<a_t> b_t> utils_gpu_available constexpr auto operator+ (const a_t& a, const b_t& b) noexcept { return operator_vector<[](const auto& a, const auto& b) { return a + b; }>(a, b); }
	template <concepts::memberwise_operators a_t, concepts::compatible_memberwise<a_t> b_t> utils_gpu_available constexpr auto operator- (const a_t& a, const b_t& b) noexcept { return operator_vector<[](const auto& a, const auto& b) { return a - b; }>(a, b); }
	template <concepts::memberwise_operators a_t, concepts::compatible_memberwise<a_t> b_t> utils_gpu_available constexpr auto operator* (const a_t& a, const b_t& b) noexcept { return operator_vector<[](const auto& a, const auto& b) { return a * b; }>(a, b); }
	template <concepts::memberwise_operators a_t, concepts::compatible_memberwise<a_t> b_t> utils_gpu_available constexpr auto operator/ (const a_t& a, const b_t& b) noexcept { return operator_vector<[](const auto& a, const auto& b) { return a / b; }>(a, b); }
	template <concepts::memberwise_operators a_t, concepts::compatible_memberwise<a_t> b_t> utils_gpu_available constexpr auto operator| (const a_t& a, const b_t& b) noexcept { return operator_vector<[](const auto& a, const auto& b) { return a | b; }>(a, b); }
	template <concepts::memberwise_operators a_t, concepts::compatible_memberwise<a_t> b_t> utils_gpu_available constexpr auto operator& (const a_t& a, const b_t& b) noexcept { return operator_vector<[](const auto& a, const auto& b) { return a & b; }>(a, b); }
	template <concepts::memberwise_operators a_t, concepts::compatible_memberwise<a_t> b_t> utils_gpu_available constexpr bool operator!=(const a_t& a, const b_t& b) noexcept { return !(a == b); }
	template <concepts::memberwise_operators a_t, concepts::compatible_memberwise<a_t> b_t> utils_gpu_available constexpr bool operator==(const a_t& a, const b_t& b) noexcept
		{
		size_t i{0};
		for (; i < std::min(a.size(), b.size()); i++)
			{
			if (a[i] != b[i]) { return false; }
			}

		if constexpr (a_t::static_size > b_t::static_size) { for (; i < a.size(); i++) { if (a[i] != typename a_t::nonref_value_type{}) { return false; } } }
		else
		if constexpr (a_t::static_size < b_t::static_size) { for (; i < b.size(); i++) { if (b[i] != typename b_t::nonref_value_type{}) { return false; } } }

		return true;
		}
	//template <concepts::compatible_array<derived_t> T2> constexpr derived_t& operator =(const T2& b) noexcept { for (size_t i{0}; i < std::min(derived().size(), b.size()); i++) { derived()[i] = b[i]; } return *this; }
#pragma endregion array
	}



namespace utils::math
	{
	template <utils::details::vec::concepts::array a_t>
	utils_gpu_available a_t abs(const a_t& v) noexcept { a_t ret; for (size_t i{0}; i < a_t::static_size; i++) { ret[i] = std::abs(v[i]); } return ret; }

	template <utils::details::vec::concepts::array a_t>
	utils_gpu_available inline a_t lerp(const a_t& a, const a_t& b, float t)
		{
		a_t ret;
		for (size_t i{0}; i < a_t::static_size; i++) { ret[i] = utils::math::lerp(a[i], b[i], t); }
		return ret;
		}

	template <utils::details::vec::concepts::array a_t>
	utils_gpu_available inline a_t clamp(const a_t& in, const a_t& min, const a_t& max)
		{
		a_t ret;
		for (size_t i = 0; i < a_t::static_size; i++)
			{
			ret[i] = utils::math::clamp(in[i], min[i], max[i]);
			}
		return ret;
		}

	template <utils::details::vec::concepts::array a_t, utils::details::vec::concepts::compatible_array<a_t> b_t>
	utils_gpu_available inline a_t min(const a_t& a, const b_t& b)
		{
		auto ret{a};
		for (size_t i{0}; i < std::min(a.size(), b.size()); i++) 
			{
			ret[i] = std::min(a[i], b[i]);
			} 

		return ret;
		}

	template <utils::details::vec::concepts::array a_t, utils::details::vec::concepts::compatible_array<a_t> b_t>
	utils_gpu_available inline a_t max(const a_t& a, const b_t& b)
		{
		auto ret{a};
		for (size_t i{0}; i < std::min(a.size(), b.size()); i++)
			{
			ret[i] = std::max(a[i], b[i]);
			}

		return ret;
		}
	}