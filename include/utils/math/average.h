#pragma once
#include "math.h"

namespace utils
	{
	template <typename T>
	class cumulative_average
		{
		public:
			void push(T value) noexcept
				{
				avg = ((avg * count) + value) / (count + 1);
				count++;
				}
			T get_average() const noexcept { return avg; }
			size_t get_count() const noexcept { return count; }

			void reset() noexcept
				{
				avg = T{0};
				count = 0;
				}

			cumulative_average& operator+=(T value) { push(value); return *this; }
			operator T() const { return avg; }

		private:
			size_t count = 0;
			T avg = T{0};
		};
	}