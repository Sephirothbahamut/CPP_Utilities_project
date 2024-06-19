#pragma once

#include "base.h"
#include "definitions.h"
#include "../../storage.h"

namespace utils::details::vector
	{
	template<typename definitions_t>
	struct utils_oop_empty_bases memberwise_operators
		{
		private:
			inline static constexpr auto extent      {definitions_t::storage_t::extent      };
			inline static constexpr auto storage_type{definitions_t::storage_t::storage_type};
			using self_t        = typename definitions_t::self_t       ;
			using nonref_self_t = typename definitions_t::nonref_self_t;
			
			const self_t& self() const noexcept { return static_cast<const self_t&>(*this); }
			      self_t& self()       noexcept { return static_cast<      self_t&>(*this); }
		public:
			template <auto callback> utils_gpu_available void for_each() const noexcept { for (const auto& value : self()) { callback(value); } }
			template <auto callback> utils_gpu_available void for_each()       noexcept { for (      auto& value : self()) { callback(value); } }

			template <auto callback> utils_gpu_available nonref_self_t for_each_to_new() const noexcept
				{
				nonref_self_t ret;
				for (size_t i{0}; i < extent; i++)
					{
					ret[i] = callback(self()[i]);
					}
				return ret;
				}
			utils_gpu_available void for_each(auto callback) const noexcept { for (const auto& value : self()) { callback(value); } }
			utils_gpu_available void for_each(auto callback)       noexcept { for (      auto& value : self()) { callback(value); } }
			utils_gpu_available nonref_self_t for_each_to_new(auto callback) const noexcept
				{
				nonref_self_t ret;
				for (size_t i{0}; i < extent; i++)
					{
					ret[i] = callback(self()[i]);
					}
				return ret;
				}

			template <auto callback>
			utils_gpu_available constexpr self_t& operator_self_assign(const concepts::compatible_scalar<self_t> auto& scalar) noexcept
				{
				for (auto& value : self()) { callback(value, scalar); }
				return self();
				}
			template <typename callback_t>
			utils_gpu_available constexpr self_t& operator_self_assign(const concepts::compatible_scalar<self_t> auto& scalar, callback_t callback) noexcept
				{
				for (auto& value : self()) { callback(value, scalar); }
				return self();
				}

			template <auto callback>
			utils_gpu_available constexpr self_t& operator_self_assign(const concepts::compatible_vector<self_t> auto& other) noexcept
				{
				constexpr size_t other_extent{std::remove_cvref_t<decltype(other)>::extent};
				for (size_t i{0}; i < utils::math::min(self_t::extent, other_extent); i++)
					{
					callback(self()[i], other[i]);
					}
				return self();
				}
			template <typename callback_t>
			utils_gpu_available constexpr auto operator_self_assign(const concepts::compatible_vector<self_t> auto& other, callback_t callback) noexcept -> self_t&
				{
				constexpr size_t other_extent{std::remove_cvref_t<decltype(other)>::extent};
				for (size_t i{0}; i < utils::math::min(self_t::extent, other_extent); i++)
					{
					callback(self()[i], other[i]);
					}
				return self();
				}

			template <auto callback>
			utils_gpu_available constexpr auto operator_to_new(const concepts::compatible_scalar<self_t> auto& scalar) const noexcept -> nonref_self_t
				{
				nonref_self_t ret;
				for (size_t i{0}; i < self_t::extent; i++) { ret[i] = callback(self()[i], scalar); }
				return ret;
				}
			template <typename callback_t>
			utils_gpu_available constexpr auto operator_to_new(const concepts::compatible_scalar<self_t> auto& scalar, callback_t callback) noexcept -> nonref_self_t
				{
				nonref_self_t ret;
				for (size_t i{0}; i < self_t::extent; i++) { ret[i] = callback(self()[i], scalar); }
				return ret;
				}

			template <auto callback>
			utils_gpu_available constexpr auto operator_to_new(const concepts::compatible_vector<self_t> auto& other) const noexcept -> nonref_self_t
				{
				constexpr size_t other_extent{std::remove_cvref_t<decltype(other)>::extent};
				nonref_self_t ret;
				for (size_t i{0}; i < utils::math::min(self_t::extent, other_extent); i++)
					{
					ret[i] = callback(self()[i], other[i]);
					}
				return ret;
				}
			template <typename callback_t>
			utils_gpu_available constexpr auto operator_to_new(const concepts::compatible_vector<self_t> auto& other, callback_t callback) const noexcept -> nonref_self_t
				{
				constexpr size_t other_extent{std::remove_cvref_t<decltype(other)>::extent};
				nonref_self_t ret;
				for (size_t i{0}; i < utils::math::min(self_t::extent, other_extent); i++)
					{
					ret[i] = callback(self()[i], other[i]);
					}
				return ret;
				}

			utils_gpu_available constexpr self_t& operator=(const concepts::compatible_vector<self_t> auto& other) noexcept
				requires(!storage_type.is_const())
				{
				self().operator_self_assign<[](auto& a, const auto& b) { a = b; }>(other);
				return self();
				}

			utils_gpu_available constexpr self_t& operator=(const concepts::compatible_scalar<self_t> auto& other) noexcept
				requires(!storage_type.is_const())
				{
				self().operator_self_assign<[](auto& a, const auto& b) { a = b; }>(other);
				return self();
				}
		};
	}

	
	
#pragma region scalar
	template<utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_scalar<a_t> b_t> utils_gpu_available constexpr a_t& operator+=(      a_t& a, const b_t& b) noexcept { return a.operator_self_assign<[](      auto& a, const auto& b) {        a += b; }>(b); }
	template<utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_scalar<a_t> b_t> utils_gpu_available constexpr a_t& operator-=(      a_t& a, const b_t& b) noexcept { return a.operator_self_assign<[](      auto& a, const auto& b) {        a -= b; }>(b); }
	template<utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_scalar<a_t> b_t> utils_gpu_available constexpr a_t& operator*=(      a_t& a, const b_t& b) noexcept { return a.operator_self_assign<[](      auto& a, const auto& b) {        a *= b; }>(b); }
	template<utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_scalar<a_t> b_t> utils_gpu_available constexpr a_t& operator/=(      a_t& a, const b_t& b) noexcept { return a.operator_self_assign<[](      auto& a, const auto& b) {        a /= b; }>(b); }
	template<utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_scalar<a_t> b_t> utils_gpu_available constexpr a_t& operator%=(      a_t& a, const b_t& b) noexcept { return a.operator_self_assign<[](      auto& a, const auto& b) {        a %= b; }>(b); }
	template<utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_scalar<a_t> b_t> utils_gpu_available constexpr a_t& operator|=(      a_t& a, const b_t& b) noexcept { return a.operator_self_assign<[](      auto& a, const auto& b) {        a |= b; }>(b); }
	template<utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_scalar<a_t> b_t> utils_gpu_available constexpr a_t& operator&=(      a_t& a, const b_t& b) noexcept { return a.operator_self_assign<[](      auto& a, const auto& b) {        a &= b; }>(b); }
	template<utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_scalar<a_t> b_t> utils_gpu_available constexpr auto operator+ (const a_t& a, const b_t& b) noexcept { return a.operator_to_new     <[](const auto& a, const auto& b) { return a +  b; }>(b); }
	template<utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_scalar<a_t> b_t> utils_gpu_available constexpr auto operator- (const a_t& a, const b_t& b) noexcept { return a.operator_to_new     <[](const auto& a, const auto& b) { return a -  b; }>(b); }
	template<utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_scalar<a_t> b_t> utils_gpu_available constexpr auto operator* (const a_t& a, const b_t& b) noexcept { return a.operator_to_new     <[](const auto& a, const auto& b) { return a *  b; }>(b); }
	template<utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_scalar<a_t> b_t> utils_gpu_available constexpr auto operator/ (const a_t& a, const b_t& b) noexcept { return a.operator_to_new     <[](const auto& a, const auto& b) { return a /  b; }>(b); }
	template<utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_scalar<a_t> b_t> utils_gpu_available constexpr auto operator% (const a_t& a, const b_t& b) noexcept { return a.operator_to_new     <[](const auto& a, const auto& b) { return a %  b; }>(b); }
	template<utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_scalar<a_t> b_t> utils_gpu_available constexpr auto operator| (const a_t& a, const b_t& b) noexcept { return a.operator_to_new     <[](const auto& a, const auto& b) { return a |  b; }>(b); }
	template<utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_scalar<a_t> b_t> utils_gpu_available constexpr auto operator& (const a_t& a, const b_t& b) noexcept { return a.operator_to_new     <[](const auto& a, const auto& b) { return a &  b; }>(b); }
#pragma endregion scalar

#pragma region self
	template<utils::details::vector::concepts::vector a_t> utils_gpu_available constexpr auto operator!(const a_t& a) noexcept { return a.for_each_to_new<[](const auto& value) { return !value; }>(); }
	template<utils::details::vector::concepts::vector a_t> utils_gpu_available constexpr auto operator-(const a_t& a) noexcept { return a.for_each_to_new<[](const auto& value) { return -value; }>(); }
#pragma endregion self

#pragma region array
	template <utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_vector<a_t> b_t> utils_gpu_available constexpr a_t& operator+=(      a_t& a, const b_t& b) noexcept { return a.operator_self_assign<[](      auto& a, const auto& b) {        a += b; }>(b); }
	template <utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_vector<a_t> b_t> utils_gpu_available constexpr a_t& operator-=(      a_t& a, const b_t& b) noexcept { return a.operator_self_assign<[](      auto& a, const auto& b) {        a -= b; }>(b); }
	template <utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_vector<a_t> b_t> utils_gpu_available constexpr a_t& operator*=(      a_t& a, const b_t& b) noexcept { return a.operator_self_assign<[](      auto& a, const auto& b) {        a *= b; }>(b); }
	template <utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_vector<a_t> b_t> utils_gpu_available constexpr a_t& operator/=(      a_t& a, const b_t& b) noexcept { return a.operator_self_assign<[](      auto& a, const auto& b) {        a /= b; }>(b); }
	template <utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_vector<a_t> b_t> utils_gpu_available constexpr a_t& operator%=(      a_t& a, const b_t& b) noexcept { return a.operator_self_assign<[](      auto& a, const auto& b) {        a %= b; }>(b); }
	template <utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_vector<a_t> b_t> utils_gpu_available constexpr a_t& operator|=(      a_t& a, const b_t& b) noexcept { return a.operator_self_assign<[](      auto& a, const auto& b) {        a |= b; }>(b); }
	template <utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_vector<a_t> b_t> utils_gpu_available constexpr a_t& operator&=(      a_t& a, const b_t& b) noexcept { return a.operator_self_assign<[](      auto& a, const auto& b) {        a &= b; }>(b); }
	template <utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_vector<a_t> b_t> utils_gpu_available constexpr auto operator+ (const a_t& a, const b_t& b) noexcept { return a.operator_to_new     <[](const auto& a, const auto& b) { return a +  b; }>(b); }
	template <utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_vector<a_t> b_t> utils_gpu_available constexpr auto operator- (const a_t& a, const b_t& b) noexcept { return a.operator_to_new     <[](const auto& a, const auto& b) { return a -  b; }>(b); }
	template <utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_vector<a_t> b_t> utils_gpu_available constexpr auto operator* (const a_t& a, const b_t& b) noexcept { return a.operator_to_new     <[](const auto& a, const auto& b) { return a *  b; }>(b); }
	template <utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_vector<a_t> b_t> utils_gpu_available constexpr auto operator/ (const a_t& a, const b_t& b) noexcept { return a.operator_to_new     <[](const auto& a, const auto& b) { return a /  b; }>(b); }
	template <utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_vector<a_t> b_t> utils_gpu_available constexpr auto operator% (const a_t& a, const b_t& b) noexcept { return a.operator_to_new     <[](const auto& a, const auto& b) { return a %  b; }>(b); }
	template <utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_vector<a_t> b_t> utils_gpu_available constexpr auto operator| (const a_t& a, const b_t& b) noexcept { return a.operator_to_new     <[](const auto& a, const auto& b) { return a |  b; }>(b); }
	template <utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_vector<a_t> b_t> utils_gpu_available constexpr auto operator& (const a_t& a, const b_t& b) noexcept { return a.operator_to_new     <[](const auto& a, const auto& b) { return a &  b; }>(b); }
	template <utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_vector<a_t> b_t> utils_gpu_available constexpr bool operator!=(const a_t& a, const b_t& b) noexcept { return !(a == b); }
	template <utils::details::vector::concepts::vector a_t, utils::details::vector::concepts::compatible_vector<a_t> b_t> utils_gpu_available constexpr bool operator==(const a_t& a, const b_t& b) noexcept
		{
		size_t i{0};
		for (; i < std::min(a_t::extent, b_t::extent); i++)
			{
			if (a[i] != b[i]) { return false; }
			}

		if constexpr (a_t::extent > b_t::extent) { for (; i < a_t::extent; i++) { if (a[i] != typename a_t::value_type{}) { return false; } } }
		else
		if constexpr (a_t::extent < b_t::extent) { for (; i < b_t::extent; i++) { if (b[i] != typename b_t::value_type{}) { return false; } } }

		return true;
		}
	//template <utils::details::vector::concepts::compatible_array<derived_t> T2> constexpr derived_t& operator =(const T2& b) noexcept { for (size_t i{0}; i < std::min(crtp::derived().size(), b.size()); i++) { crtp::derived()[i] = b[i]; } return *this; }
#pragma endregion array

namespace utils::math
	{
	template <::utils::details::vector::concepts::vector T>
	utils_gpu_available auto abs(const T& v) noexcept { return v.for_each_to_new<[](const auto& value) { return utils::math::abs(value); }>(); }

	template <::utils::details::vector::concepts::vector T>
	utils_gpu_available auto floor(const T& v) noexcept { return v.for_each_to_new<[](const auto& value) { return utils::math::floor(value); }>(); }

	template <::utils::details::vector::concepts::vector T>
	utils_gpu_available auto ceil(const T& v) noexcept { return v.for_each_to_new<[](const auto& value) { return utils::math::ceil(value); }>(); }

	template <::utils::details::vector::concepts::vector T>
	utils_gpu_available auto pow(const T& a, const utils::details::vector::concepts::compatible_scalar<T> auto& b)
		{
		return a.operator_to_new<[](const auto& a, const auto& b) { return std::pow(a, b); }>(b);
		}

	template <::utils::details::vector::concepts::vector T>
	utils_gpu_available inline T pow(const T& a, const utils::details::vector::concepts::compatible_vector<T> auto& b)
		{
		return a.operator_to_new<[](const auto& a, const auto& b) { return std::pow(a, b); }>(b);
		}

	template <::utils::details::vector::concepts::vector T>
	utils_gpu_available inline typename T::nonref_self_t lerp(const T& a, const T& b, float t)
		{
		typename T::nonref_self_t ret;
		for (size_t i = 0; i < T::extent; i++)
			{
			ret[i] = utils::math::lerp(a[i], b[i], t);
			}
		return ret;
		}

	template <::utils::details::vector::concepts::vector T>
	utils_gpu_available inline typename T::nonref_self_t clamp(const T& in, const T& min, const T& max)
		{
		typename T::nonref_self_t ret;
		for (size_t i = 0; i < T::extent; i++)
			{
			ret[i] = utils::math::clamp(in[i], min[i], max[i]);
			}
		return ret;
		}

	template <utils::details::vector::concepts::vector T>
	utils_gpu_available inline auto min(const T& a, const T& b)
		{
		return a.operator_to_new<[](const auto& a, const auto& b) { return utils::math::min(a, b); }>(b);
		}

	template <utils::details::vector::concepts::vector T>
	utils_gpu_available inline T max(const T& a, const T& b)
		{
		return a.operator_to_new<[](const auto& a, const auto& b) { return utils::math::max(a, b); }>(b);
		}
	
	template <::utils::details::vector::concepts::vector T>
	utils_gpu_available inline typename T::nonref_self_t clamp(const T& in, const utils::details::vector::concepts::compatible_scalar<T> auto& min, const utils::details::vector::concepts::compatible_scalar<T> auto& max)
		{
		typename T::nonref_self_t ret;
		for (size_t i = 0; i < T::extent; i++)
			{
			ret[i] = utils::math::clamp(in[i], min, max);
			}
		return ret;
		}

	template <utils::details::vector::concepts::vector T>
	utils_gpu_available inline typename T::nonref_self_t min(const T& a, const utils::details::vector::concepts::compatible_scalar<T> auto& b)
		{
		return a.operator_to_new<[](const auto& a, const auto& b) { return utils::math::min(a, b); }>(b);
		}

	template <utils::details::vector::concepts::vector T>
	utils_gpu_available inline typename T::nonref_self_t max(const T& a, const utils::details::vector::concepts::compatible_scalar<T> auto& b)
		{
		return a.operator_to_new<[](const auto& a, const auto& b) { return utils::math::max(a, b); }>(b);
		}
	}