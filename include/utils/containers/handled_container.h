#pragma once
#include <vector>
#include <tuple>

#include "../compilation/debug.h"
#include "../id_pool.h"


//TODO use hive::next
//TODO write test cases

namespace utils::containers
	{
	/// <summary>
	/// Alternative to map. Emplace operations return an handle which can be used to remove elements. It's meant to be iterated a lot more than modified, so it will have better iteration performance than a map (underlying vector).
	/// </summary>
	template <typename T, class Allocator = std::allocator<T>>
	class handled_container
		{
		protected:
			using inner_container_t = std::vector<T, Allocator>;

		public:
			//using handle_t = size_t;
			class handle_t 
				{
				friend class handled_container<T>;//, Allocator>
				public:
					handle_t() = default;
					handle_t(id_pool_manual::value_type value) : value{value} {};
					handle_t(const handle_t& copy) = default;
					handle_t& operator=(const handle_t& copy) = default;
					handle_t           (handle_t&& move) noexcept : value{move.value} { move.value = std::numeric_limits<id_pool_manual::value_type>::max(); }
					handle_t& operator=(handle_t&& move) noexcept { value = move.value; move.value = std::numeric_limits<id_pool_manual::value_type>::max(); return *this; }
				private:
					id_pool_manual::value_type value{std::numeric_limits<id_pool_manual::value_type>::max()};
				};

			using value_type             = inner_container_t::value_type;
			using size_type              = inner_container_t::size_type;
			using reference              = inner_container_t::reference;
			using const_reference        = inner_container_t::const_reference;
			using pointer                = inner_container_t::pointer;
			using const_pointer          = inner_container_t::const_pointer;
			using iterator               = inner_container_t::iterator;
			using const_iterator         = inner_container_t::const_iterator;
			using reverse_iterator       = inner_container_t::reverse_iterator;
			using const_reverse_iterator = inner_container_t::const_reverse_iterator;
	
			template <typename ...Args>
			handle_t emplace(Args&& ...args)
				{
				inner_container.emplace_back(std::forward<Args>(args)...);
				return create_new_handle(inner_container.size() - 1);
				}

			template <typename ...Args>
			handle_t push(Args& ...args)
				{
				inner_container.push_back(args...);
				return create_new_handle(inner_container.size() - 1);
				}
	
			      T& operator[](handle_t& handle)       noexcept 
				{
				return inner_container[handle_to_container_index[handle.value]]; 
				}
			const T& operator[](handle_t& handle) const noexcept 
				{
				return inner_container[handle_to_container_index[handle.value]];
				}
			
			void remove(handle_t& handle)
				{
				if (handle_to_container_index[handle.value] < (inner_container.size() - 1))
					{
					size_t to_remove_index{handle_to_container_index[handle.value]};
					size_t other_moved_index{inner_container.size() - 1};

					inner_container[to_remove_index] = std::move(inner_container[other_moved_index]);

					handle_to_container_index[container_index_to_handle[other_moved_index]] = to_remove_index;
					container_index_to_handle[to_remove_index] = container_index_to_handle[other_moved_index];
					}
				inner_container.pop_back();
				container_index_to_handle.pop_back();

				handle_to_container_index[handle.value] = std::numeric_limits<id_pool_manual::value_type>::max();

				id_pool.release(handle.value);
				handle = std::numeric_limits<id_pool_manual::value_type>::max();
				}
	
			size_t size() const noexcept { return inner_container.size(); }
			bool empty() const noexcept { return inner_container.empty(); }

			const auto begin()   const noexcept { return inner_container.begin(); }
			      auto begin()         noexcept { return inner_container.begin(); }
			const auto end()     const noexcept { return inner_container.end(); }
			      auto end()           noexcept { return inner_container.end(); }
			const auto cbegin()  const noexcept { return inner_container.cbegin(); }
			      auto cbegin()        noexcept { return inner_container.cbegin(); }
			const auto cend()    const noexcept { return inner_container.cend(); }
			      auto cend()          noexcept { return inner_container.cend(); }
			const auto rbegin()  const noexcept { return inner_container.rbegin(); }
			      auto rbegin()        noexcept { return inner_container.rbegin(); }
			const auto rend()    const noexcept { return inner_container.rend(); }
			      auto rend()          noexcept { return inner_container.rend(); }
			const auto crbegin() const noexcept { return inner_container.crbegin(); }
			      auto crbegin()       noexcept { return inner_container.crbegin(); }
			const auto crend()   const noexcept { return inner_container.crend(); }
			      auto crend()         noexcept { return inner_container.crend(); }

		protected:
			handle_t create_new_handle(size_t new_element_index) noexcept
				{
				handle_t handle{id_pool.get()};
				if (handle.value >= handle_to_container_index.size())
					{
					handle_to_container_index.push_back(new_element_index);
					}
				else
					{
					handle_to_container_index[handle.value] = new_element_index;
					}
				container_index_to_handle.push_back(handle.value);
				return handle;
				}

			inner_container_t inner_container;
			id_pool_manual id_pool;
			std::vector<id_pool_manual::value_type> handle_to_container_index;
			std::vector<size_t/*, Allocator*/> container_index_to_handle;
		};

	}