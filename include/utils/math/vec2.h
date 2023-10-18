#pragma once

#include "vec.h"
#include "angle.h"
#include "geometry/common/root.h"

namespace utils::math
	{
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
		template<class T, typename DERIVED_T>
		class vec_sized_specialization<T, 2, DERIVED_T> : public geometry::shape_base<DERIVED_T>
			{
			public:
				using derived_t = DERIVED_T;

			private:
				utils_cuda_available constexpr const derived_t& derived() const noexcept { return static_cast<const derived_t&>(*this); }
				utils_cuda_available constexpr       derived_t& derived()       noexcept { return static_cast<derived_t&>(*this); }
				utils_cuda_available constexpr const auto     & get_arr() const noexcept { return derived().array; }
				utils_cuda_available constexpr       auto     & get_arr()       noexcept { return derived().array; }

				using arr_t = std::array<T, 2>;

			public:
				inline static constexpr const size_t static_size{std::tuple_size_v<arr_t>};
				using value_type = typename arr_t::value_type;

				template<std::floating_point T, T f_a_v>
				utils_cuda_available  static constexpr derived_t from(const math::angle::base<T, f_a_v>& angle, T magnitude = 1) noexcept
					{
					auto x{angle.cos() * magnitude};
					auto y{angle.sin() * magnitude};
					return derived_t{x, y};
					}

				template <typename T = float, T f_a_v = 360.f>
				utils_cuda_available constexpr math::angle::base<T, f_a_v> angle() const noexcept { return math::angle::base<T, f_a_v>::atan2(derived().y, derived().x); }
				
				// VEC & ANGLE OPERATIONS
				template <std::floating_point T, T f_a_v> utils_cuda_available constexpr derived_t  operator+ (const math::angle::base<T, f_a_v>& angle) const noexcept { auto ret{derived()}; ret += angle; return ret; }
				template <std::floating_point T, T f_a_v> utils_cuda_available constexpr derived_t  operator- (const math::angle::base<T, f_a_v>& angle) const noexcept { auto ret{derived()}; ret -= angle; return ret; }
				template <std::floating_point T, T f_a_v> utils_cuda_available constexpr derived_t& operator+=(const math::angle::base<T, f_a_v>& angle)       noexcept 
					{
					derived().x = derived().x * angle.cos() - derived().y * angle.sin();
					derived().y = derived().x * angle.sin() + derived().y * angle.cos();
					return derived(); 
					}
				template <std::floating_point T, T f_a_v> utils_cuda_available constexpr derived_t& operator-=(const math::angle::base<T, f_a_v>& angle)       noexcept
					{
					auto nngle{-angle};
					derived().x = derived().x * nngle.cos() - derived().y * nngle.sin();
					derived().y = derived().x * nngle.sin() + derived().y * nngle.cos();
					return derived();
					}

				template <std::floating_point T, T f_a_v> utils_cuda_available constexpr derived_t& operator= (const math::angle::base<T, f_a_v>& angle)       noexcept
					{
					return derived() = {angle.cos() * derived().magnitude(), angle.sin() * derived().magnitude()}; 
					}

				// OTHER
				utils_cuda_available constexpr derived_t perpendicular_right           () const noexcept { return { derived().y, -derived().x}; }
				utils_cuda_available constexpr derived_t perpendicular_left            () const noexcept { return {-derived().y,  derived().x}; }
				utils_cuda_available constexpr derived_t perpendicular_clockwise       () const noexcept { return perpendicular_right(); }
				utils_cuda_available constexpr derived_t perpendicular_counterclockwise() const noexcept { return perpendicular_left (); }

#pragma region geometry

				using geometry::shape_base<derived_t>::closest_point_and_distance;
				using geometry::shape_base<derived_t>::closest_point_to;
				using geometry::shape_base<derived_t>::distance_min;
				using geometry::shape_base<derived_t>::vector_to;
				using geometry::shape_base<derived_t>::intersects;
				using geometry::shape_base<derived_t>::intersection;
				using geometry::shape_base<derived_t>::contains;
				using geometry::shape_base<derived_t>::collides_with;

				utils_cuda_available vec2f closest_point_to(const geometry::point         & b) const noexcept;
				utils_cuda_available vec2f closest_point_to(const geometry::segment       & b) const noexcept;
				utils_cuda_available vec2f closest_point_to(const geometry::aabb          & b) const noexcept;
				utils_cuda_available vec2f closest_point_to(const geometry::polygon       & b) const noexcept;
				utils_cuda_available vec2f closest_point_to(const geometry::convex_polygon& b) const noexcept;
				utils_cuda_available vec2f closest_point_to(const geometry::circle        & b) const noexcept;
				utils_cuda_available vec2f closest_point_to(const geometry::capsule       & b) const noexcept;
				utils_cuda_available float distance_min    (const geometry::point         & b) const noexcept;
				utils_cuda_available float distance_min    (const geometry::segment       & b) const noexcept;
				utils_cuda_available float distance_min    (const geometry::aabb          & b) const noexcept;
				utils_cuda_available float distance_min    (const geometry::polygon       & b) const noexcept;
				utils_cuda_available float distance_min    (const geometry::convex_polygon& b) const noexcept;
				utils_cuda_available float distance_min    (const geometry::circle        & b) const noexcept;
				utils_cuda_available float distance_min    (const geometry::capsule       & b) const noexcept;

				utils_cuda_available bool contains(const geometry::point         & b) const noexcept;
				utils_cuda_available bool contains(const geometry::segment       & b) const noexcept;
				utils_cuda_available bool contains(const geometry::aabb          & b) const noexcept;
				utils_cuda_available bool contains(const geometry::polygon       & b) const noexcept;
				utils_cuda_available bool contains(const geometry::convex_polygon& b) const noexcept;
				utils_cuda_available bool contains(const geometry::circle        & b) const noexcept;
				utils_cuda_available bool contains(const geometry::capsule       & b) const noexcept;

				utils_cuda_available derived_t& scale_self    (const float      & scaling    ) noexcept;
				utils_cuda_available derived_t& rotate_self   (const angle::radf& rotation   ) noexcept;
				utils_cuda_available derived_t& translate_self(const vec2f      & translation) noexcept;

				geometry::aabb bounding_box() const noexcept;
#pragma endregion geometry
			};
		}
	}

#include "geometry/common/begin.h"