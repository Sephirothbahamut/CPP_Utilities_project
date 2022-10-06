#pragma once
#include <vector>
#include <tuple>

#include "../compilation/debug.h"
#include "../memory.h"
#include "../id_pool.h"
#include "hive/next.h"

//TODO see below smarter handled container
//TODO write test cases

namespace utils::containers
	{
	/// <summary>
	/// Alternative to map. Emplace operations return an handle which can be used to remove elements. It's meant to be iterated a lot more than modified, so it will have better iteration performance than a map (underlying vector).
	/// </summary>
	template <typename T, size_t inner_size = 8>
	class multihandled
		{
		protected:
			using inner_container_t = hive::next<T, inner_size>;
			using inner_iterator_t  = inner_container_t::iterator;

		public:
			class handle_t 
				{
				friend class multihandled<T>;

				public:
					using value_type      = T;
					using reference       = value_type&;
					using const_reference = const value_type&;
					using pointer         = value_type*;
					using const_pointer   = const value_type* const;

					handle_t(const handle_t& copy) : container_ptr{copy.container_ptr}, mapping_index{copy.mapping_index}
						{
						if (is_valid()) { container_ptr->mapping_to_inner_iterator[mapping_index].count++; }
						}
					handle_t& operator=(const handle_t& copy)
						{
						if (is_valid()) { container_ptr->mapping_to_inner_iterator[mapping_index].count--; }
						container_ptr = copy.container_ptr;
						mapping_index = copy.mapping_index;
						if (is_valid()) { container_ptr->mapping_to_inner_iterator[mapping_index].count++; }
						}
					handle_t           (handle_t&& move) noexcept : container_ptr{ std::move(move.container_ptr) }, mapping_index { move.mapping_index } { move.mapping_index = std::numeric_limits<id_pool_manual::value_type>::max(); }
					handle_t& operator=(handle_t&& move) noexcept { container_ptr = move.container_ptr;             mapping_index = move.mapping_index;    move.mapping_index = std::numeric_limits<id_pool_manual::value_type>::max(); return *this; }

					~handle_t() 
						{
						if (is_valid()) 
							{
							auto& count = container_ptr->mapping_to_inner_iterator[mapping_index].count;
							count--;
							//if the handle in the container isn't used externally anymore and the resources has been erased
							if (count == 0)
								{
								if (content_exists())
									{
									container_ptr->erase_if_last(*this);
									}

								//release the inner handle
								container_ptr->id_pool.release(mapping_index);
								}
							}
						}

					constexpr       reference operator* ()       noexcept { return container_ptr->operator[](*this); }
					constexpr const_reference operator* () const noexcept { return container_ptr->operator[](*this); }

					constexpr       pointer   operator->()       noexcept { return std::addressof(container_ptr->operator[](*this)); }
					constexpr const_pointer   operator->() const noexcept { return std::addressof(container_ptr->operator[](*this)); }

					constexpr       reference value     ()       noexcept { return container_ptr->operator[](*this); }
					constexpr const_reference value     () const noexcept { return container_ptr->operator[](*this); }

					constexpr       pointer   get       ()       noexcept { return std::addressof(container_ptr->operator[](*this)); }
					constexpr const_pointer   get       () const noexcept { return std::addressof(container_ptr->operator[](*this)); }

					bool has_value() const noexcept { return is_valid() && content_exists(); }

					handle_t splice(                  )    const noexcept { return container_ptr->splice(*this    ); }
					void     remap (const handle_t& to)          noexcept {        container_ptr->remap (*this, to); }

				private:
					bool is_valid      () const noexcept { return mapping_index != std::numeric_limits<size_t>::max(); }
					bool content_exists() const noexcept { return container_ptr->mapping_to_inner_iterator[mapping_index].inner_iterator != inner_iterator_t{}; }

					handle_t(multihandled<T>& container, size_t mapping_index) : 
						container_ptr{&container}, mapping_index{mapping_index}
						{
						container.mapping_to_inner_iterator[mapping_index].count++;
						};

					utils::observer_ptr<multihandled<T>> container_ptr;
					id_pool_manual::value_type mapping_index{std::numeric_limits<id_pool_manual::value_type>::max()};
				};
			friend class handle_t;

			using value_type      = inner_container_t::value_type;
			using size_type       = inner_container_t::size_type;
			using reference       = inner_container_t::reference;
			using const_reference = inner_container_t::const_reference;
			using pointer         = inner_container_t::pointer;
			using const_pointer   = inner_container_t::const_pointer;

			template <typename ...Args>
			handle_t emplace(Args&& ...args)
				{
				typename inner_container_t::iterator inner_iterator{ inner_container.emplace(std::forward<Args>(args)...) };
				size_t mapping_index{create_new_mapping(inner_iterator)};
				return {*this, mapping_index };
				}

			      T& operator[](const handle_t& handle)       noexcept
				{
				return *mapping_to_inner_iterator[handle.mapping_index].inner_iterator;
				}
			const T& operator[](const handle_t& handle) const noexcept 
				{
				return *mapping_to_inner_iterator[handle.mapping_index].inner_iterator;
				}

			void erase(handle_t& handle)
				{
				auto container_iterator_being_removed{ mapping_to_inner_iterator[handle.mapping_index].inner_iterator };

				inner_container.erase(container_iterator_being_removed);

				for (auto& mapping : mapping_to_inner_iterator)
					{
					if (mapping.inner_iterator == container_iterator_being_removed)
						{
						mapping.inner_iterator = {};
						}
					}
				}
			void erase_and_remap(handle_t& handle, const handle_t& to)
				{
				auto container_index_being_removed{mapping_to_inner_iterator[handle.mapping_index].inner_iterator};

				inner_container.erase(container_index_being_removed);

				for (auto& mapping : mapping_to_inner_iterator)
					{
					if (mapping.inner_iterator == container_index_being_removed)
						{
						mapping.inner_iterator = mapping_to_inner_iterator[to.mapping_index].inner_iterator;
						}
					}
				}
	
			handle_t undergo_mythosis(const handle_t& source_handle) noexcept
				{
				auto source_iterator{ mapping_to_inner_iterator[source_handle.mapping_index].inner_iterator };
				size_t mapping_index{ create_new_mapping(source_iterator) };
				return {*this,  mapping_index};
				}

			handle_t splice(const handle_t& source_handle) noexcept { return undergo_mythosis(source_handle); }

			void remap(handle_t& handle_to_remap, const handle_t& handle_toward_target) noexcept
				{
				auto source_iterator{ mapping_to_inner_iterator[handle_toward_target.mapping_index].inner_iterator };
				mapping_to_inner_iterator[handle_to_remap.mapping_index].inner_iterator = source_iterator;
				}

		protected:
			size_t create_new_mapping(inner_iterator_t element_index) noexcept
				{
				size_t mapping_index{id_pool.get()};
				if (mapping_index >= mapping_to_inner_iterator.size())
					{
					mapping_to_inner_iterator.emplace_back(element_index);
					}
				else
					{
					mapping_to_inner_iterator[mapping_index] = {element_index};
					}
				return mapping_index;
				}

			void erase_if_last(handle_t& handle)
				{
				auto container_index_being_removed{ mapping_to_inner_iterator[handle.mapping_index].inner_iterator };

				for (auto& mapping : mapping_to_inner_iterator)
					{
					if (mapping.inner_iterator == container_index_being_removed)
						{
						return;
						}
					}

				inner_container.erase(container_index_being_removed);
				}

			inner_iterator_t get_inner_iterator(const handle_t& handle)
				{
				return mapping_to_inner_iterator[handle.mapping_index].inner_iterator;
				}

			inner_container_t inner_container;
			id_pool_manual id_pool;

			struct handle_mapping_data { inner_iterator_t inner_iterator; size_t count{0}; };
			std::vector<handle_mapping_data> mapping_to_inner_iterator;
		};

	template <typename T, size_t inner_size = 8>
	using multicoso = multihandled<T, inner_size>;
	}