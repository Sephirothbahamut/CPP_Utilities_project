#pragma once

#include "base.h"

namespace utils::math::geometry::interactions
	{
	utils_gpu_available constexpr typename auto& scale_self    (const shape::concepts::polyline auto& polyline, const float                      & scaling    ) noexcept { for(auto& vertex : polyline) { vertex.scale_self    (scaling    ); } return polyline; }
	utils_gpu_available constexpr typename auto& rotate_self   (const shape::concepts::polyline auto& polyline, const angle::concepts::angle auto& rotation   ) noexcept { for(auto& vertex : polyline) { vertex.rotate_self   (rotation   ); } return polyline; }
	utils_gpu_available constexpr typename auto& translate_self(const shape::concepts::polyline auto& polyline, const vec2f                      & translation) noexcept { for(auto& vertex : polyline) { vertex.translate_self(translation); } return polyline; }
	utils_gpu_available constexpr typename auto& translate_self(const shape::concepts::polyline auto& polyline, const transform2                 & transform  ) noexcept { for(auto& vertex : polyline) { vertex.transform_self(transform  ); } return polyline; }
	}