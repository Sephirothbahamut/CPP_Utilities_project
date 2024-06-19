#pragma once

#include "../base_interactions.h"

namespace utils::math::geometry::interactions
	{
	namespace point
		{
		namespace common
			{
			utils_gpu_available constexpr bool contains(const shape::concepts::point auto& a, const shape::concepts::any auto& b) noexcept
				{
				return false;
				}

			utils_gpu_available constexpr std::optional<vec2f> intersection_with(const shape::concepts::point auto& a, const shape::concepts::any auto& b) noexcept
				{
				return a.intersects(b) ? std::optional<vec2f>{a} : std::nullopt;
				}
			}

		namespace point
			{
			utils_gpu_available constexpr geometry::side side_of(const shape::concepts::point auto& a, const shape::concepts::point auto& b) noexcept
				{
				return a == b ? geometry::side::create::coincident() : geometry::side::create::outside();
				}
			utils_gpu_available constexpr float distance(const shape::concepts::point auto& a, const shape::concepts::point auto& b) noexcept
				{
				return utils::math::vec2f::distance(a, b);
				}
			utils_gpu_available constexpr geometry::signed_distance_t distance_signed(const shape::concepts::point auto& a, const shape::concepts::point auto& b) noexcept
				{
				return {interactions::point::point::distance(a, b)};
				}

			utils_gpu_available constexpr vec2f closest_point_of(const shape::concepts::point auto& a, const shape::concepts::point auto& b) noexcept
				{
				return b;
				}

			utils_gpu_available constexpr bool intersects(const shape::concepts::point auto& a, const shape::concepts::point auto& b) noexcept
				{
				return a == b;
				}
			}

		namespace circle
			{
			utils_gpu_available constexpr geometry::signed_distance_t distance_signed(const shape::concepts::point auto& a, const shape::concepts::circle auto& b) noexcept
				{
				return {interactions::point::point::distance(a, b.center_point()) - b.radius};
				}
			utils_gpu_available constexpr geometry::side side_of(const shape::concepts::point auto& a, const shape::concepts::circle auto& b) noexcept
				{
				return interactions::point::circle::distance_signed(a, b).side();
				}
			utils_gpu_available constexpr geometry::signed_distance_t distance(const shape::concepts::point auto& a, const shape::concepts::circle auto& b) noexcept
				{
				return interactions::point::circle::distance_signed(a, b).absolute();
				}

			utils_gpu_available constexpr vec2f closest_point_of(const shape::concepts::point auto& a, const shape::concepts::circle auto& b) noexcept
				{
				const shape::segment b_center_to_a{b.center, a};
				return b.center + (b_center_to_a.forward() * b.radius);
				}

			utils_gpu_available constexpr bool intersects(const shape::concepts::point auto& a, const shape::concepts::circle auto& b) noexcept
				{
				return distance(a, b).is_coincident();
				}

			}

		namespace ab
			{
			utils_gpu_available constexpr geometry::side side_of(const shape::concepts::point auto& a, const shape::concepts::ab auto& b) noexcept 
				{
				return {b.some_significant_name_ive_yet_to_figure_out(a)}; 
				}

			utils_gpu_available constexpr vec2f closest_point_of(const shape::concepts::point auto& a, const shape::concepts::ab auto& b) noexcept
				{
				constexpr ends_t ends{std::remove_cvref_t<decltype(b)>::static_ends};
				return b.closest_point_to_custom_clamp<ends.is_a_finite(), ends.is_b_finite()>(a);
				}

			utils_gpu_available constexpr geometry::signed_distance_t distance_signed(const shape::concepts::point auto& a, const shape::concepts::ab auto& b) noexcept
				{
				const float tmp{b.some_significant_name_ive_yet_to_figure_out(a)};
				const float length{shape::segment{b.a, b.b}.length()}; //needs finite length to calculate distance even if ab is infinite

				constexpr ends_t ends{std::remove_cvref_t<decltype(b)>::static_ends};
				if constexpr (!ends.is_infinite())
					{
					const float projected_percent{b.projected_percent(a)};
					if constexpr (ends.is_a_finite())
						{
						if (projected_percent < 0.f) { return {utils::math::vec2f::distance(a, b.a) * geometry::side{tmp}}; }
						}
					if constexpr (ends.is_b_finite())
						{
						if (projected_percent > 1.f) { return {utils::math::vec2f::distance(a, b.b) * geometry::side{tmp}}; }
						}
					}

				return {tmp / length}; 
				}

			utils_gpu_available constexpr closest_point_with_distance_t closest_with_distance(const shape::concepts::point auto& a, const shape::concepts::ab auto& b) noexcept
				{
				const auto closest_point{interactions::point::ab::closest_point_of(a, b)};
				const auto side{interactions::point::ab::side_of(a, b)};
				const signed_distance_t signed_distance{interactions::point::point::distance(a, closest_point) * side};
				return {closest_point, signed_distance};
				}

			utils_gpu_available constexpr float distance(const shape::concepts::point auto& a, const shape::concepts::ab auto& b) noexcept
				{
				constexpr ends_t ends{std::remove_cvref_t<decltype(b)>::static_ends};

				const utils::math::vec2f b_a{b.b - b.a};
				const utils::math::vec2f p_a{a   - b.a};
				float h{utils::math::vec2f::dot(p_a, b_a) / utils::math::vec2f::dot(b_a, b_a)};
				if constexpr (ends.is_a_finite()) { h = utils::math::max(h, 0.f); }
				if constexpr (ends.is_b_finite()) { h = utils::math::min(h, 1.f); }

				return (p_a - (b_a * h)).length;
				}
			}

		namespace aabb
			{
			utils_gpu_available constexpr geometry::signed_distance_t distance_signed(const shape::concepts::point auto& a, const shape::concepts::aabb auto& b) noexcept
				{
				const auto point_from_center_ur_quadrant{utils::math::abs(a - b.get_center())};
				const auto corner_from_center{b.ur() - b.get_center()};
				const auto distances{point_from_center_ur_quadrant - corner_from_center};
				return utils::math::max(distances, 0.f).lenght + utils::math::min(utils::math::max(distances.x, distances.y), 0.f);
				}
			}
		}
	}