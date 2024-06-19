#pragma once

#include "../base_types.h"
#include "../../shape/ab.h"

namespace utils::math::geometry::interactions
	{
	utils_gpu_available constexpr return_types::bounding_box bounding_box(const shape::concepts::ab auto& ab) noexcept
		{
		return return_types::bounding_box::create::from_vertices(ab.a, ab.b);
		}
	}