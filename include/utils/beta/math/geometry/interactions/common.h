#pragma once

#include "base_interactions.h"

namespace utils::math::geometry::interactions
	{
	namespace common
		{
		namespace common
			{
			utils_gpu_available constexpr bool collides_with(const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept
				{
				return interactions::contains(a, b) || interactions::intersects(a, b);
				}

			utils_gpu_available constexpr vec2f closest_point_to(const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept
				{
				return interactions::closest_point_of(b, a);
				}

			//utils_gpu_available constexpr side                          side_of              (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { utils::concepts::unimplemented_specialization<decltype(a)>(); }
			//utils_gpu_available constexpr float                         distance             (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { utils::concepts::unimplemented_specialization<decltype(a)>(); }
			
			//utils_gpu_available constexpr geometry::signed_distance_t   distance_signed      (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { utils::concepts::unimplemented_specialization<decltype(a)>(); }
			////utils_gpu_available constexpr vec2f                         closest_point_to     (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { utils::concepts::unimplemented_specialization<decltype(a)>(); }
			utils_gpu_available constexpr closest_point_with_distance_t closest_with_distance(const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { utils::concepts::unimplemented_specialization<decltype(a)>(); }
			utils_gpu_available constexpr closest_pair_with_distance_t  closest_pair         (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { utils::concepts::unimplemented_specialization<decltype(a)>(); }
			utils_gpu_available constexpr vec2f                         vector_to            (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { utils::concepts::unimplemented_specialization<decltype(a)>(); }
			//utils_gpu_available constexpr bool                          intersects           (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { utils::concepts::unimplemented_specialization<decltype(a)>(); }
			//utils_gpu_available constexpr std::optional<vec2f>          intersection_with    (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { utils::concepts::unimplemented_specialization<decltype(a)>(); }
			//utils_gpu_available constexpr bool                          contains             (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { utils::concepts::unimplemented_specialization<decltype(a)>(); }
			////utils_gpu_available constexpr bool                          collides_with        (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { utils::concepts::unimplemented_specialization<decltype(a)>(); }
			}

		namespace point
			{
			utils_gpu_available constexpr geometry::side side_of(const shape::concepts::any auto& a, const shape::concepts::point auto& b) noexcept
				{
				return {interactions::distance(a, b)};
				}
			utils_gpu_available constexpr geometry::signed_distance_t distance_signed(const shape::concepts::any auto& a, const shape::concepts::point auto& b) noexcept
				{
				return {interactions::distance(a, b)};
				}
			}
		}
	using namespace common::common;
	using namespace common::point;
	}