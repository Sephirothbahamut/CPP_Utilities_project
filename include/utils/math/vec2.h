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

	using vec2s   = vec2<size_t>;
	using vec2f   = vec2<float>;
	using vec2d   = vec2<double>;

	namespace details
		{
		template<class leaf_t, class T>
		class vec_impl<leaf_t, T, 2>
			{
			public:
				vec_impl() = default; //TODO we should be able to remove this?
				vec_impl(T x, T y)                            noexcept { auto& self = static_cast<leaf_t&>(*this); self.set_x(x); self.set_y(y); }; //TODO we should be able to remove this?
				vec_impl(math::angle::deg angle, T magnitude) noexcept { auto& self = static_cast<leaf_t&>(*this); self.set_x(angle.cos() * magnitude); self.set_y(angle.sin() * magnitude); } //TODO test
				vec_impl(math::angle::rad angle, T magnitude) noexcept { auto& self = static_cast<leaf_t&>(*this); self.set_x(angle.cos() * magnitude); self.set_y(angle.sin() * magnitude); } //TODO test
				//template <float full_angle_value>
				//vec(math::angle::base<full_angle_value> angle) noexcept requires(size == 2) : x{angle.cos()}, y{angle.sin()} {} //TODO test
				//template <typename other_t>
				//vec2(vec2<other_t> other)           noexcept : x{ static_cast<T>(other.x) }, y{ static_cast<T>(other.y) } {} //TODO test
				//vec2(const vec2<T>& other) noexcept : x{other.x}, y{other.y} {} //TODO test

				template <typename T = float, T full_angle_value = 360.f>
				math::angle::base<T, full_angle_value> angle() const noexcept { const leaf_t& self = static_cast<const leaf_t&>(*this); return math::angle::base<T, full_angle_value>::atan2(self.y, self.x); }
				
				// VEC & ANGLE OPERATIONS
				template <typename T, T full_angle_value> leaf_t  operator+ (const utils::math::angle::base<T, full_angle_value> angle) const noexcept { const auto& self = static_cast<const leaf_t&>(*this); return {self.x * angle.cos() - self.y * angle.sin(), self.x * angle.sin() + self.y * angle.cos()}; }
				template <typename T, T full_angle_value> leaf_t  operator- (const utils::math::angle::base<T, full_angle_value> angle) const noexcept { const auto& self = static_cast<const leaf_t&>(*this); return {self.x * angle.cos() - self.y * angle.sin(), self.x * angle.sin() + self.y * angle.cos()}; }
				template <typename T, T full_angle_value> leaf_t& operator+=(const utils::math::angle::base<T, full_angle_value> angle)       noexcept {       auto& self = static_cast<      leaf_t&>(*this); return self = self + angle; }
				template <typename T, T full_angle_value> leaf_t& operator-=(const utils::math::angle::base<T, full_angle_value> angle)       noexcept {       auto& self = static_cast<      leaf_t&>(*this); return self = self - angle; }
				template <typename T, T full_angle_value> leaf_t& operator= (const utils::math::angle::base<T, full_angle_value> angle)       noexcept {       auto& self = static_cast<      leaf_t&>(*this); return self = {angle.cos() * self.magnitude(), angle.sin() * self.magnitude()}; }

				// OTHER
				leaf_t perpendicular_right()            const noexcept { const auto& self = static_cast<const leaf_t&>(*this); return { self.y, -self.x}; }
				leaf_t perpendicular_left()             const noexcept { const auto& self = static_cast<const leaf_t&>(*this); return {-self.y,  self.x}; }
				leaf_t perpendicular_clockwise()        const noexcept { return perpendicular_right(); }
				leaf_t perpendicular_counterclockwise() const noexcept { return perpendicular_left (); }
			};
		}
	}