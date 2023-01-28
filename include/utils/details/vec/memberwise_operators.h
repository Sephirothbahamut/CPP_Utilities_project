#pragma once

#include <concepts>

#include "common.h"

namespace utils::details::vec
	{
	template<typename T, typename DERIVED_T>
	class memberwise_operators;

	namespace concepts
		{
		template <typename T>
		concept memberwise_operators = std::derived_from<T, vec::memberwise_operators<T, typename T::value_type>>;
		}

	template<typename T, typename DERIVED_T>
	class memberwise_operators
		{
		public:
			using derived_t = DERIVED_T;
			using value_type = T;

		private:
			constexpr const derived_t& derived() const noexcept { return static_cast<const derived_t&>(*this); }
			constexpr       derived_t& derived()       noexcept { return static_cast<      derived_t&>(*this); }

		public:
#pragma region scalar
			constexpr derived_t& operator+=(const value_type& b)       noexcept { for (size_t i{0}; i < derived().size(); i++) { derived()[i] += b; } return derived(); }
			constexpr derived_t& operator-=(const value_type& b)       noexcept { for (size_t i{0}; i < derived().size(); i++) { derived()[i] -= b; } return derived(); }
			constexpr derived_t& operator*=(const value_type& b)       noexcept { for (size_t i{0}; i < derived().size(); i++) { derived()[i] *= b; } return derived(); }
			constexpr derived_t& operator/=(const value_type& b)       noexcept { for (size_t i{0}; i < derived().size(); i++) { derived()[i] /= b; } return derived(); }
			constexpr derived_t& operator|=(const value_type& b)       noexcept { for (size_t i{0}; i < derived().size(); i++) { derived()[i] |= b; } return derived(); }
			constexpr derived_t& operator&=(const value_type& b)       noexcept { for (size_t i{0}; i < derived().size(); i++) { derived()[i] &= b; } return derived(); }
			constexpr derived_t& operator =(const value_type& b)       noexcept { for (size_t i{0}; i < derived().size(); i++) { derived()[i]  = b; } return derived(); }

			constexpr derived_t  operator+ (const value_type& b) const noexcept { auto ret{derived()}; ret += b; return ret; }
			constexpr derived_t  operator- (const value_type& b) const noexcept { auto ret{derived()}; ret -= b; return ret; }
			constexpr derived_t  operator* (const value_type& b) const noexcept { auto ret{derived()}; ret *= b; return ret; }
			constexpr derived_t  operator/ (const value_type& b) const noexcept { auto ret{derived()}; ret /= b; return ret; }
			constexpr derived_t  operator| (const value_type& b) const noexcept { auto ret{derived()}; ret |= b; return ret; }
			constexpr derived_t  operator& (const value_type& b) const noexcept { auto ret{derived()}; ret &= b; return ret; }
#pragma endregion scalar
#pragma region array
			template <concepts::compatible_array<derived_t> T2> constexpr derived_t& operator =(const T2& b) noexcept { for (size_t i{0}; i < std::min(derived().size(), b.size()); i++) { derived()[i] = b[i]; } return *this; }
#pragma endregion array
#pragma region self
			constexpr derived_t  operator! () const noexcept { auto ret{derived()}; for (size_t i{0}; i < derived().size(); i++) { ret[i] = !ret[i]; } return ret; }
			constexpr derived_t  operator- () const noexcept { auto ret{derived()}; for (size_t i{0}; i < derived().size(); i++) { ret[i] = -ret[i]; } return ret; }
#pragma endregion self

#pragma region array
			template <concepts::compatible_array<DERIVED_T> T2> constexpr auto& operator+=(const T2& b)       noexcept { for (size_t i{0}; i < std::min(derived().size(), b.size()); i++) { derived()[i] += b[i]; } return derived(); }
			template <concepts::compatible_array<DERIVED_T> T2> constexpr auto& operator-=(const T2& b)       noexcept { for (size_t i{0}; i < std::min(derived().size(), b.size()); i++) { derived()[i] -= b[i]; } return derived(); }
			template <concepts::compatible_array<DERIVED_T> T2> constexpr auto& operator*=(const T2& b)       noexcept { for (size_t i{0}; i < std::min(derived().size(), b.size()); i++) { derived()[i] *= b[i]; } return derived(); }
			template <concepts::compatible_array<DERIVED_T> T2> constexpr auto& operator/=(const T2& b)       noexcept { for (size_t i{0}; i < std::min(derived().size(), b.size()); i++) { derived()[i] /= b[i]; } return derived(); }
			template <concepts::compatible_array<DERIVED_T> T2> constexpr auto& operator|=(const T2& b)       noexcept { for (size_t i{0}; i < std::min(derived().size(), b.size()); i++) { derived()[i] |= b[i]; } return derived(); }
			template <concepts::compatible_array<DERIVED_T> T2> constexpr auto& operator&=(const T2& b)       noexcept { for (size_t i{0}; i < std::min(derived().size(), b.size()); i++) { derived()[i] &= b[i]; } return derived(); }
			template <concepts::compatible_array<DERIVED_T> T2> constexpr auto  operator+ (const T2& b) const noexcept { auto ret{derived()}; ret += b; return ret; }
			template <concepts::compatible_array<DERIVED_T> T2> constexpr auto  operator- (const T2& b) const noexcept { auto ret{derived()}; ret -= b; return ret; }
			template <concepts::compatible_array<DERIVED_T> T2> constexpr auto  operator* (const T2& b) const noexcept { auto ret{derived()}; ret *= b; return ret; }
			template <concepts::compatible_array<DERIVED_T> T2> constexpr auto  operator/ (const T2& b) const noexcept { auto ret{derived()}; ret /= b; return ret; }
			template <concepts::compatible_array<DERIVED_T> T2> constexpr auto  operator| (const T2& b) const noexcept { auto ret{derived()}; ret |= b; return ret; }
			template <concepts::compatible_array<DERIVED_T> T2> constexpr auto  operator& (const T2& b) const noexcept { auto ret{derived()}; ret &= b; return ret; }
			template <concepts::compatible_array<DERIVED_T> T2> constexpr bool  operator!=(const T2& b) const noexcept { return !(derived() == b); }
			template <concepts::compatible_array<DERIVED_T> T2> constexpr bool  operator==(const T2& b) const noexcept
				{
				size_t i{0};
				for (; i < std::min(derived().size(), b.size()); i++)
					{
					if (derived()[i] != b[i]) { return false; }
					}

					 if constexpr (derived().size() > b.size()) { for (; i < derived().size(); i++) { if (derived()[i] != T{}) { return false; } } }
				else if constexpr (derived().size() < b.size()) { for (; i < b        .size(); i++) { if (b      ()[i] != T{}) { return false; } } }

				return true;
				}			
#pragma endregion array
		};
	}

namespace utils::math
	{
	template <utils::details::vec::concepts::memberwise_operators a_t>
	a_t abs(const a_t& v) noexcept { a_t ret; for (size_t i{0}; i < a_t::static_size; i++) { ret[i] = std::abs(v[i]); } return ret; }

	template <utils::details::vec::concepts::memberwise_operators a_t>
	inline a_t lerp(const a_t& a, const a_t& b, float t)
		{
		a_t ret;
		for (size_t i{0}; i < a_t::static_size; i++) { ret[i] = utils::math::lerp(a[i], b[i], t); }
		return ret;
		}

	template <utils::details::vec::concepts::memberwise_operators a_t>
	inline a_t clamp(const a_t& in, const a_t& min, const a_t& max)
		{
		a_t ret;
		for (size_t i = 0; i < a_t::static_size; i++)
			{
			ret[i] = utils::math::clamp(in[i], min[i], max[i]);
			}
		return ret;
		}

	template <utils::details::vec::concepts::memberwise_operators a_t, utils::details::vec::concepts::compatible_array<a_t> b_t>
	inline a_t min(const a_t& a, const b_t& b)
		{
		auto ret{a};
		for (size_t i{0}; i < std::min(a.size(), b.size()); i++) 
			{
			ret[i] = std::min(a[i], b[i]);
			} 

		return ret;
		}

	template <utils::details::vec::concepts::memberwise_operators a_t, utils::details::vec::concepts::compatible_array<a_t> b_t>
	inline a_t max(const a_t& a, const b_t& b)
		{
		auto ret{a};
		for (size_t i{0}; i < std::min(a.size(), b.size()); i++)
			{
			ret[i] = std::max(a[i], b[i]);
			}

		return ret;
		}
	}