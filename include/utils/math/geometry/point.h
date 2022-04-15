#pragma once

#include "../vec2.h"
#include "aabb.h"

namespace utils::math::geometry
	{
	struct point : vec2f 
		{
		using vec2f::vec2; 
		
		operator aabb() const noexcept { return {.ll{x}, .up{y}, .rr{x}, .dw{y}}; }
		};
	}