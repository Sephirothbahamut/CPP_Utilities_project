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
	class linked_vector
		{

		private:
			template<typename iter_t>
			struct base_iterator;
			template<typename iter_t>
			struct base_reverse_iterator;

#pragma region segment 
			class segment_t
				{
				friend class linked_vector;
				template<typename T>
				friend struct linked_vector::base_iterator;
				template<typename T>
				friend struct linked_vector::base_reverse_iterator;

				private:
#pragma region iterators
					template <typename iter_T>
					struct base_iterator
						{
						template <typename iter_T>
						friend struct linked_vector::base_iterator;
						template <typename iter_T>
						friend struct base_iterator;
						friend class linked_vector;

						public:
							using self_type         = base_iterator<iter_T>;
							using value_type        = iter_T  ;
							using reference         = value_type& ;
							using const_reference   = const reference;
							using pointer           = value_type* ;
							using const_pointer     = const pointer;
							using iterator_category = std::contiguous_iterator_tag ;
							using difference_type   = ptrdiff_t ;

							base_iterator(pointer ptr) : ptr(ptr) { }

							template<typename rhs_T>
								requires std::same_as < T, std::remove_cv_t<rhs_T>>
							base_iterator(const base_iterator<rhs_T>& other) : ptr(const_cast<T*>(other.ptr)) {} //Needed by linked vector's erase. Cause the parameters must be const iterators for some reason :|

							template<typename rhs_T>
								requires std::same_as < T, std::remove_cv_t<rhs_T>>
							self_type& operator=(const base_iterator<rhs_T>& other)
								{
								ptr = other.ptr;
								return *this;
								}

							self_type  operator+ (difference_type rhs) const noexcept { return ptr + rhs; }
							self_type  operator- (difference_type rhs) const noexcept { return ptr - rhs; }

							template<typename rhs_T>
								requires std::same_as < T, std::remove_cv_t<rhs_T>>
							difference_type operator- (const base_iterator<rhs_T>& rhs) const noexcept { return ptr - rhs.ptr; }

							self_type& operator+=(difference_type rhs) noexcept { *this = *this + rhs; return *this; }
							self_type& operator-=(difference_type rhs) noexcept { *this = *this - rhs; return *this; }

							self_type  operator++(   ) noexcept { self_type i = *this; ptr++; return i; }
							self_type& operator++(int) noexcept { ptr++; return *this; }
							self_type  operator--(   ) noexcept { self_type i = *this; ptr--; return i; }
							self_type& operator--(int) noexcept { ptr--; return *this; }

							const_reference operator* () const noexcept                             { return *ptr; }
								  reference operator* ()       noexcept requires(!std::is_const<iter_T>) { return *ptr; }
							const_pointer   operator->() const noexcept                             { return  ptr; }
								  pointer   operator->()       noexcept requires(!std::is_const<iter_T>) { return  ptr; }
			
							auto operator<=>(const self_type& rhs) const noexcept { return ptr <=> rhs.ptr; }
							bool operator== (const self_type& rhs) const noexcept { return ptr ==  rhs.ptr; }

						private:
							pointer ptr;
						};
					
					template<typename iter_T>
					struct base_reverse_iterator
						{
						template <typename iter_T>
						friend struct linked_vector::base_reverse_iterator;
						template <typename iter_T>
						friend struct base_reverse_iterator;
						friend class linked_vector;

						public:
							using self_type         = base_reverse_iterator<iter_T>;
							using value_type        = iter_T  ;
							using reference         = value_type& ;
							using const_reference   = const reference;
							using pointer           = value_type* ;
							using const_pointer     = const pointer;
							using iterator_category = std::contiguous_iterator_tag ;
							using difference_type   = ptrdiff_t ;

							base_reverse_iterator(pointer ptr) : ptr(ptr) { }

							template<typename rhs_T>
								requires std::same_as < T, std::remove_cv_t<rhs_T>>
							base_reverse_iterator(const base_reverse_iterator<rhs_T>& other) : ptr(const_cast<T*>(other.ptr)) {} //Needed by linked vector's erase. Cause the parameters must be const iterators for some reason :|

							template<typename rhs_T>
								requires std::same_as < T, std::remove_cv_t<rhs_T>>
							self_type& operator=(const base_reverse_iterator<rhs_T>& other)
								{
								ptr = other.ptr;
								return *this;
								}

							self_type  operator+ (difference_type rhs) const noexcept { return ptr - rhs; }
							self_type  operator- (difference_type rhs) const noexcept { return ptr + rhs; }
				
							template<typename rhs_T>
								requires std::same_as < T, std::remove_cv_t<rhs_T>>
							difference_type operator- (const base_reverse_iterator<rhs_T>& rhs) const noexcept { return rhs.ptr - ptr; }

							self_type& operator+=(difference_type rhs) noexcept { *this = *this + rhs; return *this; }
							self_type& operator-=(difference_type rhs) noexcept { *this = *this - rhs; return *this; }

							self_type  operator++(   ) noexcept { self_type i = *this; ptr--; return i; }
							self_type& operator++(int) noexcept { *this = ptr--; return *this; }
							self_type  operator--(   ) noexcept { self_type i = *this; ptr++; return i; }
							self_type& operator--(int) noexcept { *this = ptr++; return *this; }

							const_reference operator* () const noexcept                             { return *ptr; }
								  reference operator* ()       noexcept requires(!std::is_const<iter_T>) { return *ptr; }
							const_pointer   operator->() const noexcept                             { return  ptr; }
								  pointer   operator->()       noexcept requires(!std::is_const<iter_T>) { return  ptr; }

							bool operator==(const self_type& rhs) const noexcept { return ptr == rhs.ptr; }
							bool operator!=(const self_type& rhs) const noexcept { return ptr != rhs.ptr; }
							bool operator< (const self_type& rhs) const noexcept { return ptr >  rhs.ptr; }
							bool operator> (const self_type& rhs) const noexcept { return ptr <  rhs.ptr; }
							bool operator<=(const self_type& rhs) const noexcept { return ptr >= rhs.ptr; }
							bool operator>=(const self_type& rhs) const noexcept { return ptr <= rhs.ptr; }

						private:
							pointer ptr;
						};
#pragma endregion iterators

				public:
					using value_type      =       T ;
					using pointer         =       T*;
					using const_pointer   = const T*;
					using reference       =       T&;
					using const_reference = const T&;

					using iterator               = base_iterator        <      T>;
					using const_iterator         = base_iterator        <const T>;
					using reverse_iterator       = base_reverse_iterator<      T>;
					using const_reverse_iterator = base_reverse_iterator<const T>;
				
					const_iterator         cbegin () const { return {arr.data()}; }
					const_iterator         begin  () const { return {arr.data()}; }
					      iterator         begin  ()       { return {arr.data()}; }
					
					const_iterator         cend   () const { return {arr.data() + inner_size}; }
					const_iterator         end    () const { return {arr.data() + inner_size}; }
					      iterator         end    ()       { return {arr.data() + inner_size}; }
					
					const_reverse_iterator crbegin() const { return {arr.data() + inner_size - 1}; }
					const_reverse_iterator rbegin () const { return {arr.data() + inner_size - 1}; }
					      reverse_iterator rbegin ()       { return {arr.data() + inner_size - 1}; }
					
					const_reverse_iterator crend  () const { return {arr.data() - 1}; }
					const_reverse_iterator rend   () const { return {arr.data() - 1}; }
					      reverse_iterator rend   ()       { return {arr.data() - 1}; }

				private:
					std::array<T, inner_size> arr;
					utils::observer_ptr<segment_t> next;
					utils::observer_ptr<segment_t> prev;
					size_t size;
				};
#pragma endregion segment

			using segment_ptr_t = utils::observer_ptr<segment_t>;
			using segment_allocator_t = typename std::allocator_traits<Allocator>::template rebind_alloc<segment_t>;

#pragma region iterators

			template<typename iter_T>
			struct base_iterator
				{
				friend class linked_vector;
				template <typename iter_T>
				friend struct linked_vector::base_iterator;
				
				using segment_iterator_t = linked_vector   <T, inner_size, Allocator>::segment_t::base_iterator<iter_T>;
				using segment_ptr_t      = linked_vector   <T, inner_size, Allocator>::segment_ptr_t;

				public:
					using self_type         = base_iterator<iter_T>;
					using value_type        = iter_T  ;
					using reference         = value_type& ;
					using const_reference   = const reference;
					using pointer           = value_type* ;
					using const_pointer     = const pointer;
					using iterator_category = std::random_access_iterator_tag;
					using difference_type   = ptrdiff_t ;

					base_iterator() : segment_iterator{ nullptr }, segment_ptr{ nullptr } { }
					base_iterator(segment_iterator_t segment_iterator, const segment_ptr_t segment_ptr) : segment_iterator{ segment_iterator }, segment_ptr{ segment_ptr } { }

					template<typename rhs_T>
						requires std::same_as < T, std::remove_cv_t<rhs_T>>
					base_iterator(const base_iterator<rhs_T>& other) : segment_iterator{ other.segment_iterator }, segment_ptr{ other.segment_ptr } {}

					template<typename rhs_T>
						requires std::same_as < T, std::remove_cv_t<rhs_T>>
					self_type& operator=(const base_iterator<rhs_T>& other)
						{
						segment_iterator = other.segment_iterator;
						segment_ptr      = other.segment_ptr;
						return *this;
						}

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

						while(true)
							{
							if (!ret.segment_ptr->next) { return { ret.segment_ptr->end(), ret.segment_ptr }; }

							ret.segment_ptr = ret.segment_ptr->next;
							if (leftover < inner_size) { break; }
							
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

					template<typename rhs_T>
						requires std::same_as <T, std::remove_cv_t<rhs_T>>
					difference_type operator- (const base_iterator<rhs_T>& rhs) const noexcept
						{ 
						if (segment_ptr == rhs.segment_ptr)
							{
							return segment_iterator - rhs.segment_iterator;
							}

						auto a{ this->segment_ptr };
						auto b{ rhs  .segment_ptr };

						size_t iterations{ 1 };
						while(a->prev)
							{
							a = a->prev;

							if (a == rhs.segment_ptr)
								{
								difference_type a_to_begin{ this->segment_iterator   - this->segment_ptr->begin() };
								difference_type b_to_end  { rhs  .segment_ptr->end() - rhs  .segment_iterator     };
								return  static_cast<difference_type>(a_to_begin + iterations + b_to_end);
								}
							iterations++;
							}
							
						iterations = 1;
						while (b->prev)
							{
							b = b->prev;

							if (b == this->segment_ptr)
								{
								difference_type a_to_end  { this->segment_ptr->end() - this->segment_iterator};
								difference_type b_to_begin{ rhs  .segment_iterator   - rhs.segment_ptr->begin()  };
								return -static_cast<difference_type>(a_to_end + iterations + b_to_begin);
								}
							iterations++;
							}
						return {}; // this should not happen!
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
					
					const_reference operator* () const noexcept                                  { return *segment_iterator; }
					reference       operator* ()       noexcept requires(!std::is_const<iter_T>) { return *segment_iterator; }
					const_pointer   operator->() const noexcept                                  { return  segment_iterator.operator->(); }
					pointer         operator->()       noexcept requires(!std::is_const<iter_T>) { return  segment_iterator.operator->(); }
					bool operator==(const self_type& rhs) const noexcept 
						{
						if (!segment_ptr || !rhs.segment_ptr) { return segment_ptr == rhs.segment_ptr; }
						return segment_iterator == rhs.segment_iterator; 
						}
					bool operator!=(const self_type& rhs) const noexcept { return !(segment_iterator == rhs.segment_iterator); }
					bool operator< (const self_type& rhs) const noexcept 
						{ 
						if (segment_ptr == rhs.segment_ptr) { return segment_iterator < rhs.segment_iterator; }

						segment_ptr_t this_backward{ segment_ptr }, rhs_backward{ rhs.segment_ptr };
						while (true)
							{
							this_backward = this_backward->prev;
							rhs_backward  = rhs_backward ->prev;
							if (!this_backward) { return true  ;}
							if (!rhs_backward ) { return false ;}
							}
						}
					bool operator> (const self_type& rhs) const noexcept
						{
						if (segment_ptr == rhs.segment_ptr) { return segment_iterator > rhs.segment_iterator; }

						segment_ptr_t this_backward{ segment_ptr }, rhs_backward{ rhs.segment_ptr };
						while (true)
							{
							this_backward = this_backward->prev;
							rhs_backward  = rhs_backward ->prev;
							if (!this_backward) { return false ;}
							if (!rhs_backward ) { return true  ;}
							}
						}
				private:
					segment_iterator_t segment_iterator;
					segment_ptr_t      segment_ptr;
				};

			template<typename iter_T>
			struct base_reverse_iterator
				{
				friend class linked_vector;
				template <typename iter_T>
				friend struct linked_vector::base_reverse_iterator;
				
				using segment_iterator_t = linked_vector   <T, inner_size, Allocator>::segment_t::base_reverse_iterator<iter_T>;
				using segment_ptr_t      = linked_vector   <T, inner_size, Allocator>::segment_ptr_t;

				public:
					using self_type         = base_reverse_iterator<iter_T>;
					using value_type        = iter_T  ;
					using reference         = value_type& ;
					using const_reference   = const reference;
					using pointer           = value_type* ;
					using const_pointer     = const pointer;
					using iterator_category = std::random_access_iterator_tag;
					using difference_type   = ptrdiff_t ;

					base_reverse_iterator() : segment_iterator{ nullptr }, segment_ptr{ nullptr } { }
					base_reverse_iterator(segment_iterator_t segment_iterator, const segment_ptr_t segment_ptr) : segment_iterator{ segment_iterator }, segment_ptr{ segment_ptr } { }

					template<typename rhs_T>
						requires std::same_as < T, std::remove_cv_t<rhs_T>>
					base_reverse_iterator(const base_reverse_iterator<rhs_T>& other) : segment_iterator{ other.segment_iterator }, segment_ptr{ other.segment_ptr } {}

					template<typename rhs_T>
						requires std::same_as < T, std::remove_cv_t<rhs_T>>
					self_type& operator=(const base_reverse_iterator<rhs_T>& other)
						{
						segment_iterator = other.segment_iterator;
						segment_ptr      = other.segment_ptr;
						return *this;
						}

					self_type  operator+ (difference_type rhs)
						{
						auto ret{ *this };

						auto to_end{ segment_ptr->rend() - segment_iterator };
						if (rhs < to_end)
							{
							ret.segment_iterator += rhs;
							return ret;
							}

						auto leftover{ rhs - to_end };

						while (true)
							{
							if (!ret.segment_ptr->prev) { return { ret.segment_ptr->rend(), ret.segment_ptr }; }

							ret.segment_ptr = ret.segment_ptr->prev;
							if (leftover < inner_size) { break; }

							leftover -= inner_size;
							}

						ret.segment_iterator = ret.segment_ptr->rbegin() + leftover;

						return ret;
						}
					self_type  operator- (difference_type rhs)
						{
						auto ret{ *this };

						auto to_begin{ segment_iterator - segment_ptr->rbegin() };
						if (rhs <= to_begin)
							{
							ret.segment_iterator -= rhs;
							return ret;
							}

						auto leftover{ rhs - to_begin };
						ret.segment_ptr = ret.segment_ptr->next;

						while (leftover > inner_size)
							{
							ret.segment_ptr = ret.segment_ptr->next;
							leftover -= inner_size;
							}

						ret.segment_iterator = ret.segment_ptr->rend() - leftover;

						return ret;
						}

					template<typename rhs_T>
						requires std::same_as <T, std::remove_cv_t<rhs_T>>
					difference_type operator- (const base_reverse_iterator<rhs_T>& rhs) const noexcept
						{ 
						if (segment_ptr == rhs.segment_ptr)
							{
							return segment_iterator - rhs.segment_iterator;
							}

						auto a{ this->segment_ptr };
						auto b{ rhs  .segment_ptr };

						size_t iterations{ 1 };
						while(a->next)
							{
							a = a->next;

							if (a == rhs.segment_ptr)
								{
								difference_type a_to_begin{ this->segment_iterator   - this->segment_ptr->rbegin() };
								difference_type b_to_end  { rhs  .segment_ptr->rend() - rhs  .segment_iterator     };
								return  static_cast<difference_type>(a_to_begin + iterations + b_to_end);
								}
							iterations++;
							}
							
						iterations = 1;
						while (b->next)
							{
							b = b->next;

							if (b == this->segment_ptr)
								{
								difference_type a_to_end  { this->segment_ptr->rend() - this->segment_iterator};
								difference_type b_to_begin{ rhs  .segment_iterator   - rhs.segment_ptr->rbegin()  };
								return -static_cast<difference_type>(a_to_end + iterations + b_to_begin);
								}
							iterations++;
							}
						return {}; // this should not happen!
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
					
					const_reference operator* () const noexcept                                  { return *segment_iterator; }
					reference       operator* ()       noexcept requires(!std::is_const<iter_T>) { return *segment_iterator; }
					const_pointer   operator->() const noexcept                                  { return  segment_iterator.operator->(); }
					pointer         operator->()       noexcept requires(!std::is_const<iter_T>) { return  segment_iterator.operator->(); }
					bool operator==(const self_type& rhs) const noexcept
						{
						if (!segment_ptr || !rhs.segment_ptr) { return segment_ptr == rhs.segment_ptr; }
						return segment_iterator == rhs.segment_iterator;
						}
					bool operator!=(const self_type& rhs) const noexcept { return !(segment_iterator == rhs.segment_iterator); }
					bool operator< (const self_type& rhs) const noexcept 
						{ 
						if (segment_ptr == rhs.segment_ptr) { return segment_iterator < rhs.segment_iterator; }

						segment_ptr_t this_backward{ segment_ptr }, rhs_backward{ rhs.segment_ptr };
						while (true)
							{
							this_backward = this_backward->next;
							rhs_backward  = rhs_backward ->next;
							if (!this_backward) { return true  ;}
							if (!rhs_backward ) { return false ;}
							}
						}
					bool operator> (const self_type& rhs) const noexcept
						{
						if (segment_ptr == rhs.segment_ptr) { return segment_iterator > rhs.segment_iterator; }

						segment_ptr_t this_backward{ segment_ptr }, rhs_backward{ rhs.segment_ptr };
						while (true)
							{
							this_backward = this_backward->next;
							rhs_backward  = rhs_backward ->next;
							if (!this_backward) { return false ;}
							if (!rhs_backward ) { return true  ;}
							}
						}
				private:
					segment_iterator_t segment_iterator;
					segment_ptr_t      segment_ptr;
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

			linked_vector() {}
			linked_vector(const Allocator& allocator) : segment_allocator{allocator} {}

			~linked_vector() { clear(); }

			inline size_t capacity() const noexcept { return _segments_count * inner_size; }
			inline size_t size()     const noexcept
				{
				if (segment_ptr_t segment = last_segment_ptr){ return (segments_count() - 1) * inner_size + segment->size; }
				return 0;
				}

			inline T& front() { return first_segment->arr[0]; }
			inline T& back()  { return last_segment_ptr ->arr[last_segment_ptr->size - 1];  }

			inline bool empty() const { return first_segment == nullptr;  }
			inline void clear()
				{
				if (segment_ptr_t segment_ptr = first_segment)
					{
					while (segment_ptr != last_segment_ptr)
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
					last_segment_ptr  = nullptr;
					_segments_count = 0;
					}
				}

			inline T& push(const value_type& value)
				{
				auto new_t{ new(get_free_slot()) T(value) };
				last_segment_ptr->size++;
				return *new_t;
				}

			template <typename ...Args>
			inline T& emplace(Args&&... args) 
				{
				auto new_t{ new(get_free_slot()) T(std::forward<Args>(args)...) };
				last_segment_ptr->size++;
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
				auto new_last_segment_ptr{ new_end.segment_ptr };
				
				while (new_last_segment_ptr != last_segment_ptr)
					{
					auto delete_segment_ptr{ last_segment_ptr };
					last_segment_ptr = last_segment_ptr->prev;
					std::allocator_traits<segment_allocator_t>::deallocate(segment_allocator, delete_segment_ptr, 1);
					}
				new_last_segment_ptr->size = new_end.segment_iterator - new_last_segment_ptr->begin();
				new_last_segment_ptr->next = nullptr;
				
				return { erase_from };
				}
				
			const_iterator         cbegin () const { if (first_segment) { return { first_segment->cbegin ()                                    , first_segment }; } else { return {}; } }
			const_iterator         begin  () const { if (first_segment) { return { first_segment->cbegin ()                                    , first_segment }; } else { return {}; } }
			iterator               begin  ()       { if (first_segment) { return { first_segment->begin  ()                                    , first_segment }; } else { return {}; } }


 			const_iterator         cend   () const { if (first_segment) { return { last_segment_ptr ->cbegin () +  last_segment_ptr->size              , last_segment_ptr  }; } else { return {}; } }
			const_iterator         end    () const { if (first_segment) { return { last_segment_ptr ->cbegin () +  last_segment_ptr->size              , last_segment_ptr  }; } else { return {}; } }
			iterator               end    ()       { if (first_segment) { return { last_segment_ptr ->begin  () +  last_segment_ptr->size              , last_segment_ptr  }; } else { return {}; } }

			const_reverse_iterator crbegin() const { if (first_segment) { return { last_segment_ptr ->crbegin() + (inner_size - last_segment_ptr->size), last_segment_ptr  }; } else { return {}; } }
			const_reverse_iterator rbegin () const { if (first_segment) { return { last_segment_ptr ->crbegin() + (inner_size - last_segment_ptr->size), last_segment_ptr  }; } else { return {}; } }
			reverse_iterator       rbegin ()       { if (first_segment) { return { last_segment_ptr ->rbegin () + (inner_size - last_segment_ptr->size), last_segment_ptr  }; } else { return {}; } }

			const_reverse_iterator crend  () const { if (first_segment) { return { first_segment->crend  ()                                    , first_segment }; } else { return {}; } }
			const_reverse_iterator rend   () const { if (first_segment) { return { first_segment->crend  ()                                    , first_segment }; } else { return {}; } }
			reverse_iterator       rend   ()       { if (first_segment) { return { first_segment->rend   ()                                    , first_segment }; } else { return {}; } }

		protected:
			segment_ptr_t  first_segment {nullptr};
			segment_ptr_t  last_segment_ptr  {nullptr};

			size_t              _segments_count{0};
			segment_allocator_t segment_allocator ;

			inline size_t segments_count() const { return _segments_count; }

			inline const segment_ptr_t get_first() const { return first_segment; }
			inline       segment_ptr_t get_first()       { return first_segment; }

			inline const segment_ptr_t get_last () const { return last_segment_ptr ; }
			inline       segment_ptr_t get_last ()       { return last_segment_ptr ; }

			pointer get_free_slot()
				{
				segment_ptr_t segment_ptr{ grow_if_full() };
				segment_t& segment{ *segment_ptr };
				return segment.arr.data() + segment.size;
				}

			segment_ptr_t grow_if_full()
				{
				if (!first_segment) { return grow_first(); }
				else if (last_segment_ptr->size == inner_size) { return grow(); }
				else { return last_segment_ptr; }
				}

			segment_ptr_t grow_first()
				{
				segment_ptr_t new_segment{ std::allocator_traits<segment_allocator_t>::allocate(segment_allocator, 1) };
				new_segment->size = 0;
				new_segment->prev = nullptr;
				new_segment->next = nullptr;

				first_segment = new_segment;
				last_segment_ptr = new_segment;

				return new_segment;
				}

			segment_ptr_t grow()
				{
				segment_ptr_t new_segment{ std::allocator_traits<segment_allocator_t>::allocate(segment_allocator, 1) };
				last_segment_ptr->next = new_segment;
				new_segment->size = 0;
				new_segment->prev = last_segment_ptr;
				new_segment->next = nullptr;
				last_segment_ptr = new_segment;

				return new_segment;
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