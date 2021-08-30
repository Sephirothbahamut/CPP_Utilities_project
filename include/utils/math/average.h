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
				if (utils::will_overflow_mult(avg, static_cast<T>(count))) { return; }

				avg = ((avg * count) + value) / (count + 1);
				count++;
				}
			T get_average() const noexcept { return avg; }

			cumulative_average& operator+=(T value) { push(value); return *this; }
			operator T() const { return avg; }

		private:
			size_t count = 0;
			T avg;
		};
	}