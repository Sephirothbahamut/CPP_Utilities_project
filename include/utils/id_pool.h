#pragma once

#include <vector>
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <memory>

#include "memory.h"
#include "compilation/debug.h"

//TODO allocators support

namespace utils
	{
	class id_pool_manual
		{
		public:
			using value_type = size_t;
			inline static constexpr value_type min{std::numeric_limits<value_type>::min()};
			inline static constexpr value_type max{std::numeric_limits<value_type>::max()};

			value_type size()  const noexcept { return max - used(); }
			value_type used()  const noexcept { return count - unused.size(); }
			bool       empty() const noexcept { return size() == 0; }

			value_type get() noexcept
				{
				if constexpr (utils::compilation::debug) { return get_except(); }
				else { return get_inner(); }
				}

			value_type get_except()
				{
				if (empty()) { throw std::out_of_range{"All available ids have already been assigned"}; }
				return get_inner();
				}

			void release(value_type id)
				{
				if (id == count - 1) { count--; }
				else { unused.push_back(id); }
				}

		private:
			value_type get_inner() noexcept
				{
				if (unused.size())
					{
					size_t ret = unused.back();
					unused.pop_back();
					return ret;
					}
				else { return count++; }
				}


			size_t count = min;
			std::vector<size_t> unused;
		};

	class id_pool
		{
		public:
			using value_type = size_t;
			
			class id_t
				{
				public:
					id_t(id_pool& pool) : pool{&pool}, _value{pool.get()} {}
					id_t(const id_t& copy) = delete;
					id_t& operator=(const id_t& copy) = delete;
					id_t(id_t&& move) noexcept : pool{move.pool}, _value{move._value} { move._value = std::numeric_limits<id_pool::value_type>::max(); }
					id_t& operator=(id_t&& move) noexcept { release(); pool = move.pool; _value = move._value; move._value = std::numeric_limits<id_pool::value_type>::max(); }
					~id_t() { release(); }

					operator id_pool::value_type() const noexcept { return _value; }
					id_pool::value_type value() const noexcept { return _value; }

				private:
					void release() const noexcept
						{
						if (_value != std::numeric_limits<size_t>::max()) { pool->release(_value); }
						}

					utils::observer_ptr<id_pool> pool;
					id_pool::value_type _value;
				};
			friend class id_t;

			inline static constexpr value_type min{std::numeric_limits<value_type>::min()};
			inline static constexpr value_type max{std::numeric_limits<value_type>::max()};

			value_type size()      const noexcept { return max - used(); }
			value_type used()      const noexcept { return count - unused.size(); }
			bool       empty()     const noexcept { return size() == 0; }


		private:
			value_type count = min;
			std::vector<value_type> unused;

			value_type get() noexcept
				{
				if constexpr (utils::compilation::debug) { return get_except(); }

				if (unused.size())
					{
					value_type ret = unused.back();
					unused.pop_back();
					return ret;
					}
				else { return count++; }
				}

			value_type get_except()
				{
				if (empty()) { throw std::out_of_range{"All available ids have already been assigned"}; }
				return get();
				}

			void release(value_type id)
				{
				unused.push_back(id);
				}
		};
	}