#pragma once

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

			const_iterator         begin () const { return const_iterator(data); }
			iterator               begin ()       { return       iterator(data); }

			const_iterator         end   () const { return const_iterator(data + inner_size); }
			iterator               end   ()       { return       iterator(data + inner_size); }

			const_reverse_iterator rbegin() const { return const_reverse_iterator(data + inner_size - 1); }
			reverse_iterator       rbegin()       { return       reverse_iterator(data + inner_size - 1); }

			const_reverse_iterator rend  () const { return const_reverse_iterator(data - 1); }
			reverse_iterator       rend  ()       { return       reverse_iterator(data - 1); }

		private:
			union
				{
				segment* next;
				size_t   size;
				};

			segment* prev;

			std::array<T, inner_size> data;

			template<typename, size_t, typename> friend class segmented_vector;
			template<typename, size_t, typename> friend struct segmented_vector_iterator;
		
		};

	template<typename T, size_t inner_size>
	struct segment<T, inner_size>::iterator
		{
		public:
			using self_type         = iterator ;
			using value_type        = T  ;
			using reference         = T& ;
			using pointer           = T* ;
			using iterator_category = std::forward_iterator_tag ;
			using difference_type   = int ;

			iterator(pointer ptr) : ptr(ptr) { }

			self_type operator++()    { self_type i = *this; ptr++; return i; }
			self_type operator++(int) { ptr++; return *this; }
			reference operator* ()    { return *ptr; }
			pointer   operator->()    { return  ptr; }
			bool operator==(const self_type& rhs) { return ptr == rhs.ptr; }
			bool operator!=(const self_type& rhs) { return ptr != rhs.ptr; }
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
			using iterator_category = std::forward_iterator_tag ;
			using difference_type   = int ;

			const_iterator(pointer ptr) : ptr(ptr) { }

			self_type operator++() { self_type i = *this; ptr++; return i; }
			self_type operator++(int) { ptr++; return *this; }
			const reference operator* () { return *ptr; }
			const pointer   operator->() { return  ptr; }
			bool operator==(const self_type& rhs) { return ptr == rhs.ptr; }
			bool operator!=(const self_type& rhs) { return ptr != rhs.ptr; }
		private:
			pointer ptr;
		};

	template<typename T, size_t inner_size>
	struct segment<T, inner_size>::reverse_iterator
		{
		public:
			using self_type         = reverse_iterator;
			using value_type        = T  ;
			using reference         = T& ;
			using pointer           = T* ;
			using iterator_category = std::forward_iterator_tag ;
			using difference_type   = int ;

			reverse_iterator(pointer ptr) : ptr(ptr) { }

			self_type operator--()    { self_type i = *this; ptr--; return i; }
			self_type operator--(int) { ptr--; return *this; }
			reference operator* ()    { return *ptr; }
			pointer   operator->()    { return  ptr; }
			bool operator==(const self_type& rhs) { return ptr == rhs.ptr; }
			bool operator!=(const self_type& rhs) { return ptr != rhs.ptr; }
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
			using iterator_category = std::forward_iterator_tag ;
			using difference_type   = int ;

			const_reverse_iterator(pointer ptr) : ptr(ptr) { }

			self_type operator--() { self_type i = *this; ptr--; return i; }
			self_type operator--(int) { ptr--; return *this; }
			const reference operator* () { return *ptr; }
			const pointer   operator->() { return  ptr; }
			bool operator==(const self_type& rhs) { return ptr == rhs.ptr; }
			bool operator!=(const self_type& rhs) { return ptr != rhs.ptr; }
		private:
			pointer ptr;
		};
		
	}