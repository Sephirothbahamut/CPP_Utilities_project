#pragma once
#include <memory>
#include <utility>
#include <type_traits>
#include <vector>

#include "../memory.h"
#include <array>

namespace utils::containers
	{
	template <typename T, size_t inner_size = 8, typename Allocator = std::allocator<T>>
	class segmented_vector
		{

		private:
			template<typename iter_t>
			struct base_iterator;
			template<typename iter_t>
			struct base_reverse_iterator;

#pragma region segment 
			using segment_t = std::array<T, inner_size> ;
#pragma endregion segment

			using segment_ptr_t = utils::observer_ptr<segment_t>;
			using segment_allocator_t = typename std::allocator_traits<Allocator>::template rebind_alloc<segment_t>;

#pragma region iterators

			template<typename iter_T>
			struct base_iterator
				{
				friend class segmented_vector;
				template <typename iter_T>
				friend struct segmented_vector::base_iterator;

				//using segment_ptr_t      = segmented_vector   <T, inner_size, Allocator>::segment_ptr_t;

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
					base_iterator(size_t index, segmented_vector& container, pointer element_ptr) : index{ index }, container_ptr{ &container }, element_ptr{ element_ptr } { }

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

					self_type  operator+ (difference_type rhs)
						{
						auto ret{ *this };

						ret.index += rhs;
						ret.element_ptr = ret.container_ptr->address_at(ret.index);
						return ret;
						}
					self_type  operator- (difference_type rhs)
						{
						auto ret{*this};

						ret.index -= rhs;
						ret.element_ptr = ret.container_ptr->address_at(ret.index);
						return ret;
						}

					template<typename rhs_T>
						requires std::same_as <T, std::remove_cv_t<rhs_T>>
					difference_type operator- (const base_iterator<rhs_T>& rhs) const noexcept
						{ 
						return static_cast<difference_type>(index) - static_cast<difference_type>(rhs.index);
						}

					self_type& operator+=(difference_type rhs) { *this = *this + rhs; return *this; }
					self_type& operator-=(difference_type rhs) { *this = *this - rhs; return *this; }

					self_type operator++()
						{
						auto ret{ *this };
						operator+=(1);
						return ret;
						}

					self_type& operator++(int) { operator++(); return *this; }

					self_type  operator--()
						{
						auto ret{ *this };
						operator-=(1);
						return ret;
						}

					self_type& operator--(int) { operator--(); return *this; }
					
					const_reference operator* () const noexcept                                  { return *element_ptr; }
					reference       operator* ()       noexcept requires(!std::is_const<iter_T>) { return *element_ptr; }
					const_pointer   operator->() const noexcept                                  { return  element_ptr; }
					pointer         operator->()       noexcept requires(!std::is_const<iter_T>) { return  element_ptr; }
					bool operator== (const self_type& rhs) const noexcept { return index ==  rhs.index; }
					bool operator<=>(const self_type& rhs) const noexcept { return index <=> rhs.index; }

				private:
					size_t            index;
					segmented_vector* container_ptr;
					pointer           element_ptr;
				};

			template<typename iter_T>
			struct base_reverse_iterator
				{
				friend class segmented_vector;
				template <typename iter_T>
				friend struct segmented_vector::base_reverse_iterator;
				
				//using segment_ptr_t      = segmented_vector   <T, inner_size, Allocator>::segment_ptr_t;

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
					base_reverse_iterator(ptrdiff_t index, segmented_vector& container, pointer element_ptr) : index{index}, container_ptr{&container}, element_ptr{element_ptr} { }

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

					self_type  operator+ (difference_type rhs)
						{
						auto ret{*this};

						ret.index -= rhs;
						ret.element_ptr = ret.container_ptr->address_at(ret.index);
						return ret;
						}
					self_type  operator- (difference_type rhs)
						{
						auto ret{*this};

						ret.index += rhs;
						ret.element_ptr = ret.container_ptr->address_at(ret.index);
						return ret;
						}

					template<typename rhs_T>
						requires std::same_as <T, std::remove_cv_t<rhs_T>>
					difference_type operator- (const base_reverse_iterator<rhs_T>& rhs) const noexcept
						{ 
						return static_cast<difference_type>(index) - static_cast<difference_type>(rhs.index);
						}

					self_type& operator+=(difference_type rhs) { *this = *this + rhs; return *this; }
					self_type& operator-=(difference_type rhs) { *this = *this - rhs; return *this; }

					self_type operator++()
						{
						auto ret{ *this };
						operator+=(1);
						return ret;
						}

					self_type& operator++(int) { operator++(); return *this; }

					self_type  operator--()
						{
						auto ret{ *this };
						operator-=(1);
						return ret;
						}

					self_type& operator--(int) { operator--(); return *this; }
					
					const_reference operator* () const noexcept                                  { return *element_ptr; }
					reference       operator* ()       noexcept requires(!std::is_const<iter_T>) { return *element_ptr; }
					const_pointer   operator->() const noexcept                                  { return  element_ptr; }
					pointer         operator->()       noexcept requires(!std::is_const<iter_T>) { return  element_ptr; }
					bool operator== (const self_type& rhs) const noexcept { return index ==  rhs.index; }
					bool operator<=>(const self_type& rhs) const noexcept { return index <=> rhs.index; }
				private:
					ptrdiff_t         index;
					segmented_vector* container_ptr;
					pointer           element_ptr;
				};
#pragma endregion iterators

		public:
			using value_type      = segment_t::value_type;
			using reference       = segment_t::reference;
			using const_reference = segment_t::const_reference;
			using pointer         = segment_t::pointer;
			using const_pointer   = segment_t::const_pointer;

			using iterator               = base_iterator        <      T>;
			using const_iterator         = base_iterator        <const T>;
			using reverse_iterator       = base_reverse_iterator<      T>;
			using const_reverse_iterator = base_reverse_iterator<const T>;

			segmented_vector() {}
			segmented_vector(const Allocator& allocator) : segment_allocator{allocator} {}

			~segmented_vector() { clear(); }
			
			size_t size()     const noexcept { return _size; }
			bool   empty()    const noexcept { return !size(); }
			size_t capacity() const noexcept { return segments.size() * inner_size; }

			void shrink_to_fit() noexcept { /*Done because the standard says it's fine to do nothing lol :) */ } //TODO

			inline T& front() { return first_segment().arr[0]; }
			inline T& back()  { return last_segment_ptr ().arr[last_segment_size() - 1];  } // TODO da fare modifica ricordamelo

			inline void clear()
				{
				auto segments_it{segments.begin()};
				for (; segments_it != segments.end() - 1; segments_it++)
					{
					(*segments_it)->~segment_t(); //call destructor so individual elements within the array are destroyed automatically
					std::allocator_traits<segment_allocator_t>::deallocate(segment_allocator, *segments_it, 1);
					}

				auto& last_segment_ptr{**segments_it};

				for (size_t i{0}; i < last_segment_size(); i++)
					{
					last_segment_ptr[i].~T();
					}
				std::allocator_traits<segment_allocator_t>::deallocate(segment_allocator, *segments_it, 1);

				segments.clear();
				_size = 0;
				}

			inline T& push(const value_type& value)
				{
				auto new_t{ new(get_free_slot()) T(value) };
				_size++;
				return *new_t;
				}

			template <typename ...Args>
			inline T& emplace(Args&&... args) 
				{
				auto new_t{ new(get_free_slot()) T(std::forward<Args>(args)...) };
				_size++;
				return *new_t;
				}

			iterator erase(const_iterator erase_from, const_iterator erase_to)
				{
				iterator erasing_it{ erase_from };
				iterator move_from { erase_to + 1 };
				
				while (move_from != end())
					{
					*erasing_it = std::move(*move_from);
					erasing_it++;
					move_from++;
					}
				
				iterator new_end{ erasing_it };
				
				while (erasing_it != end())
					{
					erasing_it->~T();
					erasing_it++;
					}
				
				//update size

				size_t new_last_segment_index{segment_index_containing_my_index(new_end.index)};

				for (size_t i{new_last_segment_index + 1}; i < segments.size(); i++)
					{
					std::allocator_traits<segment_allocator_t>::deallocate(segment_allocator, segments[i], 1);
					}
				segments.resize(new_last_segment_index + 1);
				_size -= erase_to - erase_from;

				return { erase_from };
				}
				
			const_iterator         cbegin () const { return {                                                         0, *this, address_at(static_cast<size_t>(0))            }; }
			const_iterator         begin  () const { return {                                                         0, *this, address_at(static_cast<size_t>(0))            }; }
			iterator               begin  ()       { return {                                                         0, *this, address_at(static_cast<size_t>(0))            }; }

 			const_iterator         cend   () const { return {size()                                                    , *this, nullptr                                       }; }
			const_iterator         end    () const { return {size()                                                    , *this, nullptr                                       }; }
			iterator               end    ()       { return {size()                                                    , *this, nullptr                                       }; }

			const_reverse_iterator crbegin() const { return {static_cast<ptrdiff_t>(size()) - static_cast<ptrdiff_t>(1), *this, address_at(static_cast<ptrdiff_t>(size() - 1))}; }
			const_reverse_iterator rbegin () const { return {static_cast<ptrdiff_t>(size()) - static_cast<ptrdiff_t>(1), *this, address_at(static_cast<ptrdiff_t>(size() - 1))}; }
			reverse_iterator       rbegin ()       { return {static_cast<ptrdiff_t>(size()) - static_cast<ptrdiff_t>(1), *this, address_at(static_cast<ptrdiff_t>(size() - 1))}; }

			const_reverse_iterator crend  () const { return {static_cast<ptrdiff_t>(                                -1), *this, nullptr                                       }; }
			const_reverse_iterator rend   () const { return {static_cast<ptrdiff_t>(                                -1), *this, nullptr                                       }; }
			reverse_iterator       rend   ()       { return {static_cast<ptrdiff_t>(                                -1), *this, nullptr                                       }; }

			const T& operator[](const size_t index) const noexcept
				{
				return segments[segment_index_containing_my_index(index)][my_index_to_segment_index(index)];
				}

			      T& operator[](const size_t index)       noexcept
				{
				return segments[segment_index_containing_my_index(index)][my_index_to_segment_index(index)];
				}

		protected:
			size_t _size{0};
			std::vector<utils::observer_ptr<segment_t>> segments;
			
			segment_allocator_t segment_allocator ;

			pointer get_free_slot()
				{
				segment_t& segment{grow_if_full()};
				return segment.data() + (size() % inner_size);
				}

			segment_t& grow_if_full()
				{
				if (size() == capacity()) { grow(); }
				return **segments.rbegin();
				}

			void grow()
				{
				segment_ptr_t new_segment{ std::allocator_traits<segment_allocator_t>::allocate(segment_allocator, 1) };
				segments.push_back(new_segment);
				}
			
			const segment_t& first_segment() const noexcept{ return *segments[0                  ]; }
			      segment_t& first_segment()       noexcept{ return *segments[0                  ]; }
			const segment_t& last_segment_ptr () const noexcept{ return *segments[segments.size() - 1]; }
			      segment_t& last_segment_ptr ()       noexcept{ return *segments[segments.size() - 1]; }

			size_t last_segment_size() const noexcept { return size() - inner_size * (segments.size() - 1); }
			
			size_t segment_index_containing_my_index(size_t index) const noexcept
				{
				return index / inner_size;
				}
			size_t my_index_to_segment_index(size_t index) const noexcept
				{
				return index % inner_size;
				}
			 
			pointer address_at(size_t index) noexcept
				{
				size_t segment_index{segment_index_containing_my_index(index)};
				if (segment_index >= segments.size()) { return nullptr; }
				else { return &(*segments[segment_index])[my_index_to_segment_index(index)]; }
				}
			const_pointer address_at(size_t index) const noexcept
				{
				size_t segment_index{segment_index_containing_my_index(index)};
				if (segment_index >= segments.size()) { return nullptr; }
				else { return &(*segments[segment_index])[my_index_to_segment_index(index)]; }
				}
			pointer address_at(ptrdiff_t  index) noexcept
				{
				size_t segment_index{segment_index_containing_my_index(index)};
				if (segment_index >= segments.size() || segment_index < 0) { return nullptr; }
				else { return &(*segments[segment_index])[my_index_to_segment_index(index)]; }
				}
			const_pointer address_at(ptrdiff_t  index) const noexcept
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
		};


	}