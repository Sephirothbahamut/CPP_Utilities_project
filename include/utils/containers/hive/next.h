#pragma once
#include <vector>
#include <array>

#include "../storage_for.h"
#include "../../compilation/debug.h"
#include "../../memory.h"
#include "../../id_pool.h"

//TODO allocator
//TODO research union's alignment vs storage_for?
namespace utils::containers::hive
	{

	template <typename T, size_t inner_size = 8, float growth_factor = 1.2f>
	class next
		{
		private:
			union slot_t 
				{
				T element; size_t next_free; 

				//these are needed because the inner_container's array of slot_t needs to know an explicit constructor and destructor for slot_t when slot_t contains a non-trivially constructible/destructible type
				slot_t () {}
				~slot_t() {}
				};
		
		public:
			next() = default;

			//TODO implement
			next(next&& move) = delete;
			next& operator=(next&& move) = delete;
			//TONOTDO do not implement
			next(const next& copy) = delete;
			next& operator=(const next& copy) = delete;


			using handle_t        = utils::id_pool_manual::value_type;

			using value_type      = T;
			using size_type       = size_t;
			using reference       = T&;
			using const_reference = const T&;
			using pointer         = T*;
			using const_pointer   = const T*;

			~next()
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

			template <typename ...Args>
			size_t emplace(Args&& ...args)
				{
				if (first_free >= capacity())
					{
					if (inner_container.empty()) { grow(1); }
					else { grow(inner_container.size() * growth_factor); }
					}

				auto index{first_free};
				auto& slot{inner_get(index)};
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
				my_slot.element.~T();

				size_t* last_free_before_me{&first_free};

				//Increase until the first empty slot which next_free is greater than index
				while (index > *last_free_before_me) { last_free_before_me = &(inner_get(*last_free_before_me).next_free); }

				auto previous_free{*last_free_before_me};
				*last_free_before_me = index;
				
				my_slot.next_free = previous_free;
				}
			
			      T& operator[](size_t handle)       noexcept
				{
				return inner_get(handle).element;
				}
			const T& operator[](size_t handle) const noexcept
				{
				return inner_get(handle).element;
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