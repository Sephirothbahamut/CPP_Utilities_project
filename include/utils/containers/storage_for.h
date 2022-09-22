#pragma once

#include <memory>
#include <utility>
#include <cstddef>

namespace utils::containers
	{
	template<typename T>
	class storage_for
		{
		public:
			using value_type = T;
			using reference = value_type&;
			using const_reference = const value_type&;
			using pointer = value_type*;
			using const_pointer = const value_type* const;

			storage_for() = default;

			template <typename ...Args>
			storage_for(Args&& ...args) { emplace(std::forward<Args>(args)...); }

			template <typename ...Args>
			reference emplace(Args&& ...args)
				{
				return *(new(get()) T{ std::forward<Args>(args)... });
				}

			void destroy() { value().~T(); }

			//No reason to make it private
			operator const value_type& () { return *reinterpret_cast<T*>(std::addressof(_data)); }
			operator       value_type& () { return *reinterpret_cast<T*>(std::addressof(_data)); }
		
			//Compatible interface with pointers and optional
			constexpr       reference operator*()        noexcept { return *reinterpret_cast<T*>(std::addressof(_data)); }
			constexpr const_reference operator*()  const noexcept { return *reinterpret_cast<T*>(std::addressof(_data)); }

			constexpr       pointer   operator->()       noexcept { return reinterpret_cast <T*>(std::addressof(_data)); }
			constexpr const_pointer   operator->() const noexcept { return reinterpret_cast <T*>(std::addressof(_data)); }

			constexpr       reference value()            noexcept { return *reinterpret_cast<T*>(std::addressof(_data)); }
			constexpr const_reference value()      const noexcept { return *reinterpret_cast<T*>(std::addressof(_data)); }

			constexpr       pointer   get()              noexcept { return reinterpret_cast <T*>(std::addressof(_data)); }
			constexpr const_pointer   get()        const noexcept { return reinterpret_cast <T*>(std::addressof(_data)); }
			
		private:
			alignas(T) std::byte _data[sizeof(T)];
		};
	
	}