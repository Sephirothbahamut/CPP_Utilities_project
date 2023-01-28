#pragma once

#include "vec.h"
#include "angle.h"

namespace utils::math
	{
	template <typename T> 
	using vec2 = vec<T, 2>;
	
	//fast typenames
	template <typename T> 
	using vecref2 = vecref<T, 2>;
	
	using vec2i      = vec   <int          , 2>;
	using vec2i8     = vec   <int8_t       , 2>;
	using vec2i16    = vec   <int16_t      , 2>;
	using vec2i32    = vec   <int32_t      , 2>;
	using vec2i64    = vec   <int64_t      , 2>;
	using vec2u      = vec   <unsigned     , 2>;
	using vec2u8     = vec   <uint8_t      , 2>;
	using vec2u16    = vec   <uint16_t     , 2>;
	using vec2u32    = vec   <uint32_t     , 2>;
	using vec2u64    = vec   <uint64_t     , 2>;
	using vec2s      = vec   <size_t       , 2>;
	using vec2f      = vec   <float        , 2>;
	using vec2d      = vec   <double       , 2>;
	using vec2l      = vec   <long         , 2>;
	using vec2ul     = vec   <unsigned long, 2>;
	using vecref2i   = vecref<int          , 2>;
	using vecref2i8  = vecref<int8_t       , 2>;
	using vecref2i16 = vecref<int16_t      , 2>;
	using vecref2i32 = vecref<int32_t      , 2>;
	using vecref2i64 = vecref<int64_t      , 2>;
	using vecref2u   = vecref<unsigned     , 2>;
	using vecref2u8  = vecref<uint8_t      , 2>;
	using vecref2u16 = vecref<uint16_t     , 2>;
	using vecref2u32 = vecref<uint32_t     , 2>;
	using vecref2u64 = vecref<uint64_t     , 2>;
	using vecref2s   = vecref<size_t       , 2>;
	using vecref2f   = vecref<float        , 2>;
	using vecref2d   = vecref<double       , 2>;
	using vecref2l   = vecref<long         , 2>;
	using vecref2ul  = vecref<unsigned long, 2>;

	namespace details
		{
		template<class T, typename DERIVED_T>
		class vec_sized_specialization<T, 2, DERIVED_T>
			{
			public:
				using derived_t = DERIVED_T;

			private:
				constexpr const derived_t& derived() const noexcept { return static_cast<const derived_t&>(*this); }
				constexpr       derived_t& derived()       noexcept { return static_cast<derived_t&>(*this); }
				constexpr const auto& get_arr() const noexcept { return derived().array; }
				constexpr       auto& get_arr()       noexcept { return derived().array; }

				using arr_t = std::array<T, 2>;

			public:
				inline static constexpr const size_t static_size{std::tuple_size_v<arr_t>};
				using value_type = typename arr_t::value_type;

				template<std::floating_point T, T f_a_v>
				inline static constexpr derived_t from(const math::angle::base<T, f_a_v>& angle, T magnitude = 1) noexcept
					{
					auto x{angle.cos() * magnitude};
					auto y{angle.sin() * magnitude};
					return derived_t::from(x, y);
					}

				template <typename T = float, T f_a_v = 360.f>
				constexpr math::angle::base<T, f_a_v> angle() const noexcept { return math::angle::base<T, f_a_v>::atan2(derived().y, derived().x); }
				
				// VEC & ANGLE OPERATIONS
				template <std::floating_point T, T f_a_v> constexpr derived_t  operator+ (const math::angle::base<T, f_a_v>& angle) const noexcept { auto ret{derived()}; ret += angle; return ret; }
				template <std::floating_point T, T f_a_v> constexpr derived_t  operator- (const math::angle::base<T, f_a_v>& angle) const noexcept { auto ret{derived()}; ret -= angle; return ret; }
				template <std::floating_point T, T f_a_v> constexpr derived_t& operator+=(const math::angle::base<T, f_a_v>& angle)       noexcept 
					{
					derived().x = derived().x * angle.cos() - derived().y * angle.sin();
					derived().y = derived().x * angle.sin() + derived().y * angle.cos();
					return derived(); 
					}
				template <std::floating_point T, T f_a_v> constexpr derived_t& operator-=(const math::angle::base<T, f_a_v>& angle)       noexcept
					{
					auto nngle{-angle};
					derived().x = derived().x * nngle.cos() - derived().y * nngle.sin();
					derived().y = derived().x * nngle.sin() + derived().y * nngle.cos();
					return derived();
					}

				template <std::floating_point T, T f_a_v> constexpr derived_t& operator= (const math::angle::base<T, f_a_v>& angle)       noexcept
					{
					return derived() = {angle.cos() * derived().magnitude(), angle.sin() * derived().magnitude()}; 
					}

				// OTHER
				constexpr derived_t perpendicular_right           () const noexcept { return { derived().y, -derived().x}; }
				constexpr derived_t perpendicular_left            () const noexcept { return {-derived().y,  derived().x}; }
				constexpr derived_t perpendicular_clockwise       () const noexcept { return perpendicular_right(); }
				constexpr derived_t perpendicular_counterclockwise() const noexcept { return perpendicular_left (); }
			};
		}
	}