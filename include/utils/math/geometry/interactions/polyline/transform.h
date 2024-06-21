#pragma once

#include "base.h"
#include "../point.h"

namespace utils::math::geometry::interactions
	{
	utils_gpu_available constexpr typename auto& scale_self    (shape::concepts::polyline auto& polyline, const float                      & scaling    ) noexcept { for(auto& vertex : polyline) { scale_self    (vertex, scaling    ); } return polyline; }
	utils_gpu_available constexpr typename auto& rotate_self   (shape::concepts::polyline auto& polyline, const angle::concepts::angle auto& rotation   ) noexcept { for(auto& vertex : polyline) { rotate_self   (vertex, rotation   ); } return polyline; }
	utils_gpu_available constexpr typename auto& translate_self(shape::concepts::polyline auto& polyline, const vec2f                      & translation) noexcept { for(auto& vertex : polyline) { translate_self(vertex, translation); } return polyline; }
	utils_gpu_available constexpr typename auto& transform_self(shape::concepts::polyline auto& polyline, const transform2                 & transform  ) noexcept { for(auto& vertex : polyline) { transform_self(vertex, transform  ); } return polyline; }
	}