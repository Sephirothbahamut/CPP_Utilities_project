#pragma once

#include <concepts>

#include "common.h"
#include "../../memory.h"

namespace utils::details::vec
	{
			
	template<typename T, size_t SIZE, template<typename, size_t> class UNSPECIALIZED, typename DERIVED_T>
	class memberwise_operators;

	namespace concepts
		{
		//template <typename T>
		//concept memberwise_operators = std::derived_from<T, vec::memberwise_operators<typename T::value_type, T::static_size, T::unspecialized>>;
		}

	namespace details
		{
		template <typename T>
		struct get_nonref { using type = T; };

		template <utils::concepts::reference T>
		struct get_nonref<T> { using type = typename T::value_type; };
		}

	template<typename T, size_t SIZE, template<typename, size_t> class UNSPECIALIZED, typename DERIVED_T>
	class memberwise_operators
		{
		public:
			template <typename T, size_t size>
			using unspecialized_t = UNSPECIALIZED<T, size>;

			using derived_t = DERIVED_T;
			using value_type = T;
			using nonref_value_type = details::get_nonref<value_type>::type;
			using nonref_derived_t  = std::conditional_t<utils::concepts::reference<value_type>, unspecialized_t<nonref_value_type, SIZE>, derived_t>;
		private:
			constexpr const derived_t& derived() const noexcept { return static_cast<const derived_t&>(*this); }
			constexpr       derived_t& derived()       noexcept { return static_cast<      derived_t&>(*this); }

		public:
#pragma region scalar
			constexpr auto& operator+=(const value_type& b)       noexcept { for (size_t i{0}; i < derived().size(); i++) { derived()[i] += b; } return derived(); }
			constexpr auto& operator-=(const value_type& b)       noexcept { for (size_t i{0}; i < derived().size(); i++) { derived()[i] -= b; } return derived(); }
			constexpr auto& operator*=(const value_type& b)       noexcept { for (size_t i{0}; i < derived().size(); i++) { derived()[i] *= b; } return derived(); }
			constexpr auto& operator/=(const value_type& b)       noexcept { for (size_t i{0}; i < derived().size(); i++) { derived()[i] /= b; } return derived(); }
			constexpr auto& operator|=(const value_type& b)       noexcept { for (size_t i{0}; i < derived().size(); i++) { derived()[i] |= b; } return derived(); }
			constexpr auto& operator&=(const value_type& b)       noexcept { for (size_t i{0}; i < derived().size(); i++) { derived()[i] &= b; } return derived(); }
			constexpr auto& operator =(const value_type& b)       noexcept { for (size_t i{0}; i < derived().size(); i++) { derived()[i]  = b; } return derived(); }
			
			constexpr auto  operator+ (const value_type& b) const noexcept { nonref_derived_t ret{derived()}; ret += b; return ret; }
			constexpr auto  operator- (const value_type& b) const noexcept { nonref_derived_t ret{derived()}; ret -= b; return ret; }
			constexpr auto  operator* (const value_type& b) const noexcept { nonref_derived_t ret{derived()}; ret *= b; return ret; }
			constexpr auto  operator/ (const value_type& b) const noexcept { nonref_derived_t ret{derived()}; ret /= b; return ret; }
			constexpr auto  operator| (const value_type& b) const noexcept { nonref_derived_t ret{derived()}; ret |= b; return ret; }
			constexpr auto  operator& (const value_type& b) const noexcept { nonref_derived_t ret{derived()}; ret &= b; return ret; }
#pragma endregion scalar
#pragma region array
			template <concepts::compatible_array<derived_t> T2> constexpr derived_t& operator =(const T2& b) noexcept { for (size_t i{0}; i < std::min(derived().size(), b.size()); i++) { derived()[i] = b[i]; } return *this; }
#pragma endregion array
#pragma region self
			constexpr nonref_derived_t operator! () const noexcept { nonref_derived_t ret{derived()}; for (size_t i{0}; i < derived().size(); i++) { ret[i] = !ret[i]; } return ret; }
			constexpr nonref_derived_t operator- () const noexcept { nonref_derived_t ret{derived()}; for (size_t i{0}; i < derived().size(); i++) { ret[i] = -ret[i]; } return ret; }
#pragma endregion self

#pragma region array
			template <concepts::compatible_array<DERIVED_T> T2> constexpr auto& operator+=(const T2& b)       noexcept { for (size_t i{0}; i < std::min(derived().size(), b.size()); i++) { derived()[i] += b[i]; } return derived(); }
			template <concepts::compatible_array<DERIVED_T> T2> constexpr auto& operator-=(const T2& b)       noexcept { for (size_t i{0}; i < std::min(derived().size(), b.size()); i++) { derived()[i] -= b[i]; } return derived(); }
			template <concepts::compatible_array<DERIVED_T> T2> constexpr auto& operator*=(const T2& b)       noexcept { for (size_t i{0}; i < std::min(derived().size(), b.size()); i++) { derived()[i] *= b[i]; } return derived(); }
			template <concepts::compatible_array<DERIVED_T> T2> constexpr auto& operator/=(const T2& b)       noexcept { for (size_t i{0}; i < std::min(derived().size(), b.size()); i++) { derived()[i] /= b[i]; } return derived(); }
			template <concepts::compatible_array<DERIVED_T> T2> constexpr auto& operator|=(const T2& b)       noexcept { for (size_t i{0}; i < std::min(derived().size(), b.size()); i++) { derived()[i] |= b[i]; } return derived(); }
			template <concepts::compatible_array<DERIVED_T> T2> constexpr auto& operator&=(const T2& b)       noexcept { for (size_t i{0}; i < std::min(derived().size(), b.size()); i++) { derived()[i] &= b[i]; } return derived(); }
			template <concepts::compatible_array<DERIVED_T> T2> constexpr auto  operator+ (const T2& b) const noexcept { nonref_derived_t ret{derived()}; ret += b; return ret; }
			template <concepts::compatible_array<DERIVED_T> T2> constexpr auto  operator- (const T2& b) const noexcept { nonref_derived_t ret{derived()}; ret -= b; return ret; }
			template <concepts::compatible_array<DERIVED_T> T2> constexpr auto  operator* (const T2& b) const noexcept { nonref_derived_t ret{derived()}; ret *= b; return ret; }
			template <concepts::compatible_array<DERIVED_T> T2> constexpr auto  operator/ (const T2& b) const noexcept { nonref_derived_t ret{derived()}; ret /= b; return ret; }
			template <concepts::compatible_array<DERIVED_T> T2> constexpr auto  operator| (const T2& b) const noexcept { nonref_derived_t ret{derived()}; ret |= b; return ret; }
			template <concepts::compatible_array<DERIVED_T> T2> constexpr auto  operator& (const T2& b) const noexcept { nonref_derived_t ret{derived()}; ret &= b; return ret; }
			template <concepts::compatible_array<DERIVED_T> T2> constexpr bool  operator!=(const T2& b) const noexcept { return !(derived() == b); }
			template <concepts::compatible_array<DERIVED_T> T2> constexpr bool  operator==(const T2& b) const noexcept
				{
				size_t i{0};
				for (; i < std::min(derived().size(), b.size()); i++)
					{
					if (derived()[i] != b[i]) { return false; }
					}

					 if /*constexpr*/ (derived().size() > b.size()) { for (; i < derived().size(); i++) { if (derived()[i] != T{}) { return false; } } } //TODO check why no constexpr
				else if /*constexpr*/ (derived().size() < b.size()) { for (; i < b        .size(); i++) { if (b        [i] != T{}) { return false; } } } //TODO check why no constexpr

				return true;
				}			
#pragma endregion array
		};
	}

namespace utils::math
	{
	template <utils::details::vec::concepts::array a_t>
	a_t abs(const a_t& v) noexcept { a_t ret; for (size_t i{0}; i < a_t::static_size; i++) { ret[i] = std::abs(v[i]); } return ret; }

	template <utils::details::vec::concepts::array a_t>
	inline a_t lerp(const a_t& a, const a_t& b, float t)
		{
		a_t ret;
		for (size_t i{0}; i < a_t::static_size; i++) { ret[i] = utils::math::lerp(a[i], b[i], t); }
		return ret;
		}

	template <utils::details::vec::concepts::array a_t>
	inline a_t clamp(const a_t& in, const a_t& min, const a_t& max)
		{
		a_t ret;
		for (size_t i = 0; i < a_t::static_size; i++)
			{
			ret[i] = utils::math::clamp(in[i], min[i], max[i]);
			}
		return ret;
		}

	template <utils::details::vec::concepts::array a_t, utils::details::vec::concepts::compatible_array<a_t> b_t>
	inline a_t min(const a_t& a, const b_t& b)
		{
		auto ret{a};
		for (size_t i{0}; i < std::min(a.size(), b.size()); i++) 
			{
			ret[i] = std::min(a[i], b[i]);
			} 

		return ret;
		}

	template <utils::details::vec::concepts::array a_t, utils::details::vec::concepts::compatible_array<a_t> b_t>
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