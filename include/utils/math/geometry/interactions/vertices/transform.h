#pragma once

#include "base.h"
#include "../point.h"

namespace utils::math::geometry::interactions
	{
	utils_gpu_available constexpr typename auto& scale_self    (shape::concepts::vertices auto& vertices, const float                      & scaling    ) noexcept { for(auto& vertex : vertices) { scale_self    (vertex, scaling    ); } return vertices; }
	utils_gpu_available constexpr typename auto& rotate_self   (shape::concepts::vertices auto& vertices, const angle::concepts::angle auto& rotation   ) noexcept { for(auto& vertex : vertices) { rotate_self   (vertex, rotation   ); } return vertices; }
	utils_gpu_available constexpr typename auto& translate_self(shape::concepts::vertices auto& vertices, const vec2f                      & translation) noexcept { for(auto& vertex : vertices) { translate_self(vertex, translation); } return vertices; }
	utils_gpu_available constexpr typename auto& transform_self(shape::concepts::vertices auto& vertices, const transform2                 & transform  ) noexcept { for(auto& vertex : vertices) { transform_self(vertex, transform  ); } return vertices; }
	}