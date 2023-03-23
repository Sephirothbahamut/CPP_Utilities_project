#pragma once
#include "../../concepts.h"

#include "shapes/continuous_point.h"

namespace utils::math::geometry
	{
	template <typename T>
	concept shape_continuous = ::utils::concepts::any_of<T, continuous_point>;
	}