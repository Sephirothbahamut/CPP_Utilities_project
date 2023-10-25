#pragma once

#include "../common.h"

namespace utils::beta::math::geometry::shape::interface
	{
	template <typename derived_t>
	struct segment : details::base<derived_t>
		{

		};
	}

namespace utils::beta::math::geometry::shape
	{
	namespace view
		{
		struct segment : interface::segment<segment>
			{
			};
		}

	struct segment : interface::segment<segment>
		{
		};
	}