#pragma once

#include "common.h"

namespace utils::math::geometry::interactions
	{
	namespace details
		{
		template <auto function>
		utils_gpu_available constexpr auto inverted(const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return (b.*function)(a); }

		utils_gpu_available constexpr side                           side_from_distance_signed                      (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return a.distance(b).side(); }
		utils_gpu_available constexpr float                          distance_from_distance_signed                  (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return a.distance(b).distance(); }
		utils_gpu_available constexpr geometry::signed_distance_t    distance_signed_from_side_and_distance         (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return {.value{a.distance(b) * a.side_of(b)}}; }
		utils_gpu_available constexpr vec2f                          closest_from_closest_with_distance             (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return a.closest_with_distance(b).closest ; }
		utils_gpu_available constexpr float                          distance_from_closest_with_distance            (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return a.closest_with_distance(b).distance; }
		utils_gpu_available constexpr closest_point_with_distance_t  closest_with_distance_from_closest_and_distance(const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return {a.closest_point_to(b), a.distance(b)}; }
		utils_gpu_available constexpr closest_pair_with_distance_t   closest_pair_from_closest_and_other_closest    (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return {a.closest_point_to(b), b.closest_point_to(a), a.distance(b)}; }
		utils_gpu_available constexpr vec2f                          vector_to_from_closest_pair                    (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return b.closest_point_to(a) - a.closest_point_to(b); }
		utils_gpu_available constexpr bool                           intersects_from_intersection                   (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return a.intersection_with(b).has_value(); }
		utils_gpu_available constexpr std::optional<vec2f>           intersection_from_intersects_and_closest       (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return a.intersects(b) ? std::optional<vec2f>{a.closest_point_to(b)} : std::nullopt; }
		utils_gpu_available constexpr bool                           contains_from_side                             (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return b.distance(a).side().is_inside(); }
		utils_gpu_available constexpr bool                           collides_with_from_intersects_and_contains     (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return a.intersects(b) || a.contains(b); }

		//some operations (closest point to) cannot be inverted for obvious reasons
		utils_gpu_available constexpr geometry::signed_distance_t  other_distance             (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return -b.distance            (a); }
		utils_gpu_available constexpr closest_pair_with_distance_t other_closest_pair_inverted(const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return -b.closest_pair        (a); }
		utils_gpu_available constexpr vec2f                        other_vector_to_inverted   (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return -b.vector_to           (a); }
		utils_gpu_available constexpr bool                         other_intersects           (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return  b.intersects          (a); }
		utils_gpu_available constexpr std::optional<vec2f>         other_intersection         (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return  b.intersection_with   (a); }
		utils_gpu_available constexpr bool                         other_collides_with        (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return  b.collides_with       (a); }
		}

	namespace point
		{
		namespace point
			{
			utils_gpu_available constexpr geometry::signed_distance_t distance(const shape::concepts::point auto& a, const shape::concepts::point auto& b) noexcept
				{
				return {utils::math::vec2f::distance(a, b)};
				}
			}
		namespace circle
			{
			utils_gpu_available constexpr geometry::signed_distance_t distance(const shape::concepts::point auto& a, const shape::concepts::circle auto& b) noexcept
				{
				return {distance(other.center_point()).value - other.radius};
				}
			}
		}
	}

namespace utils::math::geometry
	{
	using namespace interactions::details;
	using namespace interactions::point::point;
	using namespace interactions::point::circle  ;
	//using namespace interactions::point::line    ;
	//using namespace interactions::point::ray     ;
	//using namespace interactions::point::segment ;
	//using namespace interactions::point::aabb    ;
	//using namespace interactions::point::capsule ;
	//using namespace interactions::point::polyline;
	//using namespace interactions::point::bezier  ;
	//using namespace interactions::point::polygon ;
	//
	//using namespace interactions::circle  ::point;
	//using namespace interactions::line    ::point;
	//using namespace interactions::ray     ::point;
	//using namespace interactions::segment ::point;
	//using namespace interactions::aabb    ::point;
	//using namespace interactions::capsule ::point;
	//using namespace interactions::polyline::point;
	//using namespace interactions::bezier  ::point;
	//using namespace interactions::polygon ::point;
	}