#pragma once

#include <vector>

#include "../common/begin.h"
#include "segment.h"

namespace utils::math::geometry
	{
	struct edge_ref
		{
		utils::math::vecref2f a;
		utils::math::vecref2f b;

		inline operator segment() const noexcept { return segment{a, b}; }
		};
	}