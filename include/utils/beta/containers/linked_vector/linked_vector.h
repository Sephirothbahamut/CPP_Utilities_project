#pragma once
#include <memory>

#include "memory.h"
#include "segment.h"

// TODO allocator, iterators
namespace utils::beta::containers
	{
	template <typename T, size_t inner_size, typename Allocator>
	class linked_vector
		{
		private:
			using segment_t = segment<T, inner_size>;
			
			using segment_ptr_t       = utils::observer_ptr<segment_t>;
			using segment_allocator_t = std::allocator_traits<Allocator>::rebind_alloc<segment_t>;
		
		public:
			using value_type      = segment_t::value_type;
			using reference       = segment_t::reference;
			using const_reference = segment_t::const_reference;
			using pointer         = segment_t::pointer;
			using const_pointer   = segment_t::const_pointer;

			struct iterator;//TODO
			struct const_iterator;//TODO
			struct reverse_iterator;//TODO
			struct const_reverse_iterator;//TODO

			linked_vector(const Allocator& allocator) : segment_allocator{allocator} {}

			~linked_vector() { clear(); }

			inline size_t size() const
				{
				if (segment_ptr_t segment = last_segment){ return (segments_count() - 1) * inner_size + segment->size; }
				return 0;
				}
			inline size_t segments_count() const { return _segments_count; }

			inline T& front() { return first_segment->data[0]; }
			inline T& back () { return last_segment ->data[last_segment->size - 1] }

			inline bool empty() const { return first_segment = 0}
			inline void clear()
				{
				if (segment_ptr_t segment_ptr = first_segment)
					{
					while (segment_ptr != last_segment)
						{
						segment_ptr_t next_segment = segment->next;
						segment->~segment_t(); //call destructor so individual elements within the array are destroyed automatically
						std::allocator_traits<segment_allocator_t>::deallocate(segment_allocator, segment, 1);
						segment = next_segment;
						}
					// last segment doesn't call the destructor so individual elements within the array are NOT destroyed automatically
					for (auto it{ segment_ptr->begin() }; it != segment_ptr->begin() + segment_ptr->size; it++)
						{
						it->~T();
						}
					std::allocator_traits<segment_allocator_t>::deallocate(segment_allocator, segment_ptr, 1);
					first_segment = nullptr;
					last_segment  = nullptr;
					segment_count = 0;
					}
				}

			inline T& push(const segment_t::value_type& value)
				{
				return *new(get_free_slot()) T(value);
				}

			template <typename ...Args>
			inline T& emplace(Args&&... args) 
				{
				return *new(get_free_slot()) T(std::forward<Args>(args)...);
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
				
			const_iterator         begin () const { return const_iterator(data); }
			iterator               begin ()       { return       iterator(data); }

			const_iterator         end   () const { return const_iterator(data + inner_size); }
			iterator               end   ()       { return       iterator(data + inner_size); }

			const_reverse_iterator rbegin() const { return const_reverse_iterator(data + inner_size - 1); }
			reverse_iterator       rbegin()       { return       reverse_iterator(data + inner_size - 1); }

			const_reverse_iterator rend  () const { return const_reverse_iterator(data - 1); }
			reverse_iterator       rend  ()       { return       reverse_iterator(data - 1); }

		protected:
			segment_ptr_t  first_segment {nullptr};
			segment_ptr_t  last_segment  {nullptr};

			size_t              _segments_count{0};
			segment_allocator_t segment_allocator ;

			inline const segment_ptr_t first_segment() const { return first_segment };
			inline       segment_ptr_t first_segment()       { return first_segment };
			

			pointer get_free_slot()
				{
				segment_ptr_t segment_ptr{ grow_if_full() };
				segment_t& segment{ *segment_ptr };
				return segment.data + segment.size;
				}


			segment_ptr_t grow_if_full()
				{
				if      (!first_segment                  ) { return grow_first(); }
				else if (last_segment->size == inner_size) { return grow      (); }
				else                                       { return last_segment; }
				}

			segment_ptr_t grow_first()
				{
				segment_ptr_t new_segment{ std::allocator_traits<segment_allocator_t>::allocate(segment_allocator, 1); };
				new_segment->size = 0;

				first_segment = new_segment;
				last_segment = new_segment;
				
				return new_segment;
				}

			segment_ptr_t grow()
				{
				segment_ptr_t new_segment{ std::allocator_traits<segment_allocator_t>::allocate(segment_allocator, 1); };
				last_segment->next = new_segment;
				new_segment ->size = 0;
				return new_segment;
				}
		};
	}