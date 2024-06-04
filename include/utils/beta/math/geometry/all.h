#pragma once

#include "details/base_types.h"

#include "shape/all.h"

#include "interactions/all.h"

namespace utils::math::geometry::shape
	{
	template <typename derived_t> utils_gpu_available constexpr side                        interface<derived_t>::side_of              (const auto& other) const noexcept { return geometry::interactions::side_of              (derived(), other); }
	template <typename derived_t> utils_gpu_available constexpr float                       interface<derived_t>::distance             (const auto& other) const noexcept { return geometry::interactions::distance             (derived(), other); }
	template <typename derived_t> utils_gpu_available constexpr geometry::signed_distance   interface<derived_t>::distance_signed      (const auto& other) const noexcept { return geometry::interactions::distance_signed      (derived(), other); }
	template <typename derived_t> utils_gpu_available constexpr vec2f                       interface<derived_t>::closest_point_of     (const auto& other) const noexcept { return geometry::interactions::closest_point_of     (derived(), other); }
	template <typename derived_t> utils_gpu_available constexpr vec2f                       interface<derived_t>::closest_point_to     (const auto& other) const noexcept { return geometry::interactions::closest_point_to     (derived(), other); }
	template <typename derived_t> utils_gpu_available constexpr closest_point_with_distance interface<derived_t>::closest_with_distance(const auto& other) const noexcept { return geometry::interactions::closest_with_distance(derived(), other); }
	template <typename derived_t> utils_gpu_available constexpr closest_pair_with_distance  interface<derived_t>::closest_pair         (const auto& other) const noexcept { return geometry::interactions::closest_pair         (derived(), other); }
	template <typename derived_t> utils_gpu_available constexpr vec2f                       interface<derived_t>::vector_to            (const auto& other) const noexcept { return geometry::interactions::vector_to            (derived(), other); }
	template <typename derived_t> utils_gpu_available constexpr bool                        interface<derived_t>::intersects           (const auto& other) const noexcept { return geometry::interactions::intersects           (derived(), other); }
	template <typename derived_t> utils_gpu_available constexpr std::optional<vec2f>        interface<derived_t>::intersection_with    (const auto& other) const noexcept { return geometry::interactions::intersection_with    (derived(), other); }
	template <typename derived_t> utils_gpu_available constexpr bool                        interface<derived_t>::contains             (const auto& other) const noexcept { return geometry::interactions::contains             (derived(), other); }
	template <typename derived_t> utils_gpu_available constexpr bool                        interface<derived_t>::collides_with        (const auto& other) const noexcept { return geometry::interactions::collides_with        (derived(), other); }
	}