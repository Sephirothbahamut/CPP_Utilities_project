#pragma once
#include <vector>
#include <array>

#include "../compilation/debug.h"
#include "../memory.h"
#include "../id_pool.h"

//TODO test, allocator

namespace utils::containers
	{

	template <typename T, size_t inner_size, float growth_factor = 1.2f>
	class hive
		{
		private:
			union slot_t { T element; size_t next_free; };
		
		public:

			template <typename ...Args>
			size_t emplace(Args&& ...args)
				{
				if (first_free >= capacity()) { grow(inner_container.size() * growth_factor); }

				auto& slot{inner_get(first_free)};
				auto previous_next_free{slot.next_free};

				try
					{
					new(&slot.element) T{std::forward<Args>(args)...};
					}
				catch (...)
					{
					slot.next_free = previous_next_free;
					throw;
					}

				first_free = previous_next_free;
				return index;
				}

			void remove(size_t index)
				{
				auto& my_slot{inner_get(index)};
				my_slot.element->~T();

				if (index < first_free)
					{
					auto previous_first_free{first_free};
					first_free = index;
					inner_get(first_free).next_free = previous_first_free;
					}
				else
					{
					//TODO
					size_t stuff{first_free};
					while (next_free(stuff) < index)
						{
						stuff = next_free(stuff);
						}

					auto& last_free_before_me_slot{inner_get(first_free)};
					
					my_slot.next_free = last_free_before_me_slot.next_free;
					last_free_before_me_slot.next_free = index;
					}
				}

			~hive()
				{
				size_t free{first_free};

				for (size_t i{0}; i < capacity(); i++)
					{
					if (i != free)
						{
						auto& slot{inner_get(first_free)};
						slot.element.~T();
						}
					else { free = next_free(free); }
					}
				}

			size_t capacity() const noexcept { return inner_container.size() * inner_size; }
			
		private:
			std::vector<std::unique_ptr<std::array<slot_t, inner_size>>> inner_container;

			size_t first_free{0};
			size_t last_free {0};

			
			      slot_t& inner_get(size_t index)       noexcept { return (*(inner_container[index / inner_size]))[index % inner_size]; }
			const slot_t& inner_get(size_t index) const noexcept { return (*(inner_container[index / inner_size]))[index % inner_size]; }

			size_t next_free(size_t current) const noexcept { return inner_get(current).next_free; }

			void grow(size_t new_arrays_count)
				{
				for (size_t i = inner_container.size(); i < new_arrays_count; i++)
					{
					inner_container.emplace_back(std::make_unique<std::array<slot_t, inner_size>>());
					}
				for (size_t i{last_free}; i < capacity(); i++)
					{
					inner_get(i).next_free = i + 1;
					}
				last_free = capacity();
				}
		};
	}