#pragma once

#include "../vec2.h"
#include "aabb.h"

namespace utils::math::geometry
	{
	struct point : vec2f 
		{
		using vec2f::vec2f; 
		point(const vec2f& copy) : vec2f{ copy } {}
		
		operator aabb() const noexcept { return {.rr{x}, .up{y}, .ll{x}, .dw{y}}; }
		};
	}