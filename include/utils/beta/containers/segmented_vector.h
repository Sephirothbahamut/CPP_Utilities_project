#pragma once
#include <memory>
#include <vector>

#include "../../memory.h"
#include "segment.h"

// TODO allocator, iterators
namespace utils::beta::containers
	{
	template <typename T, size_t inner_size = 8, typename Allocator = std::allocator<T>>
	class segmented_vector
		{
		private:
			using segment_t = segment<T, inner_size>;
			
			using segment_ptr_t = utils::observer_ptr<segment_t>;
			using segment_unique_ptr_t = std::unique_ptr<segment_t>;
			using segment_allocator_t = typename std::allocator_traits<Allocator>::template rebind_alloc<segment_t>;

		public:
			using value_type      = segment_t::value_type;
			using reference       = segment_t::reference;
			using const_reference = segment_t::const_reference;
			using pointer         = segment_t::pointer;
			using const_pointer   = segment_t::const_pointer;

			struct iterator;
			struct const_iterator;
			struct reverse_iterator;
			struct const_reverse_iterator;

			segmented_vector() {}
			segmented_vector(const Allocator& allocator) : segment_allocator{allocator} {}

			~segmented_vector() { clear(); }

			inline size_t size() const
				{
				if (segment_ptr_t segment = last_segment){ return (segments_count() - 1) * inner_size + segment->size; }
				return 0;
				}
			inline size_t segments_count() const { return _segments_count; }

			inline T& front() { return first_segment->arr[0]; }
			inline T& back() { return last_segment->arr[last_segment->size - 1];  }

			inline bool empty() const { return first_segment = 0;  }
			inline void clear()
				{

				auto& disney_count{ T::count };

				if (segment_ptr_t segment_ptr = first_segment)
					{
					while (segment_ptr != last_segment)
						{
						segment_ptr_t next_segment = segment_ptr->next;
						segment_ptr->~segment_t(); //call destructor so individual elements within the array are destroyed automatically
						std::allocator_traits<segment_allocator_t>::deallocate(segment_allocator, segment_ptr, 1);
						segment_ptr = next_segment;
						}
					// last segment doesn't call the destructor so individual elements within the array are NOT destroyed automatically
					for (auto it{ segment_ptr->begin() }; it != (segment_ptr->begin() + segment_ptr->size); it++)
						{
						it->~T();
						}
					std::allocator_traits<segment_allocator_t>::deallocate(segment_allocator, segment_ptr, 1);
					first_segment = nullptr;
					last_segment  = nullptr;
					_segments_count = 0;
					}
				}

			inline T& push(const value_type& value)
				{
				auto new_t{ new(get_free_slot()) T(value) };
				last_segment->size++;
				return *new_t;
				}

			template <typename ...Args>
			inline T& emplace(Args&&... args) 
				{
				auto new_t{ new(get_free_slot()) T(std::forward<Args>(args)...) };
				last_segment->size++;
				return *new_t;
				}


			//TODO for segmented vector
			//void reserve(size_t capacity)
			//	{
			//
			//	}
			//
			//void shrink_to_fit()
			//	{
			//
			//	}
			//
			//void resize(size_t new_size)
			//	{
			//
			//	}

			void erase()
				{
					//TODO
				}
				
			const_iterator         begin () const { return {first_segment->begin()                     , first_segment}; }
			iterator               begin ()       { return {first_segment->begin()                     , first_segment}; }

			const_iterator         end   () const { return {last_segment ->begin() + last_segment->size, last_segment }; }
			iterator               end   ()       { 
				auto x{ last_segment->begin() };
				auto y{ last_segment->begin() + last_segment->size };
				return {last_segment ->begin() + last_segment->size, last_segment }; 
				}

		protected:
			std::vector<segment_unique_ptr_t> segments;

			segment_allocator_t segment_allocator ;

			pointer get_free_slot()
				{
				segment_ptr_t segment_ptr{ grow_if_full() };
				segment_t& segment{ *segment_ptr };
				return segment.arr.data() + segment.size;
				}

			segment_ptr_t grow_if_full()
				{
				if (!first_segment) { return grow_first(); }
				else if (last_segment->size == inner_size) { return grow(); }
				else { return last_segment; }
				}

			segment_ptr_t grow_first()
				{
				segment_ptr_t new_segment{ std::allocator_traits<segment_allocator_t>::allocate(segment_allocator, 1) };
				new_segment->size = 0;

				first_segment = new_segment;
				last_segment = new_segment;

				return new_segment;
				}

			segment_ptr_t grow()
				{
				segment_ptr_t new_segment{ std::allocator_traits<segment_allocator_t>::allocate(segment_allocator, 1) };
				last_segment->next = new_segment;
				new_segment->size = 0;
				new_segment->prev = last_segment;
				last_segment = new_segment;

				return new_segment;
				}
			
		};

		template<typename T, size_t inner_size, typename Allocator>
		struct segmented_vector<T, inner_size, Allocator>::iterator
			{
			public:
				using self_type = iterator;
				using value_type = T;
				using reference = T&;
				using pointer = T*;
				using iterator_category = std::random_access_iterator_tag;
				using difference_type = ptrdiff_t;

				//TODO fix operator ++, add const noexcept where needed
				iterator(segment_t::iterator segment_iterator, segment_ptr_t segment_ptr) : segment_iterator{ segment_iterator }, segment_ptr{ segment_ptr } { }

				self_type  operator+ (difference_type rhs) 
					{
					auto ret{ *this };
					
					auto to_end{ segment_ptr->end() - segment_iterator };
					if (rhs < to_end)
						{
						ret.segment_iterator += rhs;
						return ret;
						}

					auto leftover{ rhs - to_end };
					ret.segment_ptr = ret.segment_ptr->next;

					while (leftover >= inner_size)
						{
						ret.segment_ptr = ret.segment_ptr->next;
						leftover -= inner_size;
						}

					ret.segment_iterator = ret.segment_ptr->begin() + leftover;

					return ret;
					}
				self_type  operator- (difference_type rhs) 
					{
					auto ret{ *this };

					auto to_begin{ segment_iterator - segment_ptr->begin() };
					if (rhs <= to_begin)
						{
						ret.segment_iterator -= rhs;
						return ret;
						}

					auto leftover{ rhs - to_begin };
					ret.segment_ptr = ret.segment_ptr->prev;

					while (leftover > inner_size)
						{
						ret.segment_ptr = ret.segment_ptr->prev;
						leftover -= inner_size;
						}

					ret.segment_iterator = ret.segment_ptr->end() - leftover;

					return ret;
					}
				self_type& operator+=(difference_type rhs) { *this = *this + rhs; return *this; }
				self_type& operator-=(difference_type rhs) { *this = *this - rhs; return *this; }

				self_type operator++()
					{
					auto ret{ *this };
					ret.segment_iterator++;
					if (ret.segment_iterator == ret.segment_ptr->end())
						{
						ret.segment_iterator = ret.segment_ptr->next->begin();
						ret.segment_ptr = ret.segment_ptr->next;
						}
					return ret;
					}

				self_type& operator++(int) { *this = operator++(); return *this; }

				self_type operator--()
					{
					auto ret{ *this };
					if (ret.segment_iterator == ret.segment_ptr->begin())
						{
						ret.segment_iterator = ret.segment_ptr->prev->end() - 1;
						ret.segment_ptr = ret.segment_ptr->prev;
						}
					else { ret.segment_iterator--; }
					return ret;
					}

				self_type& operator--(int) { *this = operator--(); return *this; }

				const_reference operator* () const { return *segment_iterator; }
				      reference operator* ()       { return *segment_iterator; }
				const_pointer   operator->() const { return  segment_iterator.operator->(); }
				      pointer   operator->()       { return  segment_iterator.operator->(); }
				bool operator==(const self_type& rhs) const noexcept { return segment_iterator == rhs.segment_iterator; }
				bool operator!=(const self_type& rhs) const noexcept { return segment_iterator != rhs.segment_iterator; }
			private:
				segment_ptr_t segment_ptr;
				segment_t::iterator segment_iterator;
			};

		template<typename T, size_t inner_size, typename Allocator>
		struct segmented_vector<T, inner_size, Allocator>::const_iterator
			{
			public:
				using self_type         = const_iterator;
				using value_type        = T;
				using reference         = T&;
				using pointer           = T*;
				using iterator_category = std::random_access_iterator_tag;
				using difference_type   = ptrdiff_t ;

				const_iterator(segment_t::const_iterator segment_iterator, segment_ptr_t segment_ptr) : segment_iterator{ segment_iterator }, segment_ptr{ segment_ptr } { }


				self_type  operator+ (difference_type rhs)
					{
					auto ret{ *this };

					auto to_end{ segment_ptr->end() - segment_iterator };
					if (rhs < to_end)
						{
						ret.segment_iterator += rhs;
						return ret;
						}

					auto leftover{ rhs - to_end };
					ret.segment_ptr = ret.segment_ptr->next;

					while (leftover >= inner_size)
						{
						ret.segment_ptr = ret.segment_ptr->next;
						leftover -= inner_size;
						}

					ret.segment_iterator = ret.segment_ptr->begin() + leftover;

					return ret;
					}
				self_type  operator- (difference_type rhs)
					{
					auto ret{ *this };

					auto to_begin{ segment_iterator - segment_ptr->begin() };
					if (rhs <= to_begin)
						{
						ret.segment_iterator -= rhs;
						return ret;
						}

					auto leftover{ rhs - to_begin };
					ret.segment_ptr = ret.segment_ptr->prev;

					while (leftover > inner_size)
						{
						ret.segment_ptr = ret.segment_ptr->prev;
						leftover -= inner_size;
						}

					ret.segment_iterator = ret.segment_ptr->end() - leftover;

					return ret;
					}
				self_type& operator+=(difference_type rhs) { *this = *this + rhs; return *this; }
				self_type& operator-=(difference_type rhs) { *this = *this - rhs; return *this; }

				self_type& operator++()
					{
					auto ret{ *this };
					operator+=(1);
					return ret;
					}

				self_type& operator++(int) { operator++(); return *this; }

				self_type& operator--()
					{
					auto ret{ *this };
					operator-=(1);
					return ret;
					}

				self_type& operator--(int) { operator--(); return *this; }

				const reference operator* () const { return *segment_iterator; }
				const pointer   operator->() const { return  segment_iterator; }
				bool operator==(const self_type& rhs) const noexcept { return segment_iterator == rhs.segment_iterator; }
				bool operator!=(const self_type& rhs) const noexcept { return segment_iterator != rhs.segment_iterator; }
			private:
				segment_ptr_t segment_ptr;
				segment_t::const_iterator segment_iterator;
			};
	}