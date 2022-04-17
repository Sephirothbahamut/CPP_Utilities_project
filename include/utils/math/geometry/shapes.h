#pragma once
#include "../../concepts.h"

#include "shapes_discrete.h"
#include "shapes_continuous.h"

namespace utils::math::geometry
	{
	template <typename T>
	concept shape = shape_discrete<T> || shape_continuous<T>;
	}