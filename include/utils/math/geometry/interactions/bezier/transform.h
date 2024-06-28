#pragma once

#include "base.h"
#include "../point.h"

namespace utils::math::geometry::interactions
	{
	utils_gpu_available constexpr typename auto& scale_self    (shape::concepts::bezier auto& bezier, const float                      & scaling    ) noexcept { for(auto& vertex : bezier) { scale_self    (vertex, scaling    ); } return bezier; }
	utils_gpu_available constexpr typename auto& rotate_self   (shape::concepts::bezier auto& bezier, const angle::concepts::angle auto& rotation   ) noexcept { for(auto& vertex : bezier) { rotate_self   (vertex, rotation   ); } return bezier; }
	utils_gpu_available constexpr typename auto& translate_self(shape::concepts::bezier auto& bezier, const vec2f                      & translation) noexcept { for(auto& vertex : bezier) { translate_self(vertex, translation); } return bezier; }
	utils_gpu_available constexpr typename auto& transform_self(shape::concepts::bezier auto& bezier, const transform2                 & transform  ) noexcept { for(auto& vertex : bezier) { transform_self(vertex, transform  ); } return bezier; }
	}