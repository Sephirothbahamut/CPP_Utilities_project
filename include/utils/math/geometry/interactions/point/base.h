#pragma once

#include "../base_types.h"
#include "../../shape/point.h"

namespace utils::math::geometry::interactions
	{
	template <shape::concepts::point T>
	utils_gpu_available constexpr return_types::bounding_box bounding_box(const T& point) noexcept
		{
		return {point.x(), point.y(), point.x(), point.y()};
		}
	}