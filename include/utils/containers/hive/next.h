#pragma once
#include <memory>
#include <utility>
#include <type_traits>
#include <vector>

#include "../../memory.h"
#include <array>

//TODO complete iterators

namespace utils::containers::hive
	{
	template <typename T, size_t inner_size = 8, typename Allocator = std::allocator<T>>
	class next
		{
		private:
			union slot_t
				{
				T element; size_t next_free;

				//these are needed because the inner_container's array of slot_t needs to know an explicit constructor and destructor for slot_t when slot_t contains a non-trivially constructible/destructible type
				slot_t() {}
				~slot_t() {}
				};
			template<typename iter_t>
			struct base_iterator;
			template<typename iter_t>
			struct base_reverse_iterator;

#pragma region segment 
			using segment_t = std::array<slot_t, inner_size> ;
#pragma endregion segment

			using segment_ptr_t = utils::observer_ptr<segment_t>;
			using segment_allocator_t = typename std::allocator_traits<Allocator>::template rebind_alloc<segment_t>;

#pragma region iterators

			template<typename iter_T>
			struct base_iterator
				{
				friend class next;
				template <typename iter_T>
				friend struct next::base_iterator;

				//using segment_ptr_t      = next   <slot_t, inner_size, Allocator>::segment_ptr_t;

				public:
					using self_type         = base_iterator<iter_T>;
					using value_type        = iter_T  ;
					using reference         = value_type& ;
					using const_reference   = const reference;
					using pointer           = value_type* ;
					using const_pointer     = const pointer;
					using iterator_category = std::random_access_iterator_tag;
					using difference_type   = ptrdiff_t ;

					base_iterator() : container_ptr{nullptr} {}
					base_iterator(size_t index, next& container, pointer element_ptr) : index{ index }, container_ptr{ &container }, element_ptr{ element_ptr } { }

					template<typename rhs_T>
						requires std::same_as < T, std::remove_cv_t<rhs_T>>
					base_iterator(const base_iterator<rhs_T>& other) : index{other.index}, container_ptr{other.container_ptr}, element_ptr{const_cast<pointer>(other.element_ptr)} {}

					template<typename rhs_T>
						requires std::same_as < T, std::remove_cv_t<rhs_T>>
					self_type& operator=(const base_iterator<rhs_T>& other)
						{
						index         = other.index;
						container_ptr = other.container_ptr;
						element_ptr   = other.element_ptr;
						return *this;
						}

					template<typename rhs_T>
						requires std::same_as <T, std::remove_cv_t<rhs_T>>
					difference_type operator- (const base_iterator<rhs_T>& rhs) const noexcept
						{ 
						return static_cast<difference_type>(index) - static_cast<difference_type>(rhs.index);
						}

					template<typename rhs_T>
						requires std::same_as < slot_t, std::remove_cv_t<rhs_T>>
					self_type& operator=(const base_iterator<rhs_T>& other)
						{
						index         = other.index;
						container_ptr = other.container_ptr;
						element_ptr   = other.element_ptr;
						return *this;
						}

					const_reference operator* () const noexcept                                  { return *element_ptr; }
					reference       operator* ()       noexcept requires(!std::is_const<iter_T>) { return *element_ptr; }
					const_pointer   operator->() const noexcept                                  { return  element_ptr; }
					pointer         operator->()       noexcept requires(!std::is_const<iter_T>) { return  element_ptr; }
					bool operator== (const self_type& rhs) const noexcept { return index ==  rhs.index; }
					bool operator<=>(const self_type& rhs) const noexcept { return index <=> rhs.index; }

				private:
					size_t  index;
					next*   container_ptr;
					pointer element_ptr;
				};

			template<typename iter_T>
			struct base_reverse_iterator
				{
				friend class next;
				template <typename iter_T>
				friend struct next::base_reverse_iterator;
				
				//using segment_ptr_t      = next   <slot_t, inner_size, Allocator>::segment_ptr_t;

				public:
					using self_type         = base_reverse_iterator<iter_T>;
					using value_type        = iter_T  ;
					using reference         = value_type& ;
					using const_reference   = const reference;
					using pointer           = value_type* ;
					using const_pointer     = const pointer;
					using iterator_category = std::random_access_iterator_tag;
					using difference_type   = ptrdiff_t ;

					base_reverse_iterator() : container_ptr{nullptr} {}
					base_reverse_iterator(ptrdiff_t index, next& container, pointer element_ptr) : index{index}, container_ptr{&container}, element_ptr{element_ptr} { }

					template<typename rhs_T>
						requires std::same_as < T, std::remove_cv_t<rhs_T>>
					base_reverse_iterator(const base_reverse_iterator<rhs_T>& other) : index{other.index}, container_ptr{other.container_ptr}, element_ptr{ const_cast<pointer>(other.element_ptr) } {}

					template<typename rhs_T>
						requires std::same_as < T, std::remove_cv_t<rhs_T>>
					self_type& operator=(const base_reverse_iterator<rhs_T>& other)
						{
						index         = other.index;
						container_ptr = other.container_ptr;
						element_ptr   = other.element_ptr;
						return *this;
						}

					template<typename rhs_T>
						requires std::same_as <T, std::remove_cv_t<rhs_T>>
					difference_type operator- (const base_reverse_iterator<rhs_T>& rhs) const noexcept
						{ 
						return static_cast<difference_type>(index) - static_cast<difference_type>(rhs.index);
						}

					const_reference operator* () const noexcept                                  { return *element_ptr; }
					reference       operator* ()       noexcept requires(!std::is_const<iter_T>) { return *element_ptr; }
					const_pointer   operator->() const noexcept                                  { return  element_ptr; }
					pointer         operator->()       noexcept requires(!std::is_const<iter_T>) { return  element_ptr; }
					bool operator== (const self_type& rhs) const noexcept { return index ==  rhs.index; }
					bool operator<=>(const self_type& rhs) const noexcept { return index <=> rhs.index; }
				private:
					ptrdiff_t index;
					next*     container_ptr;
					pointer   element_ptr;
				};
#pragma endregion iterators

		public:
			using value_type      = segment_t::value_type;
			using reference       = segment_t::reference;
			using const_reference = segment_t::const_reference;
			using pointer         = segment_t::pointer;
			using const_pointer   = segment_t::const_pointer;

			using size_type       = size_t;

			using iterator               = base_iterator        <      T>;
			using const_iterator         = base_iterator        <const T>;
			using reverse_iterator       = base_reverse_iterator<      T>;
			using const_reverse_iterator = base_reverse_iterator<const T>;

			next() {}
			next(const Allocator& allocator) : segment_allocator{allocator} {}

			~next() { clear(); }
			
			size_t size()     const noexcept { return _size; }
			bool   empty()    const noexcept { return !size(); }
			size_t capacity() const noexcept { return segments.size() * inner_size; }

			void shrink_to_fit() noexcept { /*Done because the standard says it's fine to do nothing lol :) */ } //TODO

			inline void clear()
				{
				//Detructions
				size_t free{first_free};

				for (size_t i{0}; i < capacity(); i++)
					{
					if (i != free)
						{
						auto& slot{*address_at(first_free)};
						slot.element.~T();
						}
					else { free = address_at(free)->next_free; }
					}


				//Deallocate segments
				auto segments_it{segments.begin()};
				for (; segments_it != segments.end(); segments_it++)
					{
					std::allocator_traits<segment_allocator_t>::deallocate(segment_allocator, *segments_it, 1);
					}
				segments.clear();

				_size = 0;
				}

			template <typename ...Args>
			inline iterator emplace(Args&&... args) 
				{
				auto index{first_free};
				auto& slot{ get_free_slot() };
				auto previous_next_free{ slot.next_free };

				try
					{
					new(&slot.element) T{ std::forward<Args>(args)... };
					}
				catch (...)
					{
					slot.next_free = previous_next_free;
					throw;
					}

				_size++;

				first_free = previous_next_free;
				return {index, *this, &(address_at(static_cast<size_t>(index))->element) };
				}

			inline iterator push(const value_type& value)
				{
				return emplace(value);
				}

			void erase(const_iterator erased)
				{
				auto& my_slot{ *address_at(erased.index) };
				my_slot.element.~T();

				size_t* last_free_before_me{ &first_free };

				//Increase until the first empty slot which next_free is greater than index
				while (erased.index > *last_free_before_me) { last_free_before_me = &(address_at(*last_free_before_me)->next_free); }

				auto previous_free{ *last_free_before_me };
				*last_free_before_me = erased.index;

				my_slot.next_free = previous_free;

				_size--;
				}

			//TODO make iterable
			//Note: iterators to a valid element will need to know the next_free index so on ++ increment they can skip over it.
			//Thus iterators are invalidated not only on erase operations, but also on emplace operations
			//const_iterator         cbegin () const { return {                                                         0, *this, address_at(static_cast<size_t>(0))            }; }
			//const_iterator         begin  () const { return {                                                         0, *this, address_at(static_cast<size_t>(0))            }; }
			//iterator               begin  ()       { return {                                                         0, *this, address_at(static_cast<size_t>(0))            }; }
			//
 			//const_iterator         cend   () const { return {size()                                                    , *this, nullptr                                       }; }
			//const_iterator         end    () const { return {size()                                                    , *this, nullptr                                       }; }
			//iterator               end    ()       { return {size()                                                    , *this, nullptr                                       }; }
			//
			//const_reverse_iterator crbegin() const { return {static_cast<ptrdiff_t>(size()) - static_cast<ptrdiff_t>(1), *this, address_at(static_cast<ptrdiff_t>(size() - 1))}; }
			//const_reverse_iterator rbegin () const { return {static_cast<ptrdiff_t>(size()) - static_cast<ptrdiff_t>(1), *this, address_at(static_cast<ptrdiff_t>(size() - 1))}; }
			//reverse_iterator       rbegin ()       { return {static_cast<ptrdiff_t>(size()) - static_cast<ptrdiff_t>(1), *this, address_at(static_cast<ptrdiff_t>(size() - 1))}; }
			//
			//const_reverse_iterator crend  () const { return {static_cast<ptrdiff_t>(                                -1), *this, nullptr                                       }; }
			//const_reverse_iterator rend   () const { return {static_cast<ptrdiff_t>(                                -1), *this, nullptr                                       }; }
			//reverse_iterator       rend   ()       { return {static_cast<ptrdiff_t>(                                -1), *this, nullptr                                       }; }

		protected:
			std::vector<utils::observer_ptr<segment_t>> segments;
			size_t first_free{ 0 };
			size_t last_free { 0 };
			size_t _size     { 0 };
			
			segment_allocator_t segment_allocator ;

			slot_t& get_free_slot()
				{
				grow_if_full();
				return *address_at(first_free);
				}

			void grow_if_full()
				{
				if (first_free >= capacity()) { grow(); }
				}

			void grow()
				{
				segment_ptr_t new_segment_ptr{ std::allocator_traits<segment_allocator_t>::allocate(segment_allocator, 1) };
				segments.push_back(new_segment_ptr);

				auto& new_segment{*new_segment_ptr };

				for (size_t i{ 0 }; i < inner_size; i++)
					{
					new_segment[i].next_free = i + 1;
					}
				last_free = capacity();
				}
			
			const segment_t& first_segment() const noexcept{ return *segments[0                  ]; }
			      segment_t& first_segment()       noexcept{ return *segments[0                  ]; }
			const segment_t& last_segment () const noexcept{ return *segments[segments.size() - 1]; }
			      segment_t& last_segment ()       noexcept{ return *segments[segments.size() - 1]; }
			
			size_t segment_index_containing_my_index(size_t index) const noexcept
				{
				return index / inner_size;
				}
			size_t my_index_to_segment_index(size_t index) const noexcept
				{
				return index % inner_size;
				}

			slot_t* address_at(size_t index) noexcept
				{
				size_t segment_index{segment_index_containing_my_index(index)};
				if (segment_index >= segments.size()) { return nullptr; }
				else { return &(*segments[segment_index])[my_index_to_segment_index(index)]; }
				}
			slot_t const* address_at(size_t index) const noexcept
				{
				size_t segment_index{segment_index_containing_my_index(index)};
				if (segment_index >= segments.size()) { return nullptr; }
				else { return &(*segments[segment_index])[my_index_to_segment_index(index)]; }
				}
			slot_t address_at(ptrdiff_t  index) noexcept
				{
				size_t segment_index{segment_index_containing_my_index(index)};
				if (segment_index >= segments.size() || segment_index < 0) { return nullptr; }
				else { return &(*segments[segment_index])[my_index_to_segment_index(index)]; }
				}
			slot_t const* address_at(ptrdiff_t  index) const noexcept
				{
				size_t segment_index{segment_index_containing_my_index(index)};
				if (segment_index >= segments.size() || segment_index < 0) { return nullptr; }
				else { return &(*segments[segment_index])[my_index_to_segment_index(index)]; }
				}

			static size_t distance(segment_ptr_t leftmost, segment_ptr_t rightmost)
				{
				segment_ptr_t curr_segment_ptr{ rightmost };
				size_t segment_distance{ 0 };

				while (curr_segment_ptr != leftmost)
					{
					curr_segment_ptr = curr_segment_ptr->prev;
					segment_distance++;
					}

				return segment_distance;
				}

				
			const slot_t& operator[](const size_t index) const noexcept
				{
				return segments[segment_index_containing_my_index(index)][my_index_to_segment_index(index)];
				}

			      slot_t& operator[](const size_t index)       noexcept
				{
				return segments[segment_index_containing_my_index(index)][my_index_to_segment_index(index)];
				}
		};


	}