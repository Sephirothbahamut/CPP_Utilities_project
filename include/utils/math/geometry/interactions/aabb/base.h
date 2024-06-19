#pragma once

#include "../base_types.h"
#include "../../shape/aabb.h"

namespace utils::math::geometry::interactions
	{
	utils_gpu_available constexpr return_types::bounding_box bounding_box(const shape::concepts::aabb auto& aabb) noexcept
		{
		return aabb;
		}
	}