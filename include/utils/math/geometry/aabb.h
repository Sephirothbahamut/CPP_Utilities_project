#pragma once

#include "common.h"

namespace utils::math::geometry
	{
	struct aabb : public rect<float>, shape_base<aabb>
		{
		aabb bounding_box() const noexcept { return *this; }
		};
	}