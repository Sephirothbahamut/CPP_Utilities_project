#pragma once
#include "math.h"

// https://stackoverflow.com/questions/22999487/update-the-average-of-a-continuous-sequence-of-numbers-in-constant-time/53618572?noredirect=1#comment123527741_53618572

//TODO write test cases


namespace utils::math
	{
	template <typename T>
	class cumulative_average
		{
		public:
			void push(T value) noexcept
				{
				//avg = ((avg * count) + value) / (count + 1);
				avg = avg + (value - avg) / (count + 1);
				count++;
				}

			/// <summary>
			/// Note: does NOT check nor know if the value being removed was actually once pushed in the average. Such knowledge should be mantained on the user side.
			/// </summary>
			void remove(T value) noexcept
				{
				avg = avg - (value - avg) / (count - 1);
				count--;
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