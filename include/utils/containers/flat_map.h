#pragma once

#include "flat_set.h"

//TODO INSERTION CHECK
//TODO finish this
/*namespace utils::containers
	{
	template <typename Key, typename T, class Allocator = std::allocator<std::pair<Key, T>>>
	class tiny_map
		{
		protected:
			using inner_container_t = std::vector<std::pair<Key, T>, Allocator>;
			using pair_t = std::pair<Key, T>;

		public:
			using value_type             = inner_container_t::value_type;
			using size_type              = inner_container_t::size_type;
			using reference              = inner_container_t::reference;
			using const_reference        = inner_container_t::const_reference;
			using pointer                = inner_container_t::pointer;
			using const_pointer          = inner_container_t::const_pointer;
			using iterator               = inner_container_t::iterator;
			using const_iterator         = inner_container_t::const_iterator;
			using reverse_iterator       = inner_container_t::reverse_iterator;
			using const_reverse_iterator = inner_container_t::const_reverse_iterator;

			template <typename ...Args>
			T& emplace(Args&& ...args)
				{
				return inner_container.emplace_back(std::forward<Args>(args)...);
				}

			//template <typename ...Args>
			//void insert(Args& ...args)
			//	{
			//	inner_container.push_back(args...);
			//	}

			      iterator find(const T& search)       noexcept { return std::find( begin(),  end(), search); }
			const_iterator find(const T& search) const noexcept { return std::find(cbegin(), cend(), search); }
			void remove(const T& search) noexcept { inner_container.erase(std::remove(begin(), end(), search)); }

			void clear() { inner_container.clear(); }
			size_t size() const noexcept { return inner_container.size(); }
			bool empty() const noexcept { return inner_container.empty(); }

			const auto begin()   const noexcept { return inner_container.begin(); }
			      auto begin()         noexcept { return inner_container.begin(); }
			const auto end()     const noexcept { return inner_container.end(); }
			      auto end()           noexcept { return inner_container.end(); }
			const auto cbegin()  const noexcept { return inner_container.cbegin(); }
			      auto cbegin()        noexcept { return inner_container.cbegin(); }
			const auto cend()    const noexcept { return inner_container.cend(); }
			      auto cend()          noexcept { return inner_container.cend(); }
			const auto rbegin()  const noexcept { return inner_container.rbegin(); }
			      auto rbegin()        noexcept { return inner_container.rbegin(); }
			const auto rend()    const noexcept { return inner_container.rend(); }
			      auto rend()          noexcept { return inner_container.rend(); }
			const auto crbegin() const noexcept { return inner_container.crbegin(); }
			      auto crbegin()       noexcept { return inner_container.crbegin(); }
			const auto crend()   const noexcept { return inner_container.crend(); }
			      auto crend()         noexcept { return inner_container.crend(); }
		
		protected:
			inner_container_t inner_container;
		};
	}*/