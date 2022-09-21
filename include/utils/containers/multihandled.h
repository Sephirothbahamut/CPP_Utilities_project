#pragma once
#include <vector>
#include <tuple>

#include "../compilation/debug.h"
#include "../memory.h"
#include "../id_pool.h"
#include "hive/pool.h"

//TODO see below smarter handled container
//TODO write test cases

namespace utils::containers
	{
	/// <summary>
	/// Alternative to map. Emplace operations return an handle which can be used to remove elements. It's meant to be iterated a lot more than modified, so it will have better iteration performance than a map (underlying vector).
	/// </summary>
	template <typename T, size_t inner_size = 8, class Allocator = std::allocator<T>>
	class multihandled
		{
		protected:
			using inner_container_t = hive::pool<T, inner_size>;

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
					handle_t           (handle_t&& move) noexcept : mapping_index { move.mapping_index } { move.mapping_index = std::numeric_limits<id_pool_manual::value_type>::max(); }
					handle_t& operator=(handle_t&& move) noexcept { mapping_index = move.mapping_index;    move.mapping_index = std::numeric_limits<id_pool_manual::value_type>::max(); return *this; }

					~handle_t() 
						{
						if (is_valid()) 
							{
							auto& count = container_ptr->mapping_to_container_index[mapping_index].count;
							count--;
							//if the handle in the container isn't used externally anymore and the resources has been removed
							if (count == 0 && container_ptr->mapping_to_container_index[mapping_index].container_index == std::numeric_limits<id_pool_manual::value_type>::max())
								{
								//release the inner handle
								container_ptr->id_pool.release(mapping_index);
								}
							}
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

			template <typename ...Args>
			handle_t emplace(Args&& ...args)
				{
				typename inner_container_t::handle_t inner_handle{ inner_container.emplace(std::forward<Args>(args)...) };
				size_t mapping_index{create_new_mapping(inner_handle)};
				return {*this, mapping_index};
				}

			      T& operator[](handle_t& handle)       noexcept 
				{
				return inner_container[mapping_to_container_index[handle.mapping_index].container_index]; 
				}
			const T& operator[](handle_t& handle) const noexcept 
				{
				return inner_container[mapping_to_container_index[handle.mapping_index].container_index];
				}
			
			void remove(handle_t& handle)
				{
				inner_container.remove(mapping_to_container_index[handle.mapping_index].container_index);
				mapping_to_container_index[handle.mapping_index].container_index = std::numeric_limits<id_pool_manual::value_type>::max();
				}
	
			handle_t undergo_mythosis(const handle_t& source_handle) noexcept
				{
				auto source_index{ mapping_to_container_index[source_handle.mapping_index].container_index };
				size_t mapping_index{ create_new_mapping(source_index) };
				return { *this,  mapping_index };
				}

			handle_t splice(const handle_t& source_handle) noexcept { return undergo_mythosis(source_handle); }

			void remap(handle_t& handle_to_remap, const handle_t& handle_toward_target) noexcept
				{
				auto source_index{ mapping_to_container_index[handle_toward_target.mapping_index].container_index };
				mapping_to_container_index[handle_to_remap.mapping_index].container_index = source_index;
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
				return mapping_index;
				}

			inner_container_t inner_container;
			id_pool_manual id_pool;

			struct handle_mapping_data { inner_container_t::handle_t container_index; size_t count{0}; };
			std::vector<handle_mapping_data> mapping_to_container_index;
		};

	}