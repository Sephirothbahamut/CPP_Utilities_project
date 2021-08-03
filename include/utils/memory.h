#pragma once
#include <memory>

namespace utils
	{
	template <typename T, typename Deleter>
	std::unique_ptr<T, Deleter> make_unique_with_deleter(T* ptr) { return std::unique_ptr<T, Deleter>(ptr); }

	template <typename T>
	using observer_ptr = T*;
	}