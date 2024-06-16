#pragma once

#include <iostream>

#include "base.h"

namespace utils::details::vector
	{
	template<const char* name>
	struct utils_oop_empty_bases output
		{
		inline static constexpr const char* static_name{name}; 
		};
	}