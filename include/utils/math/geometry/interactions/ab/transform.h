#pragma once

#include "base.h"

namespace utils::math::geometry::interactions
	{
	utils_gpu_available constexpr typename auto& scale_self    (shape::concepts::ab auto& ab, const float                      & scaling    ) noexcept { scale_self    (ab.a, scaling    ); scale_self    (ab.b, scaling    ); return ab; }
	utils_gpu_available constexpr typename auto& rotate_self   (shape::concepts::ab auto& ab, const angle::concepts::angle auto& rotation   ) noexcept { rotate_self   (ab.a, rotation   ); rotate_self   (ab.b, rotation   ); return ab; }
	utils_gpu_available constexpr typename auto& translate_self(shape::concepts::ab auto& ab, const vec2f                      & translation) noexcept { translate_self(ab.a, translation); translate_self(ab.b, translation); return ab; }
	}