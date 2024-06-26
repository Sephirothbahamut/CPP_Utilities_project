#pragma once

#include <filesystem>

#include "colour.h"
#include "../matrix_interface.h"

namespace utils::graphics
	{
	struct image
		{
		image(utils::math::vec2s sizes) : matrix{sizes} {}

		utils::matrix<colour::rgba_u> matrix;
		};
	}

#ifdef utils_implementation
#include "image.cpp"
#endif