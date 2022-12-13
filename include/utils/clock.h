#pragma once
#include <chrono>

namespace utils
	{
	template<typename Clock, typename T>
	class clock
		{
		public:
			using time_point_t = std::chrono::time_point<Clock, std::chrono::duration<T>>;
			using duration_t   = std::chrono::duration<T>;

			duration_t get_elapsed() noexcept
				{
				time_point_t new_time_point{ Clock::now() };
				return { new_time_point - last_time_point };
				}

			duration_t restart() noexcept
				{
				time_point_t new_time_point{ Clock::now() };
				duration_t ret{ new_time_point - last_time_point };
				last_time_point = new_time_point;
				return ret;
				}

		private:
			time_point_t last_time_point{ Clock::now() };
		};
	}