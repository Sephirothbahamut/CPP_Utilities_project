#pragma once

#include <vector>
#include <limits>
#include <stdexcept>

namespace utils
	{
	class id_pool_manual
		{
		public:
			inline static constexpr size_t min{std::numeric_limits<size_t>::min()};
			inline static constexpr size_t max{std::numeric_limits<size_t>::max()};

			size_t size()      const noexcept { return max - used(); }
			size_t used()      const noexcept { return count - unused.size(); }
			bool   empty()     const noexcept { return size() == 0; }

			size_t get() noexcept
				{
				if (unused.size()) 
					{
					size_t ret = unused.back();
					unused.pop_back();
					return ret;
					}
				else { return count++; }
				}

			size_t get_except()
				{
				if (empty()) { throw std::out_of_range{"All available ids have already been assigned"}; }
				return get();
				}

			void release(size_t id)
				{
				unused.push_back(id);
				}

		private:
			size_t count = min;
			std::vector<size_t> unused;
		};

	class id_t;
	class id_pool
		{
		public:
			friend class id_t;

			inline static constexpr size_t min{std::numeric_limits<size_t>::min()};
			inline static constexpr size_t max{std::numeric_limits<size_t>::max()};

			size_t size()      const noexcept { return max - used(); }
			size_t used()      const noexcept { return count - unused.size(); }
			bool   empty()     const noexcept { return size() == 0; }

		private:
			size_t count = min;
			std::vector<size_t> unused;

			size_t get() noexcept
				{
				if (unused.size())
					{
					size_t ret = unused.back();
					unused.pop_back();
					return ret;
					}
				else { return count++; }
				}

			size_t get_except()
				{
				if (empty()) { throw std::out_of_range{"All available ids have already been assigned"}; }
				return get();
				}

			void release(size_t id)
				{
				unused.push_back(id);
				}
		};

	class id_t
		{
		public:
			id_t(id_pool& pool) : pool{&pool}, _value{pool.get()} {}
			id_t(const id_t& copy) = delete;
			id_t& operator=(const id_t& copy) = delete;
			id_t(id_t&& move) noexcept : pool{move.pool}, _value{move._value} { move._value = std::numeric_limits<size_t>::max(); }
			id_t& operator=(id_t&& move) noexcept { release(); pool = move.pool; _value = move._value; move._value = std::numeric_limits<size_t>::max(); }
			~id_t() { release(); }

			operator size_t() const noexcept { return _value; }
			size_t value() const noexcept { return _value; }

		private:
			void release() const noexcept
				{
				if (_value != std::numeric_limits<size_t>::max()) { pool->release(_value); }
				}

			id_pool* pool;
			size_t _value;
		};
	}