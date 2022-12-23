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
			class handle_raw 
				{
				friend class handled_container;

				public:
					using value_type      = T;
					using reference       =       value_type&;
					using const_reference = const value_type&;
					using pointer         =       value_type*;
					using const_pointer   = const value_type* const;

					handle_raw() = default;

					bool has_value() const noexcept { return index != std::numeric_limits<id_pool_manual::value_type>::max(); }

					void reset() noexcept { index = std::numeric_limits<id_pool_manual::value_type>::max(); }
					
					bool operator==(const handle_raw& other) { return index == other.index; }

				protected:
					id_pool_manual::value_type index{std::numeric_limits<id_pool_manual::value_type>::max()};

					handle_raw(id_pool_manual::value_type index) : index{index} {}
				};
			class handle_wrapper : public handle_raw
				{
				friend class handled_container;

				public:
					using value_type      = T;
					using reference       =       value_type&;
					using const_reference = const value_type&;
					using pointer         =       value_type*;
					using const_pointer   = const value_type* const;

					handle_wrapper() = default;

					      reference operator* ()       noexcept { return container_ptr->operator[](*this); }
					const_reference operator* () const noexcept { return container_ptr->operator[](*this); }

					      pointer   operator->()       noexcept { return std::addressof(container_ptr->operator[](*this)); }
					const_pointer   operator->() const noexcept { return std::addressof(container_ptr->operator[](*this)); }

					      reference value     ()       noexcept { return container_ptr->operator[](*this); }
					const_reference value     () const noexcept { return container_ptr->operator[](*this); }

					      pointer   get       ()       noexcept { return std::addressof(container_ptr->operator[](*this)); }
					const_pointer   get       () const noexcept { return std::addressof(container_ptr->operator[](*this)); }

					void erase_and_reset()
						{
						if (handle_raw::has_value())
							{
							container_ptr->erase(*this);
							handle_raw::reset();
							}
						}

					bool operator==(const handle_wrapper& other) { return (container_ptr == other.container_ptr) && handle_raw::operator==(other); }

				protected:
					utils::observer_ptr<handled_container<T, Allocator>> container_ptr;

					handle_wrapper(handled_container<T, Allocator>& container, handle_raw inner_handle) : container_ptr{&container}, handle_raw{inner_handle} {};
				};

			class handle_unique : public handle_wrapper
				{
				friend class handled_container;

				public:
					using value_type      = T;
					using reference       =       value_type&;
					using const_reference = const value_type&;
					using pointer         =       value_type*;
					using const_pointer   = const value_type* const;

					handle_unique() = default;

					handle_unique           (const handle_unique& copy) = delete;
					handle_unique& operator=(const handle_unique& copy) = delete;
					handle_unique           (      handle_unique&& move) noexcept : handle_wrapper{std::move(move)} { move.release(); }
					handle_unique& operator=(      handle_unique&& move) noexcept { handle_wrapper::container_ptr = move.container_ptr; handle_raw::index = move.index; move.release(); return *this; }

					~handle_unique() { reset(); }

					void reset() { release().erase_and_reset(); }
					handle_wrapper release()
						{
						handle_wrapper ret{*this};
						handle_raw::reset();
						return ret;
						}

				private:
					handle_unique(handled_container<T, Allocator>& container, handle_raw inner_handle) : handle_wrapper{container, inner_handle} {};
				};
			[[nodiscard]] handle_wrapper make_wrapped(handle_raw handle) { return {*this, handle}; }
			[[nodiscard]] handle_unique  make_unique (handle_raw handle) { return {*this, handle}; }

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
			handle_raw emplace(Args&& ...args)
				{
				inner_container.emplace_back(std::forward<Args>(args)...);
				return create_new_handle(inner_container.size() - 1);
				}

			template <typename ...Args>
			handle_raw push(Args& ...args)
				{
				inner_container.push_back(args...);
				return create_new_handle(inner_container.size() - 1);
				}
	
			      T& operator[](handle_raw& handle)       noexcept
				{
				return inner_container[handle_to_container_index[handle.index]];
				}
			const T& operator[](const handle_raw& handle) const noexcept
				{
				return inner_container[handle_to_container_index[handle.index]];
				}
			
			void erase(handle_raw& handle)
				{
				if (handle_to_container_index[handle.index] < (inner_container.size() - 1))
					{
					size_t to_remove_index{handle_to_container_index[handle.index]};
					size_t other_moved_index{inner_container.size() - 1};

					inner_container[to_remove_index] = std::move(inner_container[other_moved_index]);

					handle_to_container_index[container_index_to_handle[other_moved_index]] = to_remove_index;
					container_index_to_handle[to_remove_index] = container_index_to_handle[other_moved_index];
					}
				inner_container.pop_back();
				container_index_to_handle.pop_back();

				handle_to_container_index[handle.index] = std::numeric_limits<id_pool_manual::value_type>::max();

				id_pool.release(handle.index);
				handle.index = std::numeric_limits<id_pool_manual::value_type>::max();
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
			[[nodiscard]] handle_raw create_new_handle(size_t new_element_index) noexcept
				{
				handle_raw handle{id_pool.get()};
				if (handle.index >= handle_to_container_index.size())
					{
					handle_to_container_index.push_back(new_element_index);
					}
				else
					{
					handle_to_container_index[handle.index] = new_element_index;
					}
				container_index_to_handle.push_back(handle.index);
				return handle;
				}

			inner_container_t inner_container;
			id_pool_manual id_pool;
			std::vector<id_pool_manual::value_type> handle_to_container_index;
			std::vector<size_t/*, Allocator*/> container_index_to_handle;
		};
	}