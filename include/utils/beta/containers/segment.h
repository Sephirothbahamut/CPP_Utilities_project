#pragma once
#include <array>
#include <vector>

namespace utils::beta::containers
	{

	namespace details
		{
		template<typename T, size_t inner_size = 8>
		struct segment_iterator;
		template<typename T, size_t inner_size = 8>
		struct segment_reverse_iterator;
		template<typename T, size_t inner_size, typename Allocator>
		struct linked_vector_iterator;
		template<typename T, size_t inner_size, typename Allocator>
		struct linked_vector_reverse_iterator;
		}

	template<typename T, size_t inner_size = 8>
	class segment
		{
		template <typename T, size_t inner_size, typename Allocator>
		friend struct details::linked_vector_iterator;
		template <typename T, size_t inner_size, typename Allocator>
		friend struct details::linked_vector_reverse_iterator;

		public:
			using value_type      =       T ;
			using pointer         =       T*;
			using const_pointer   = const T*;
			using reference       =       T&;
			using const_reference = const T&;

			using iterator               = details::segment_iterator        <      T, inner_size>;
			using const_iterator         = details::segment_iterator        <const T, inner_size>;
			using reverse_iterator       = details::segment_reverse_iterator<      T, inner_size>;
			using const_reverse_iterator = details::segment_reverse_iterator<const T, inner_size>;

			inline const segment* next_segment() const
				{
				return next;
				}

			inline segment* next_segment()
				{
				return next;
				}
				
			const_iterator         cbegin () const;
			const_iterator         begin  () const;
			iterator               begin  ();

			const_iterator         cend   () const;
			const_iterator         end    () const;
			iterator               end    ();

			const_reverse_iterator crbegin() const;
			const_reverse_iterator rbegin () const;
			reverse_iterator       rbegin ();

			const_reverse_iterator crend  () const;
			const_reverse_iterator rend   () const;
			reverse_iterator       rend   ();

		private:
			segment* prev;
			segment* next;
			size_t   size;

			std::array<T, inner_size> arr;

			template<typename, size_t, typename> friend class linked_vector;
		
		};

	namespace details
		{
		template<typename T, size_t inner_size>
		struct segment_iterator
			{
			template <typename T, size_t inner_size, typename Allocator>
			friend struct linked_vector_iterator;
			template <typename T, size_t inner_size>
			friend struct segment_iterator;

			public:
				using self_type         = segment_iterator<T, inner_size>;
				using value_type        = T  ;
				using reference         = value_type& ;
				using const_reference   = const reference;
				using pointer           = value_type* ;
				using const_pointer     = const pointer;
				using iterator_category = std::contiguous_iterator_tag ;
				using difference_type   = ptrdiff_t ;

				segment_iterator(pointer ptr) : ptr(ptr) { }

				template<typename rhs_T>
					requires std::same_as < std::remove_cv_t<T>, std::remove_cv_t<rhs_T>>
				segment_iterator(const segment_iterator<rhs_T, inner_size>& other) : ptr(const_cast<std::remove_cv_t<T>*>(other.ptr)) {} //Needed by linked vector's erase. Cause the parameters must be const iterators for some reason :|

				template<typename rhs_T>
					requires std::same_as < std::remove_cv_t<T>, std::remove_cv_t<rhs_T>>
				self_type& operator=(const segment_iterator<rhs_T, inner_size>& other)
					{
					ptr = other.ptr;
					return *this;
					}

				self_type  operator+ (difference_type rhs) const noexcept { return ptr + rhs; }
				self_type  operator- (difference_type rhs) const noexcept { return ptr - rhs; }

				template<typename rhs_T>
					requires std::same_as < std::remove_cv_t<T>, std::remove_cv_t<rhs_T>>
				difference_type operator- (const segment_iterator<rhs_T, inner_size>& rhs) const noexcept { return ptr - rhs.ptr; }

				self_type& operator+=(difference_type rhs) noexcept { *this = *this + rhs; return *this; }
				self_type& operator-=(difference_type rhs) noexcept { *this = *this - rhs; return *this; }

				self_type  operator++(   ) const noexcept { self_type i = *this; ptr++; return i; }
				self_type& operator++(int)       noexcept { ptr++; return *this; }
				self_type  operator--(   ) const noexcept { self_type i = *this; ptr--; return i; }
				self_type& operator--(int)       noexcept { ptr--; return *this; }

				const_reference operator* () const noexcept                             { return *ptr; }
					  reference operator* ()       noexcept requires(!std::is_const<T>) { return *ptr; }
				const_pointer   operator->() const noexcept                             { return  ptr; }
					  pointer   operator->()       noexcept requires(!std::is_const<T>) { return  ptr; }
			
				auto operator<=>(const self_type& rhs) const noexcept { return ptr <=> rhs.ptr; }
				bool operator== (const self_type& rhs) const noexcept { return ptr ==  rhs.ptr; }

			private:
				pointer ptr;
			};

		template<typename T, size_t inner_size>
		struct segment_reverse_iterator
			{
			template <typename T, size_t inner_size, typename Allocator>
			friend struct linked_vector_reverse_iterator;
			template <typename T, size_t inner_size>
			friend struct segment_reverse_iterator;

			public:
				using self_type         = segment_reverse_iterator<T, inner_size>;
				using value_type        = T  ;
				using reference         = value_type& ;
				using const_reference   = const reference;
				using pointer           = value_type* ;
				using const_pointer     = const pointer;
				using iterator_category = std::contiguous_iterator_tag ;
				using difference_type   = ptrdiff_t ;

				segment_reverse_iterator(pointer ptr) : ptr(ptr) { }

				template<typename rhs_T>
					requires std::same_as < std::remove_cv_t<T>, std::remove_cv_t<rhs_T>>
				segment_reverse_iterator(const segment_reverse_iterator<rhs_T, inner_size>& other) : ptr(const_cast<std::remove_cv_t<T>*>(other.ptr)) {} //Needed by linked vector's erase. Cause the parameters must be const iterators for some reason :|

				template<typename rhs_T>
					requires std::same_as < std::remove_cv_t<T>, std::remove_cv_t<rhs_T>>
				self_type& operator=(const segment_reverse_iterator<rhs_T, inner_size>& other)
					{
					ptr = other.ptr;
					return *this;
					}

				self_type  operator+ (difference_type rhs) const noexcept { return ptr - rhs; }
				self_type  operator- (difference_type rhs) const noexcept { return ptr + rhs; }
				
				template<typename rhs_T>
					requires std::same_as < std::remove_cv_t<T>, std::remove_cv_t<rhs_T>>
				difference_type operator- (const segment_reverse_iterator<rhs_T, inner_size>& rhs) const noexcept { return rhs.ptr - ptr; }

				self_type& operator+=(difference_type rhs) noexcept { *this = *this + rhs; return *this; }
				self_type& operator-=(difference_type rhs) noexcept { *this = *this - rhs; return *this; }

				self_type  operator++(   ) const noexcept { self_type i = *this; ptr--; return i; }
				self_type& operator++(int)       noexcept { *this = ptr--; return *this; }
				self_type  operator--(   ) const noexcept { self_type i = *this; ptr++; return i; }
				self_type& operator--(int)       noexcept { *this = ptr++; return *this; }

				const_reference operator* () const noexcept                             { return *ptr; }
					  reference operator* ()       noexcept requires(!std::is_const<T>) { return *ptr; }
				const_pointer   operator->() const noexcept                             { return  ptr; }
					  pointer   operator->()       noexcept requires(!std::is_const<T>) { return  ptr; }

				bool operator==(const self_type& rhs) const noexcept { return ptr == rhs.ptr; }
				bool operator!=(const self_type& rhs) const noexcept { return ptr != rhs.ptr; }
				bool operator< (const self_type& rhs) const noexcept { return ptr >  rhs.ptr; }
				bool operator> (const self_type& rhs) const noexcept { return ptr <  rhs.ptr; }
				bool operator<=(const self_type& rhs) const noexcept { return ptr >= rhs.ptr; }
				bool operator>=(const self_type& rhs) const noexcept { return ptr <= rhs.ptr; }
			private:
				pointer ptr;
			};
			
		}

		
	template<typename T, size_t inner_size> inline segment<T, inner_size>::const_iterator         segment<T, inner_size>::cbegin () const { return {arr.data()}; }
	template<typename T, size_t inner_size> inline segment<T, inner_size>::const_iterator         segment<T, inner_size>::begin  () const { return {arr.data()}; }
	template<typename T, size_t inner_size> inline segment<T, inner_size>::      iterator         segment<T, inner_size>::begin  ()       { return {arr.data()}; }

	template<typename T, size_t inner_size> inline segment<T, inner_size>::const_iterator         segment<T, inner_size>::cend   () const { return {arr.data() + inner_size}; }
	template<typename T, size_t inner_size> inline segment<T, inner_size>::const_iterator         segment<T, inner_size>::end    () const { return {arr.data() + inner_size}; }
	template<typename T, size_t inner_size> inline segment<T, inner_size>::      iterator         segment<T, inner_size>::end    ()       { return {arr.data() + inner_size}; }

	template<typename T, size_t inner_size> inline segment<T, inner_size>::const_reverse_iterator segment<T, inner_size>::crbegin() const { return {arr.data() + inner_size - 1}; }
	template<typename T, size_t inner_size> inline segment<T, inner_size>::const_reverse_iterator segment<T, inner_size>::rbegin () const { return {arr.data() + inner_size - 1}; }
	template<typename T, size_t inner_size> inline segment<T, inner_size>::      reverse_iterator segment<T, inner_size>::rbegin ()       { return {arr.data() + inner_size - 1}; }

	template<typename T, size_t inner_size> inline segment<T, inner_size>::const_reverse_iterator segment<T, inner_size>::crend  () const { return {arr.data() - 1}; }
	template<typename T, size_t inner_size> inline segment<T, inner_size>::const_reverse_iterator segment<T, inner_size>::rend   () const { return {arr.data() - 1}; }
	template<typename T, size_t inner_size> inline segment<T, inner_size>::      reverse_iterator segment<T, inner_size>::rend   ()       { return {arr.data() - 1}; }
		
	}