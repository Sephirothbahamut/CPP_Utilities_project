#pragma once
#include <ostream>
#include <array>
#include <vector>
#include <list>
#include <forward_list>
#include <set>
#include <map>
#include <string>

#include "cout_utilities.h"

namespace utils
	{
	namespace cout
		{
		template <typename FIRST, typename SECOND>
		inline ::std::ostream& operator<<(::std::ostream& os, const ::std::pair<FIRST, SECOND>& container)
			{
			namespace ccu = cout::support;
			return os << ccu::type << "std::pair" << ccu::brace << "<" << ccu::value << container.first << ccu::separ << ", " << ccu::value << container.second << ccu::brace << ">";
			}

		template <typename T>
		inline ::std::ostream& operator<<(::std::ostream& os, const ::std::vector<T>& container)
			{
			namespace ccu = cout::support;
			os << ccu::type << "std::vector" << ccu::brace << "[";
			if (!container.empty())
				{
				for (size_t i = 0; i < container.size() - 1; i++)
					{
					os << ccu::value << container[i] << ccu::separ << ", ";
					}
				os << ccu::value << container[container.size() - 1];
				}
			return os << ccu::brace << "...]";
			}

		template <typename T, size_t SIZE>
		inline ::std::ostream& operator<<(::std::ostream& os, const ::std::array<T, SIZE>& container) 
			{
			namespace ccu = cout::support;
			os << ccu::type << "std::array" << ccu::brace << "[";
			if constexpr (SIZE)
				{
				for (size_t i = 0; i < SIZE - 1; i++)
					{
					os << ccu::value << container[i] << ccu::separ << ", ";
					}
				os << ccu::value << container[container.size() - 1];
				}
			return os << ccu::brace << "]";
			}

		template <typename T>
		inline ::std::ostream& operator<<(::std::ostream& os, const ::std::list<T>& container)
			{
			namespace ccu = cout::support;
			os << ccu::type << "std::list" << ccu::brace << "[";

			auto it = container.begin();
			if (it != container.end())
				{
				while (true)
					{
					auto current = it++;
					os << ccu::value << *current;

					if (it != container.end()) { os << ccu::separ << " -><- "; }
					else { break; }
					}
				}

			return os << ccu::brace << "]";
			}

		template <typename T>
		inline ::std::ostream& operator<<(::std::ostream& os, const ::std::forward_list<T>& container) 
			{
			namespace ccu = cout::support;
			os << ccu::type << "std::forward_list" << ccu::brace << "[";

			auto it = container.begin();
			if (it != container.end())
				{
				while (true)
					{
					auto current = it++;
					os << ccu::value << *current;

					if (it != container.end()) { os << ccu::separ << " -> "; }
					else { break; }
					}
				}
			return os << ccu::brace << "]";
			}

		template <typename T>
		inline ::std::ostream& operator<<(::std::ostream& os, const ::std::set<T>& container)
			{
			namespace ccu = cout::support;
			os << ccu::type << "std::set" << ccu::brace << "{";

			auto it = container.begin();
			if (it != container.end())
				{
				while (true)
					{
					auto current = it++;
					os << ccu::value << *current;
					if (it != container.end()) { os << ccu::separ << ", "; }
					else { break; }
					}
				}
			return os << ccu::brace << "}";
			}

		template <typename KEY, typename VAL>
		inline ::std::ostream& operator<<(::std::ostream& os, const ::std::map<KEY, VAL>& container) 
			{
			namespace ccu = cout::support;
			os << ccu::type << "std::map" << ccu::brace << "[";

			auto it = container.begin();
			if (it != container.end())
				{
				while (true)
					{
					auto current = it++;

					os << ccu::brace << "(" << ccu::value << current->first << ccu::separ << ": " << ccu::value << current->second << ccu::brace << ")";

					if (it != container.end()) { os << ccu::separ << ", "; }
					else { break; }
					}
				}
			return os << ccu::brace << "]";
			}
		}
	}