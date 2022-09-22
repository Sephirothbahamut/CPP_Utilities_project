#pragma once
#include <vector>
#include <array>

#include "../../../compilation/debug.h"
#include "../../../memory.h"
#include "../../../id_pool.h"
#include "../../../containers/storage_for.h" 

//std::hive aka skipfield_hive
//   more memory
//   iterable
//   
//TODO next_hive
//   fastest emplace
//   slightly slower remove
//   no iteration
//   no size or ex
//
//pool_hive aka object_pool
//   slightly slower emplace
//   slightly more memory
//   no iteration
//   size 😎

//TODO test, allocator

namespace utils::containers::hive
	{
	template <typename T, size_t inner_size, float growth_factor = 1.2f>
	class pool
		{
		public:
			using handle_t        = utils::id_pool_manual::value_type;

			using value_type      = T;
			using size_type       = size_t;
			using reference       = T&;
			using const_reference = const T&;
			using pointer         = T*;
			using const_pointer   = const T*;

			~pool()
				{
				//TODO
				//sort id_pool unused indices, iterate inner_container from 0 to id_pool.count and call destructor for elements in non-unused indices
				}

			template <typename ...Args>
			handle_t emplace(Args&& ...args)
				{
				handle_t new_handle{ id_pool.get() };
				if (new_handle >= capacity())
					{
					if (inner_container.empty()) { grow(1); }
					else { grow(inner_container.size() * growth_factor); }
					}
					
				
				auto& slot{ inner_get(new_handle) };
				try  { slot.emplace(std::forward<Args>(args)...); }
				catch(...)
					{ 
					id_pool.release(new_handle);
					throw;
					}
				return new_handle;
				}

			void remove(handle_t handle)
				{
				id_pool.release(handle);

				auto& slot{ inner_get(handle) };
				slot.destroy();
				}

			      T& operator[](handle_t& handle)       noexcept
				{
				return inner_get(handle).value();
				}
			const T& operator[](handle_t& handle) const noexcept
				{
				return inner_get(handle).value();
				}

			size_t capacity() const noexcept { return inner_container.size() * inner_size; }
			size_t size    () const noexcept { return capacity() - id_pool.used(); }
			bool   empty   () const noexcept { return size() == 0; }

		private:
			std::vector<std::unique_ptr<std::array<storage_for<T>, inner_size>>> inner_container;
			utils::id_pool_manual id_pool;

			      storage_for<T>& inner_get(size_t index)       noexcept { return (*(inner_container[index / inner_size]))[index % inner_size]; }
			const storage_for<T>& inner_get(size_t index) const noexcept { return (*(inner_container[index / inner_size]))[index % inner_size]; }

			void grow(size_t new_arrays_count)
				{
				for (size_t i = inner_container.size(); i < new_arrays_count; i++)
					{
					inner_container.emplace_back(std::make_unique<std::array<storage_for<T>, inner_size>>());
					}
				}
		};
	}