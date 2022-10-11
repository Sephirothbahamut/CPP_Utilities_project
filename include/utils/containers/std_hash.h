#pragma once

#include <array>
//TODO other std containers :) someday, maybe

namespace std
	{
	template <typename T, size_t size>
	struct hash<std::array<T, size>>
		{
		size_t operator()(std::array<T, size> const& array) const
			{
			size_t ret{0};
			for (const auto& element : array) { ret * 31 + std::hash<T>{}(element); }
			return ret;
			}
		};
	}