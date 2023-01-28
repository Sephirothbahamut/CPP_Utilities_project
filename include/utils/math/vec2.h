#pragma once

#include "vec.h"
#include "angle.h"

namespace utils::math
	{
	template <typename T> 
	using vec2 = vec<T, 2>;

	//fast typenames
	using vec2i   = vec2<int>;
	using vec2i8  = vec2<int8_t>;
	using vec2i16 = vec2<int16_t>;
	using vec2i32 = vec2<int32_t>;
	using vec2i64 = vec2<int64_t>;

	using vec2u   = vec2<unsigned>;
	using vec2u8  = vec2<uint8_t>;
	using vec2u16 = vec2<uint16_t>;
	using vec2u32 = vec2<uint32_t>;
	using vec2u64 = vec2<uint64_t>;

	using vec2u   = vec2<unsigned>;
	using vec2u8  = vec2<uint8_t>;
	using vec2u16 = vec2<uint16_t>;
	using vec2u32 = vec2<uint32_t>;
	using vec2u64 = vec2<uint64_t>;

	using vec2s   = vec2<size_t>;
	using vec2f   = vec2<float>;
	using vec2d   = vec2<double>;

	using vec2l   = vec2<long>;
	using vec2ul  = vec2<unsigned long>;

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
					return {.x{angle.cos() * magnitude}, .y{angle.sin() * magnitude}};
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