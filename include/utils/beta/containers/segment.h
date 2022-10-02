#pragma once
#include <array>
#include <vector>

namespace utils::beta::containers
	{
	template<typename T, size_t inner_size = 8>
	class segment
		{
		public:
			using value_type      =       T ;
			using pointer         =       T*;
			using const_pointer   = const T*;
			using reference       =       T&;
			using const_reference = const T&;

			struct iterator;
			struct const_iterator;
			struct reverse_iterator;
			struct const_reverse_iterator;

			inline const segment* next_segment() const
				{
				return next;
				}

			inline segment* next_segment()
				{
				return next;
				}

			const_iterator         begin () const;
			iterator               begin ();

			const_iterator         end   () const;
			iterator               end   ();

			const_reverse_iterator rbegin() const;
			reverse_iterator       rbegin();

			const_reverse_iterator rend  () const;
			reverse_iterator       rend  ();

		private:
			union
				{
				segment* next;
				size_t   size;
				};

			segment* prev;

			std::array<T, inner_size> arr;

			template<typename, size_t, typename> friend class linked_vector;
		
		};

	template<typename T, size_t inner_size>
	struct segment<T, inner_size>::iterator
		{
		public:
			using self_type         = iterator ;
			using value_type        = T  ;
			using reference         = T& ;
			using pointer           = T* ;
			using iterator_category = std::contiguous_iterator_tag ;
			using difference_type   = ptrdiff_t ;

			iterator(pointer ptr) : ptr(ptr) { }

			self_type  operator+ (difference_type rhs) const noexcept { return ptr + rhs; }
			self_type  operator- (difference_type rhs) const noexcept { return ptr - rhs; }

			difference_type operator+ (const self_type& rhs) const noexcept { return ptr + rhs.ptr; }
			difference_type operator- (const self_type& rhs) const noexcept { return ptr - rhs.ptr; }

			self_type& operator+=(difference_type rhs) noexcept { *this = *this + rhs; return *this; }
			self_type& operator-=(difference_type rhs) noexcept { *this = *this - rhs; return *this; }

			self_type  operator++()    const noexcept { self_type i = *this; ptr++; return i; }
			self_type& operator++(int)       noexcept { ptr++; return *this; }
			self_type  operator--()    const noexcept { self_type i = *this; ptr--; return i; }
			self_type& operator--(int)       noexcept { ptr--; return *this; }

			const_reference operator* () const noexcept { return *ptr; }
			      reference operator* ()       noexcept { return *ptr; }
			const_pointer   operator->() const noexcept { return  ptr; }
			      pointer   operator->()       noexcept { return  ptr; }
			
			bool operator==(const self_type& rhs) const noexcept { return ptr == rhs.ptr; }
			bool operator!=(const self_type& rhs) const noexcept { return ptr != rhs.ptr; }
		private:
			pointer ptr;
		};

	template<typename T, size_t inner_size>
	struct segment<T, inner_size>::const_iterator
		{
		public:
			using self_type         = const_iterator ;
			using value_type        = T  ;
			using reference         = T& ;
			using pointer           = T* ;
			using iterator_category = std::contiguous_iterator_tag;
			using difference_type   = ptrdiff_t;

			const_iterator(const pointer ptr) : ptr(ptr) { }

			self_type  operator+ (difference_type rhs) const noexcept { return ptr + rhs; }
			self_type  operator- (difference_type rhs) const noexcept { return ptr - rhs; }

			difference_type  operator+ (const self_type& rhs) const noexcept { return ptr + rhs.ptr; }
			difference_type  operator- (const self_type& rhs) const noexcept { return ptr - rhs.ptr; }

			self_type& operator+=(difference_type rhs) noexcept { *this = *this + rhs; return *this; }
			self_type& operator-=(difference_type rhs) noexcept { *this = *this - rhs; return *this; }

			self_type  operator++()    const noexcept { self_type i = *this; ptr++; return i; }
			self_type& operator++(int)       noexcept { ptr++; return *this; }
			self_type  operator--()    const noexcept { self_type i = *this; ptr--; return i; }
			self_type& operator--(int)       noexcept { ptr--; return *this; }

			const_reference operator* () const noexcept { return *ptr; }
			const_pointer   operator->() const noexcept { return  ptr; }

			bool operator==(const self_type& rhs) const noexcept { return ptr == rhs.ptr; }
			bool operator!=(const self_type& rhs) const noexcept { return ptr != rhs.ptr; }
		private:
			const pointer ptr;
		};

	template<typename T, size_t inner_size>
	struct segment<T, inner_size>::reverse_iterator
		{
		public:
			using self_type         = reverse_iterator;
			using value_type        = T  ;
			using reference         = T& ;
			using pointer           = T* ;
			using iterator_category = std::contiguous_iterator_tag;
			using difference_type   = ptrdiff_t ;

			reverse_iterator(pointer ptr) : ptr(ptr) { }

			self_type  operator+ (difference_type rhs) const noexcept { return ptr - rhs; }
			self_type  operator- (difference_type rhs) const noexcept { return ptr + rhs; }

			difference_type operator+ (const self_type& rhs) const noexcept { return ptr - rhs.ptr; }
			difference_type operator- (const self_type& rhs) const noexcept { return ptr + rhs.ptr; }

			self_type& operator+=(difference_type rhs) noexcept { *this = *this - rhs; return *this; }
			self_type& operator-=(difference_type rhs) noexcept { *this = *this + rhs; return *this; }

			self_type  operator++()    const noexcept { self_type i = *this; ptr--; return i; }
			self_type& operator++(int)       noexcept { ptr--; return *this; }
			self_type  operator--()    const noexcept { self_type i = *this; ptr++; return i; }
			self_type& operator--(int)       noexcept { ptr++; return *this; }

			const_reference operator* () const noexcept { return *ptr; }
			      reference operator* ()       noexcept { return *ptr; }
			const_pointer   operator->() const noexcept { return  ptr; }
			      pointer   operator->()       noexcept { return  ptr; }

			bool operator==(const self_type& rhs) const noexcept { return ptr == rhs.ptr; }
			bool operator!=(const self_type& rhs) const noexcept { return ptr != rhs.ptr; }
		private:
			pointer ptr;
		};

	template<typename T, size_t inner_size>
	struct segment<T, inner_size>::const_reverse_iterator
		{
		public:
			using self_type         = const_reverse_iterator;
			using value_type        = T  ;
			using reference         = T& ;
			using pointer           = T* ;
			using iterator_category = std::contiguous_iterator_tag;
			using difference_type   = ptrdiff_t ;

			const_reverse_iterator(const pointer ptr) : ptr(ptr) { }

			self_type  operator+ (difference_type rhs) const noexcept { return ptr - rhs; }
			self_type  operator- (difference_type rhs) const noexcept { return ptr + rhs; }

			difference_type operator+ (const self_type& rhs) const noexcept { return ptr - rhs.ptr; }
			difference_type operator- (const self_type& rhs) const noexcept { return ptr + rhs.ptr; }

			self_type& operator+=(difference_type rhs) noexcept { *this = *this - rhs; return *this; }
			self_type& operator-=(difference_type rhs) noexcept { *this = *this + rhs; return *this; }

			self_type  operator++()    const noexcept { self_type i = *this; ptr--; return i; }
			self_type& operator++(int)       noexcept { ptr--; return *this; }
			self_type  operator--()    const noexcept { self_type i = *this; ptr++; return i; }
			self_type& operator--(int)       noexcept { ptr++; return *this; }

			const_reference operator* () const noexcept { return *ptr; }
			const_pointer   operator->() const noexcept { return  ptr; }

			bool operator==(const self_type& rhs) const noexcept { return ptr == rhs.ptr; }
			bool operator!=(const self_type& rhs) const noexcept { return ptr != rhs.ptr; }
		private:
			const pointer ptr;
		};


		
	template<typename T, size_t inner_size> inline segment<T, inner_size>::const_iterator         segment<T, inner_size>::begin () const { return {arr.data()}; }
	template<typename T, size_t inner_size> inline segment<T, inner_size>::      iterator         segment<T, inner_size>::begin ()       { return {arr.data()}; }

	template<typename T, size_t inner_size> inline segment<T, inner_size>::const_iterator         segment<T, inner_size>::end   () const { return {arr.data() + inner_size}; }
	template<typename T, size_t inner_size> inline segment<T, inner_size>::      iterator         segment<T, inner_size>::end   ()       { return {arr.data() + inner_size}; }

	template<typename T, size_t inner_size> inline segment<T, inner_size>::const_reverse_iterator segment<T, inner_size>::rbegin() const { return {arr.data() + inner_size - 1}; }
	template<typename T, size_t inner_size> inline segment<T, inner_size>::      reverse_iterator segment<T, inner_size>::rbegin()       { return {arr.data() + inner_size - 1}; }

	template<typename T, size_t inner_size> inline segment<T, inner_size>::const_reverse_iterator segment<T, inner_size>::rend  () const { return {arr.data() - 1}; }
	template<typename T, size_t inner_size> inline segment<T, inner_size>::      reverse_iterator segment<T, inner_size>::rend  ()       { return {arr.data() - 1}; }
		
	}