#pragma once

#include <vector>
#include <limits>
#include <stdexcept>

namespace utils
	{
	class id_pool
		{
		public:
			using id_t = size_t;
			using value_type = id_t;
			inline static constexpr size_t min{std::numeric_limits<id_t>::min()};
			inline static constexpr size_t max{std::numeric_limits<id_t>::max()};

			size_t size()      const noexcept { return max - min; }
			size_t available() const noexcept { return max - count; }
			size_t used()      const noexcept { return count - min; }
			bool   empty()     const noexcept { return available() == 0; }

			id_t get() noexcept
				{
				if (unused.size()) 
					{
					id_t ret = unused.back();
					unused.pop_back();
					return ret;
					}
				else { return count++; }
				}

			id_t get_except()
				{
				if (empty()) { throw std::out_of_range{"All available ids have already been assigned"}; }
				return get();
				}

			void release(id_t id)
				{
				unused.push_back(id);
				}

			id_t last() { return count - 1; }

		private:
			id_t count = min;
			std::vector<id_t> unused;
		};
	}