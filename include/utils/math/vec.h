#pragma once

#include <array>
#include <cmath>
#include <functional> //reference_wrapper

#include "math.h"
#include "../memory.h"
#include "../details/vec/all.h"
#include "../oop/disable_move_copy.h"

namespace utils::math
	{
	template <typename T, size_t size>
	class vec;
	
	//fast typenames
	template <typename T, size_t size> 
	using vecref = vec<std::reference_wrapper<T>, size>;
	
	template <size_t size> using vec_i      = vec   <int          , size>;
	template <size_t size> using vec_i8     = vec   <int8_t       , size>;
	template <size_t size> using vec_i16    = vec   <int16_t      , size>;
	template <size_t size> using vec_i32    = vec   <int32_t      , size>;
	template <size_t size> using vec_i64    = vec   <int64_t      , size>;
	template <size_t size> using vec_u      = vec   <unsigned     , size>;
	template <size_t size> using vec_u8     = vec   <uint8_t      , size>;
	template <size_t size> using vec_u16    = vec   <uint16_t     , size>;
	template <size_t size> using vec_u32    = vec   <uint32_t     , size>;
	template <size_t size> using vec_u64    = vec   <uint64_t     , size>;
	template <size_t size> using vec_s      = vec   <size_t       , size>;
	template <size_t size> using vec_f      = vec   <float        , size>;
	template <size_t size> using vec_d      = vec   <double       , size>;
	template <size_t size> using vec_l      = vec   <long         , size>;
	template <size_t size> using vec_ul     = vec   <unsigned long, size>;
	template <size_t size> using vecref_i   = vecref<int          , size>;
	template <size_t size> using vecref_i8  = vecref<int8_t       , size>;
	template <size_t size> using vecref_i16 = vecref<int16_t      , size>;
	template <size_t size> using vecref_i32 = vecref<int32_t      , size>;
	template <size_t size> using vecref_i64 = vecref<int64_t      , size>;
	template <size_t size> using vecref_u   = vecref<unsigned     , size>;
	template <size_t size> using vecref_u8  = vecref<uint8_t      , size>;
	template <size_t size> using vecref_u16 = vecref<uint16_t     , size>;
	template <size_t size> using vecref_u32 = vecref<uint32_t     , size>;
	template <size_t size> using vecref_u64 = vecref<uint64_t     , size>;
	template <size_t size> using vecref_s   = vecref<size_t       , size>;
	template <size_t size> using vecref_f   = vecref<float        , size>;
	template <size_t size> using vecref_d   = vecref<double       , size>;
	template <size_t size> using vecref_l   = vecref<long         , size>;
	template <size_t size> using vecref_ul  = vecref<unsigned long, size>;
	
	namespace concepts
		{
		template <typename T>
		concept vec = std::derived_from<T, utils::math::vec<typename T::value_type, T::static_size>>;
		}

	namespace details
		{
		inline extern constexpr const char name_vec[]{"vec"};

		template<class T, size_t size, typename derived_T>
		class vec_sized_specialization {};

		struct pair_sizes_t
			{
			size_t a{0};
			size_t b{0};
			size_t min{0};
			};

		template <typename a_T, typename b_T>
		consteval pair_sizes_t pair_sizes(const a_T& a, const b_T& b) noexcept
			{
			return
				{
				.a{a_T::static_size},
				.b{b_T::static_size},
				.min{utils::math::min(a_T::static_size, b_T::static_size)}
				};
			}

		template <typename a_T, typename b_T>
		consteval pair_sizes_t pair_sizes() noexcept
			{
			return
				{
				.a{a_T::static_size},
				.b{b_T::static_size},
				.min{utils::math::min(a_T::static_size, b_T::static_size)}
				};
			}
		}

	template<typename T, size_t size>
	struct utils_oop_empty_bases vec : ::utils::details::vector::base<T, size, vec, details::name_vec>, details::vec_sized_specialization<T, size, vec<T, size>>
		{
		template<class T, size_t size, typename derived_T>
		friend class details::vec_sized_specialization;

		using base_t = ::utils::details::vector::base<T, size, vec, details::name_vec>;
		using base_t::static_size; 
		using base_t::static_value_is_reference;
		using typename base_t::self_t;
		using typename base_t::value_type;
		using typename base_t::nonref_value_type;
		using typename base_t::nonref_self_t;

		using base_t::base;

		utils_gpu_available constexpr const nonref_value_type& x() const noexcept requires(static_size >= 1) { return (*this)[0]; }
		utils_gpu_available constexpr       nonref_value_type& x()       noexcept requires(static_size >= 1) { return (*this)[0]; }
		utils_gpu_available constexpr const nonref_value_type& y() const noexcept requires(static_size >= 2) { return (*this)[1]; }
		utils_gpu_available constexpr       nonref_value_type& y()       noexcept requires(static_size >= 2) { return (*this)[1]; }
		utils_gpu_available constexpr const nonref_value_type& z() const noexcept requires(static_size >= 3) { return (*this)[2]; }
		utils_gpu_available constexpr       nonref_value_type& z()       noexcept requires(static_size >= 3) { return (*this)[2]; }
		utils_gpu_available constexpr const nonref_value_type& w() const noexcept requires(static_size >= 4) { return (*this)[3]; }
		utils_gpu_available constexpr       nonref_value_type& w()       noexcept requires(static_size >= 4) { return (*this)[3]; }

#pragma region distances
		utils_gpu_available constexpr nonref_value_type get_length2() const noexcept { nonref_value_type ret{0}; base_t::for_each([&ret](const auto& value) { ret += value; }); return ret; }
		utils_gpu_available constexpr nonref_value_type get_length () const noexcept { return std::sqrt(get_length2()); }

		utils_gpu_available constexpr self_t& set_length(nonref_value_type value) noexcept { *this = normalize() * value; return *this; }

		__declspec(property(get = get_length, put = set_length)) nonref_value_type length;

		utils_gpu_available constexpr nonref_self_t normalize() const noexcept { return get_length() ? *this / get_length() : *this; }
		utils_gpu_available constexpr self_t& normalize_self() noexcept { return *this = normalize(); }

		/// <summary> Evaluate distance^2 in the size of this vec. Missing coordinates are considered 0. </summary>
		utils_gpu_available static constexpr value_type distance2(const self_t& a, const utils::details::vector::concepts::compatible_vector<self_t> auto& b) noexcept
			{
			constexpr auto sizes{details::pair_sizes<self_t, decltype(b)>()};

			nonref_value_type ret{0};
			size_t i{0};
			for (; i < sizes.min; i++)
				{
				nonref_value_type tmp{a[i] - b[i]};
				ret += tmp * tmp;
				}
					
			     if constexpr (sizes.a > sizes.b) { for (; i < sizes.a; i++) { ret += a[i] * a[i]; } }
			else if constexpr (sizes.a < sizes.b) { for (; i < sizes.b; i++) { ret += b[i] * b[i]; } }

			return ret;
			}

		/// <summary> Evaluate distance^2 in all the axes of the smaller vec. </summary>
		utils_gpu_available static constexpr value_type distance2_shared(const self_t& a, const utils::details::vector::concepts::compatible_vector<self_t> auto& b) noexcept
			{
			constexpr auto sizes{details::pair_sizes<self_t, decltype(b)>()};

			nonref_value_type ret{0};
			size_t i{0};
			for (; i < sizes.min; i++)
				{
				nonref_value_type tmp{a[i] - b[i]};
				ret += tmp * tmp;
				}

			return ret;
			}

		/// <summary> Evaluate distance in the size of this vec. Missing coordinates are considered 0. </summary>
		utils_gpu_available static constexpr value_type distance(const self_t& a, const utils::details::vector::concepts::compatible_vector<self_t> auto& b) noexcept
			{
			return std::sqrt(distance2(a, b)); 
			}

		/// <summary> Evaluate distance in all the axes of the smaller vec. </summary>
		utils_gpu_available static constexpr value_type distance_shared(const self_t& a, const utils::details::vector::concepts::compatible_vector<self_t> auto& b) noexcept
			{
			return std::sqrt(distance_shared2(a, b)); 
			}

#pragma endregion distances
#pragma region interpolation

		utils_gpu_available static constexpr nonref_self_t slerp_fast(const self_t& a, const self_t& b, value_type t) noexcept
			{
			return utils::math::lerp(a, b, t).normalize() * (utils::math::lerp(a.get_length(), b.get_length(), t));
			}
		utils_gpu_available static constexpr nonref_self_t tlerp_fast(const self_t& a, const self_t& b, value_type t) noexcept
			{
			return utils::math::lerp(a, b, t).normalize() * std::sqrt(utils::math::lerp(a.get_length2(), b.get_length2(), t));
			}
		utils_gpu_available static constexpr nonref_self_t slerp(const self_t& a, const self_t& b, value_type t) noexcept //TODO test
			{
			nonref_value_type dot = utils::math::clamp(vec::dot(a, b), -1.0f, 1.0f);
			nonref_value_type theta = std::acos(dot) * t;
			nonref_self_t relative_vec = (b - a * dot).normalize();
			return ((a * std::cos(theta)) + (relative_vec * std::sin(theta)));
			}

#pragma endregion interpolation
		struct create : ::utils::oop::non_constructible
			{
			utils_gpu_available static constexpr self_t zero    () noexcept requires(!static_value_is_reference && static_size >= 1) { return {value_type{ 0}}; }

			utils_gpu_available static constexpr self_t rr      () noexcept requires(!static_value_is_reference && static_size == 1) { return {value_type{ 1}}; }
			utils_gpu_available static constexpr self_t ll      () noexcept requires(!static_value_is_reference && static_size == 1) { return {value_type{-1}}; }
			utils_gpu_available static constexpr self_t rr      () noexcept requires(!static_value_is_reference && static_size >  1) { return {value_type{ 1}, value_type{ 0}}; }
			utils_gpu_available static constexpr self_t ll      () noexcept requires(!static_value_is_reference && static_size >  1) { return {value_type{-1}, value_type{ 0}}; }
			utils_gpu_available static constexpr self_t right   () noexcept requires(!static_value_is_reference && static_size >= 1) { return rr(); }
			utils_gpu_available static constexpr self_t left    () noexcept requires(!static_value_is_reference && static_size >= 1) { return ll(); }

			utils_gpu_available static constexpr self_t up      () noexcept requires(!static_value_is_reference && static_size == 2) { return {value_type{ 0}, value_type{-1}}; }
			utils_gpu_available static constexpr self_t dw      () noexcept requires(!static_value_is_reference && static_size == 2) { return {value_type{ 0}, value_type{ 1}}; }
			utils_gpu_available static constexpr self_t up      () noexcept requires(!static_value_is_reference && static_size >  2) { return {value_type{ 0}, value_type{-1}, value_type{ 0}}; }
			utils_gpu_available static constexpr self_t dw      () noexcept requires(!static_value_is_reference && static_size >  2) { return {value_type{ 0}, value_type{ 1}, value_type{ 0}}; }
			utils_gpu_available static constexpr self_t down    () noexcept requires(!static_value_is_reference && static_size >= 2) { return dw(); }

			utils_gpu_available static constexpr self_t fw      () noexcept requires(!static_value_is_reference && static_size == 3) { return {value_type{ 0}, value_type{ 0}, value_type{ 1}}; }
			utils_gpu_available static constexpr self_t bw      () noexcept requires(!static_value_is_reference && static_size == 3) { return {value_type{ 0}, value_type{ 0}, value_type{-1}}; }
			utils_gpu_available static constexpr self_t fw      () noexcept requires(!static_value_is_reference && static_size >  3) { return {value_type{ 0}, value_type{ 0}, value_type{ 1}, value_type{ 0}}; }
			utils_gpu_available static constexpr self_t bw      () noexcept requires(!static_value_is_reference && static_size >  3) { return {value_type{ 0}, value_type{ 0}, value_type{-1}, value_type{ 0}}; }
			utils_gpu_available static constexpr self_t forward () noexcept requires(!static_value_is_reference && static_size >= 3) { return fw(); }
			utils_gpu_available static constexpr self_t backward() noexcept requires(!static_value_is_reference && static_size >= 3) { return bw(); }
			};
				
		template <concepts::vec b_t>
		utils_gpu_available static constexpr nonref_value_type dot(const self_t& a, const b_t& b) noexcept
			requires std::convertible_to<value_type, typename b_t::value_type>
			{
			nonref_value_type ret{0}; 
			for (size_t i{0}; i < static_size; i++)
				{
				ret += a[i] * b[i];
				} 
			return ret;
			}
		};
	

	namespace operators
		{
		inline constexpr struct _dot
			{
			template <concepts::vec a_t>
			class _inner;

			template <concepts::vec a_t>
			utils_gpu_available inline friend _inner<a_t> operator<(const a_t& lhs, _dot proxy) noexcept { return {lhs}; }

			template <concepts::vec a_t>
			class _inner
				{
				public:
					template <concepts::vec b_t>
					typename a_t::nonref_value_type operator>(const b_t& rhs) const noexcept  { return a_t::dot(lhs, rhs); }
					utils_gpu_available _inner(const a_t& lhs) noexcept : lhs{lhs} {}
				private:
					const a_t& lhs;
				};

			} dot;

		inline constexpr struct _cross
			{
			template <concepts::vec a_t>
			class _inner;

			template <concepts::vec a_t>
			utils_gpu_available inline friend _inner<a_t> operator<(const a_t& lhs, _cross proxy) noexcept { return {lhs}; }

			template <concepts::vec a_t>
			class _inner
				{
				public:
					template <concepts::vec b_t>
					a_t::nonref_derived_t operator>(const b_t& rhs) const noexcept { return lhs * rhs; }
					utils_gpu_available _inner(const a_t& lhs) noexcept : lhs{lhs} {}
				private:
					const a_t& lhs;
				};

			} cross;
		}
	}