#pragma once

#include <future>
#include <chrono>

namespace utils::future
	{
	template <typename T>
	bool is_ready(const std::future<T>& future)
		{
		return future.wait_for(std::chrono::seconds{0}) == std::future_status::ready;
		}
	}