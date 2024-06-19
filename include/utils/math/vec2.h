#pragma once

#include "vec.h"
#include "angle.h"
#include "../oop/conditional_inheritance.h"
#include "../math/geometry/details/base_types.h"

namespace utils::math
	{
	struct transform2;
	template <typename T>
	struct rect;

	template <typename T> 
	using vec2 = vec<T, 2>;
	
	//fast typenames
	template <typename T> 
	using vecref2 = vecref<T, 2>;
	
	using vec2i      = vec2   <int          >;
	using vec2i8     = vec2   <int8_t       >;
	using vec2i16    = vec2   <int16_t      >;
	using vec2i32    = vec2   <int32_t      >;
	using vec2i64    = vec2   <int64_t      >;
	using vec2u      = vec2   <unsigned     >;
	using vec2u8     = vec2   <uint8_t      >;
	using vec2u16    = vec2   <uint16_t     >;
	using vec2u32    = vec2   <uint32_t     >;
	using vec2u64    = vec2   <uint64_t     >;
	using vec2s      = vec2   <size_t       >;
	using vec2f      = vec2   <float        >;
	using vec2d      = vec2   <double       >;
	using vec2l      = vec2   <long         >;
	using vec2ul     = vec2   <unsigned long>;
	using vecref2i   = vecref2<int          >;
	using vecref2i8  = vecref2<int8_t       >;
	using vecref2i16 = vecref2<int16_t      >;
	using vecref2i32 = vecref2<int32_t      >;
	using vecref2i64 = vecref2<int64_t      >;
	using vecref2u   = vecref2<unsigned     >;
	using vecref2u8  = vecref2<uint8_t      >;
	using vecref2u16 = vecref2<uint16_t     >;
	using vecref2u32 = vecref2<uint32_t     >;
	using vecref2u64 = vecref2<uint64_t     >;
	using vecref2s   = vecref2<size_t       >;
	using vecref2f   = vecref2<float        >;
	using vecref2d   = vecref2<double       >;
	using vecref2l   = vecref2<long         >;
	using vecref2ul  = vecref2<unsigned long>;

	namespace details
		{
		template<typename T, template <typename, size_t> class unspecialized_derived_T>
		class utils_oop_empty_bases vec_sized_specialization<T, 2, unspecialized_derived_T> : utils::math::geometry::shape_flag
			{
			private:
				using self_t = unspecialized_derived_T<T, 2>;
				utils_gpu_available constexpr const self_t& self() const noexcept { return static_cast<const self_t&>(*this); }
				utils_gpu_available constexpr       self_t& self()       noexcept { return static_cast<      self_t&>(*this); }

			public:
				using value_type = typename T;
				template <typename T, size_t size>
				using unspecialized_derived_t = unspecialized_derived_T<T, size>;
				using nonref_value_type = typename utils::remove_reference_t<value_type>;
				using nonref_self_t     = unspecialized_derived_t<nonref_value_type, 2>;

				utils_gpu_available  static constexpr self_t from(const math::angle::concepts::angle auto& angle, T magnitude = 1) noexcept
					{
					auto x{angle.cos() * magnitude};
					auto y{angle.sin() * magnitude};
					return self_t{x, y};
					}

				template <typename T = float, T f_a_v = 360.f>
				utils_gpu_available constexpr math::angle::base<T, f_a_v> angle() const noexcept { return math::angle::base<T, f_a_v>::atan2(self().y(), self().x()); }
				
				utils_gpu_available constexpr nonref_self_t operator+ (const math::angle::concepts::angle auto& angle) const noexcept
					{
					return
						{
						self().x() * angle.cos() - self().y() * angle.sin(),
						self().x() * angle.sin() + self().y() * angle.cos()
						};
					}
				utils_gpu_available constexpr nonref_self_t operator- (const math::angle::concepts::angle auto& angle) const noexcept
					{
					const auto nngle{-angle};
					return
						{
						self().x() * nngle.cos() - self().y() * nngle.sin(),
						self().x() * nngle.sin() + self().y() * nngle.cos()
						};
					}

				utils_gpu_available constexpr self_t& operator+=(const math::angle::concepts::angle auto& angle) noexcept { return self() = self() + angle; }
				utils_gpu_available constexpr self_t& operator-=(const math::angle::concepts::angle auto& angle) noexcept { return self() = self() - angle; }
				utils_gpu_available constexpr self_t& operator= (const math::angle::concepts::angle auto& angle) noexcept
					{
					return self() = {angle.cos() * self().magnitude(), angle.sin() * self().magnitude()}; 
					}

				utils_gpu_available constexpr nonref_self_t perpendicular_right           () const noexcept { return { self().y(), -self().x()}; }
				utils_gpu_available constexpr nonref_self_t perpendicular_left            () const noexcept { return {-self().y(),  self().x()}; }
				utils_gpu_available constexpr nonref_self_t perpendicular_clockwise       () const noexcept { return perpendicular_right(); }
				utils_gpu_available constexpr nonref_self_t perpendicular_counterclockwise() const noexcept { return perpendicular_left (); }
			};
		}
	}
