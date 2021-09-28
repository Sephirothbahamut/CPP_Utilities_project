#pragma once

#include <string>
#include <sstream>

namespace utils::algorithm
	{
	template <typename T>
	std::string join(typename T::const_iterator begin, typename T::const_iterator end, const std::string& separator)
		{
		if (begin == end) { return ""; }
		std::stringstream ss{*begin};
		begin++;
		for (; begin != end; begin++) { ss << separator << *begin; }
		return ss.str();
		}
	}