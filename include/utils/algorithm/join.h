#pragma once

#include <string>
#include <sstream>

namespace utils::algorithm
	{
	template <typename input_iteratorr>
	std::string join(input_iteratorr begin, input_iteratorr end, const std::string& separator)
		{
		if (begin == end) { return ""; }
		std::stringstream ss; 
		ss << *begin;
		begin++;
		for (; begin != end; begin++) { ss << separator << *begin; }
		return ss.str();
		}
	}