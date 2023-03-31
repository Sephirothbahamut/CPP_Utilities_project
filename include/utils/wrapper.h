#pragma once

#include <memory> //addressof
#include <utility>
#include <cstddef>

//TODO alternative diamond wrapper with virtual omheritance to not pass stuff to construct the root from everywhere

namespace utils
	{
	template <typename T>
	class wrapper
		{
		public:
			using value_type      = T;
			using reference       = value_type&;
			using const_reference = const value_type&;
			using pointer         = value_type*;
			using const_pointer   = const value_type* const;

			wrapper() = default;

			template <typename ...Args>
			wrapper(Args&& ...args) : element{std::forward<Args>(args)...} {}

			//No reason to make it private
			operator const value_type& () { return element; }
			operator       value_type& () { return element; }
			value_type element;
			
			//Compatible interface with pointers and optional
			constexpr       reference operator* ()       noexcept { return element; }
			constexpr const_reference operator* () const noexcept { return element; }

			constexpr       pointer   operator->()       noexcept { return std::addressof(element); }
			constexpr const_pointer   operator->() const noexcept { return std::addressof(element); }

			constexpr       reference value     ()       noexcept { return element; }
			constexpr const_reference value     () const noexcept { return element; }

			constexpr       pointer   get       ()       noexcept { return std::addressof(element); }
			constexpr const_pointer   get       () const noexcept { return std::addressof(element); }

		protected:
		};
	}