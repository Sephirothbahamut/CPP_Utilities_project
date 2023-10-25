#pragma once

#include "../common.h"

namespace utils::beta::math::geometry::shape::interface
	{
	template <typename derived_t>
	struct ray : details::base<derived_t>
		{
		
		};
	}

namespace utils::beta::math::geometry::shape
	{
	namespace view
		{
		struct ray : interface::ray<ray>
			{
			};
		}

	struct ray : interface::ray<ray>
		{
		};
	}