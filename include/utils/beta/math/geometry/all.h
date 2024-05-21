#pragma once

#include "details/base_types.h"

#include "interface/all.h"

#include "interactions/all.h"

namespace utils::math::geometry::shape::details
	{
	template <typename derived_t, shapes_enum shape_id> utils_gpu_available constexpr side                          base<derived_t, shape_id>::side_of              (const concepts::any auto& other) const noexcept { return geometry::interactions::side_of              (derived(), other); }
	template <typename derived_t, shapes_enum shape_id> utils_gpu_available constexpr float                         base<derived_t, shape_id>::distance             (const concepts::any auto& other) const noexcept { return geometry::interactions::distance             (derived(), other); }
	template <typename derived_t, shapes_enum shape_id> utils_gpu_available constexpr geometry::signed_distance_t   base<derived_t, shape_id>::distance_signed      (const concepts::any auto& other) const noexcept { return geometry::interactions::distance_signed      (derived(), other); }
	template <typename derived_t, shapes_enum shape_id> utils_gpu_available constexpr vec2f                         base<derived_t, shape_id>::closest_point_of     (const concepts::any auto& other) const noexcept { return geometry::interactions::closest_point_of     (derived(), other); }
	template <typename derived_t, shapes_enum shape_id> utils_gpu_available constexpr vec2f                         base<derived_t, shape_id>::closest_point_to     (const concepts::any auto& other) const noexcept { return geometry::interactions::closest_point_to     (derived(), other); }
	template <typename derived_t, shapes_enum shape_id> utils_gpu_available constexpr closest_point_with_distance_t base<derived_t, shape_id>::closest_with_distance(const concepts::any auto& other) const noexcept { return geometry::interactions::closest_with_distance(derived(), other); }
	template <typename derived_t, shapes_enum shape_id> utils_gpu_available constexpr closest_pair_with_distance_t  base<derived_t, shape_id>::closest_pair         (const concepts::any auto& other) const noexcept { return geometry::interactions::closest_pair         (derived(), other); }
	template <typename derived_t, shapes_enum shape_id> utils_gpu_available constexpr vec2f                         base<derived_t, shape_id>::vector_to            (const concepts::any auto& other) const noexcept { return geometry::interactions::vector_to            (derived(), other); }
	template <typename derived_t, shapes_enum shape_id> utils_gpu_available constexpr bool                          base<derived_t, shape_id>::intersects           (const concepts::any auto& other) const noexcept { return geometry::interactions::intersects           (derived(), other); }
	template <typename derived_t, shapes_enum shape_id> utils_gpu_available constexpr std::optional<vec2f>          base<derived_t, shape_id>::intersection_with    (const concepts::any auto& other) const noexcept { return geometry::interactions::intersection_with    (derived(), other); }
	template <typename derived_t, shapes_enum shape_id> utils_gpu_available constexpr bool                          base<derived_t, shape_id>::contains             (const concepts::any auto& other) const noexcept { return geometry::interactions::contains             (derived(), other); }
	template <typename derived_t, shapes_enum shape_id> utils_gpu_available constexpr bool                          base<derived_t, shape_id>::collides_with        (const concepts::any auto& other) const noexcept { return geometry::interactions::collides_with        (derived(), other); }
	}