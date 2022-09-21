#pragma once
#include <vector>
#include <tuple>

#include "../compilation/debug.h"
#include "../memory.h"
#include "../id_pool.h"

//TODO see below smarter handled container
//TODO write test cases

namespace utils::containers
	{
	/// <summary>
	/// Alternative to map. Emplace operations return an handle which can be used to remove elements. It's meant to be iterated a lot more than modified, so it will have better iteration performance than a map (underlying vector).
	/// </summary>
	template <typename T, class Allocator = std::allocator<T>>
	class multihandled
		{
		protected:
			using inner_container_t = std::vector<T, Allocator>;

		public:
			//using handle_t = size_t;
			class handle_t 
				{
				friend class multihandled<T>;//, Allocator>
				public:
					handle_t(const handle_t& copy) : container_ptr{copy.container_ptr}, mapping_index{copy.mapping_index}
						{
						if (is_valid()) { container_ptr->mapping_to_container_index[mapping_index].count++; }
						}
					handle_t& operator=(const handle_t& copy)
						{
						if (is_valid()) { container_ptr->mapping_to_container_index[mapping_index].count--; }
						container_ptr = copy.container_ptr;
						mapping_index = copy.mapping_index;
						if (is_valid()) { container_ptr->mapping_to_container_index[mapping_index].count++; }
						}
					handle_t           (handle_t&& move) noexcept : value{move.value} { move.value = std::numeric_limits<id_pool_manual::value_type>::max(); }
					handle_t& operator=(handle_t&& move) noexcept { value = move.value; move.value = std::numeric_limits<id_pool_manual::value_type>::max(); return *this; }

					~handle_t() 
						{
						if (is_valid()) { container_ptr->mapping_to_container_index[mapping_index].count--; }
						}
				private:
					bool is_valid() const noexcept
						{
						return mapping_index != std::numeric_limits<size_t>::max();
						}
					handle_t(multihandled<T>& container, size_t mapping_index) : container_ptr{&container}, mapping_index{mapping_index} 
						{
						container.mapping_to_container_index[mapping_index].count++;
						};

					utils::observer_ptr<multihandled<T>> container_ptr;
					id_pool_manual::value_type mapping_index{std::numeric_limits<id_pool_manual::value_type>::max()};
				};
			friend class handle_t;

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
				size_t mapping_index{create_new_mapping(inner_container.size() - 1)};
				return {*this, mapping_index};
				}

			template <typename ...Args>
			handle_t push(Args& ...args)
				{
				inner_container.push_back(args...);
				size_t mapping_index{create_new_mapping(inner_container.size() - 1)};
				return {*this, mapping_index};
				}
	
			      T& operator[](handle_t& handle)       noexcept 
				{
				return inner_container[mapping_to_container_index[handle.value]]; 
				}
			const T& operator[](handle_t& handle) const noexcept 
				{
				return inner_container[mapping_to_container_index[handle.value]];
				}
			
			void remove(handle_t& handle)
				{
				if (mapping_to_container_index[handle.value] < (inner_container.size() - 1))
					{
					size_t to_remove_index{mapping_to_container_index[handle.value]};
					size_t other_moved_index{inner_container.size() - 1};

					inner_container[to_remove_index] = std::move(inner_container[other_moved_index]);

					mapping_to_container_index[container_index_to_mapping[other_moved_index]] = to_remove_index;
					container_index_to_mapping[to_remove_index] = container_index_to_mapping[other_moved_index];
					}
				inner_container.pop_back();
				container_index_to_mapping.pop_back();

				mapping_to_container_index[handle.value] = std::numeric_limits<id_pool_manual::value_type>::max();

				id_pool.release(handle.value);
				handle = std::numeric_limits<id_pool_manual::value_type>::max();
				}
	
			size_t size () const noexcept { return inner_container.size (); }
			bool   empty() const noexcept { return inner_container.empty(); }

			const auto begin  () const noexcept { return inner_container.begin  (); }
			      auto begin  ()       noexcept { return inner_container.begin  (); }
			const auto end    () const noexcept { return inner_container.end    (); }
			      auto end    ()       noexcept { return inner_container.end    (); }
			const auto cbegin () const noexcept { return inner_container.cbegin (); }
			      auto cbegin ()       noexcept { return inner_container.cbegin (); }
			const auto cend   () const noexcept { return inner_container.cend   (); }
			      auto cend   ()       noexcept { return inner_container.cend   (); }
			const auto rbegin () const noexcept { return inner_container.rbegin (); }
			      auto rbegin ()       noexcept { return inner_container.rbegin (); }
			const auto rend   () const noexcept { return inner_container.rend   (); }
			      auto rend   ()       noexcept { return inner_container.rend   (); }
			const auto crbegin() const noexcept { return inner_container.crbegin(); }
			      auto crbegin()       noexcept { return inner_container.crbegin(); }
			const auto crend  () const noexcept { return inner_container.crend  (); }
			      auto crend  ()       noexcept { return inner_container.crend  (); }

		protected:
			size_t create_new_mapping(size_t element_index) noexcept
				{
				size_t mapping_index{id_pool.get()};
				if (mapping_index >= mapping_to_container_index.size())
					{
					mapping_to_container_index.emplace_back(element_index);
					}
				else
					{
					mapping_to_container_index[mapping_index] = {element_index};
					}
				container_index_to_mapping.push_back(mapping_index);
				return mapping_index;
				}

			inner_container_t inner_container;
			id_pool_manual id_pool;

			struct handle_mapping_data { size_t container_index; size_t count{0}; };
			std::vector<handle_mapping_data> mapping_to_container_index;
			std::vector<size_t/*, Allocator*/> container_index_to_mapping;
		};

	}