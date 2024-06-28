#pragma once

#include "../base_types.h"
#include "../../shape/polyline.h"

namespace utils::math::geometry::interactions
	{
	utils_gpu_available constexpr return_types::bounding_box bounding_box(const shape::concepts::polyline auto& polyline) noexcept
		{
		return return_types::bounding_box::create::from_vertices(polyline);
		}
	}