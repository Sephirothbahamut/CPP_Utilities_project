#pragma once
#include <set>
#include <map>
#include <list>
#include <array>
#include <vector>
#include <string>
#include <ostream>
#include <optional>
#include <forward_list>

#include "../console/colour.h"

namespace utils::output
	{
	namespace typeless
		{
		template <typename T>
		inline ::std::ostream& operator<<(::std::ostream& os, const ::std::optional<T>& container)
			{
			namespace ucc = utils::console::colour;
			os << ucc::brace << "(" << ucc::value;
			if (container.has_value()) 
				{
				using namespace utils::output; 
				os << (*container); 
				}
			else { os << "nullopt"; }
			return os << ucc::brace << ")";
			}

		template <typename FIRST, typename SECOND>
		inline ::std::ostream& operator<<(::std::ostream& os, const ::std::pair<FIRST, SECOND>& container)
			{
			namespace ucc = utils::console::colour;
			return os << ucc::brace << "<" << ucc::value << container.first << ucc::separ << ", " << ucc::value << container.second << ucc::brace << ">";
			}

		template <typename T>
		inline ::std::ostream& operator<<(::std::ostream& os, const ::std::vector<T>& container)
			{
			namespace ucc = utils::console::colour;
			os << ucc::brace << "[";
			if (!container.empty())
				{
				for (size_t i = 0; i < container.size() - 1; i++)
					{
					os << ucc::value << container[i] << ucc::separ << ", ";
					}
				os << ucc::value << container[container.size() - 1];
				}
			return os << ucc::brace << "...]";
			}

		template <typename T, size_t SIZE>
		inline ::std::ostream& operator<<(::std::ostream& os, const ::std::array<T, SIZE>& container) 
			{
			namespace ucc = utils::console::colour;
			os << ucc::brace << "[";
			if constexpr (static_cast<bool>(SIZE))
				{
				for (size_t i = 0; i < SIZE - 1; i++)
					{
					os << ucc::value << container[i] << ucc::separ << ", ";
					}
				os << ucc::value << container[container.size() - 1];
				}
			return os << ucc::brace << "]";
			}

		template <typename T>
		inline ::std::ostream& operator<<(::std::ostream& os, const ::std::list<T>& container)
			{
			namespace ucc = utils::console::colour;
			os << ucc::brace << "[";

			auto it = container.begin();
			if (it != container.end())
				{
				while (true)
					{
					auto current = it++;
					os << ucc::value << *current;

					if (it != container.end()) { os << ucc::separ << " -><- "; }
					else { break; }
					}
				}

			return os << ucc::brace << "]";
			}

		template <typename T>
		inline ::std::ostream& operator<<(::std::ostream& os, const ::std::forward_list<T>& container) 
			{
			namespace ucc = utils::console::colour;
			os << ucc::brace << "[";

			auto it = container.begin();
			if (it != container.end())
				{
				while (true)
					{
					auto current = it++;
					os << ucc::value << *current;

					if (it != container.end()) { os << ucc::separ << " -> "; }
					else { break; }
					}
				}
			return os << ucc::brace << "]";
			}

		template <typename T>
		inline ::std::ostream& operator<<(::std::ostream& os, const ::std::set<T>& container)
			{
			namespace ucc = utils::console::colour;
			os << ucc::brace << "{";

			auto it = container.begin();
			if (it != container.end())
				{
				while (true)
					{
					auto current = it++;
					os << ucc::value << *current;
					if (it != container.end()) { os << ucc::separ << ", "; }
					else { break; }
					}
				}
			return os << ucc::brace << "}";
			}

		template <typename KEY, typename VAL>
		inline ::std::ostream& operator<<(::std::ostream& os, const ::std::map<KEY, VAL>& container) 
			{
			namespace ucc = utils::console::colour;
			os << ucc::brace << "[";

			auto it = container.begin();
			if (it != container.end())
				{
				while (true)
					{
					auto current = it++;

					os << ucc::brace << "(" << ucc::value << current->first << ucc::separ << ": " << ucc::value << current->second << ucc::brace << ")";

					if (it != container.end()) { os << ucc::separ << ", "; }
					else { break; }
					}
				}
			return os << ucc::brace << "]";
			}
		}
	
	template <typename T>
	inline ::std::ostream& operator<<(::std::ostream& os, const ::std::optional<T>& container)
		{
		namespace ucc = utils::console::colour;
		os << ucc::type << "std::optional";
		return typeless::operator<<(os, container);
		}
		
	template <typename FIRST, typename SECOND>
	inline ::std::ostream& operator<<(::std::ostream& os, const ::std::pair<FIRST, SECOND>& container)
		{
		namespace ucc = utils::console::colour;
		os << ucc::type << "std::pair";
		return typeless::operator<<(os, container);
		}

	template <typename T>
	inline ::std::ostream& operator<<(::std::ostream& os, const ::std::vector<T>& container)
		{
		namespace ucc = utils::console::colour;
		os << ucc::type << "std::vector";
		return typeless::operator<<(os, container);
		}

	template <typename T, size_t SIZE>
	inline ::std::ostream& operator<<(::std::ostream& os, const ::std::array<T, SIZE>& container) 
		{
		namespace ucc = utils::console::colour;
		os << ucc::type << "std::array";
		return typeless::operator<<(os, container);
		}

	template <typename T>
	inline ::std::ostream& operator<<(::std::ostream& os, const ::std::list<T>& container)
		{
		namespace ucc = utils::console::colour;
		os << ucc::type << "std::list";
		return typeless::operator<<(os, container);
		}

	template <typename T>
	inline ::std::ostream& operator<<(::std::ostream& os, const ::std::forward_list<T>& container) 
		{
		namespace ucc = utils::console::colour;
		os << ucc::type << "std::forward_list";
		return typeless::operator<<(os, container);
		}

	template <typename T>
	inline ::std::ostream& operator<<(::std::ostream& os, const ::std::set<T>& container)
		{
		namespace ucc = utils::console::colour;
		os << ucc::type << "std::set";
		return typeless::operator<<(os, container);
		}

	template <typename KEY, typename VAL>
	inline ::std::ostream& operator<<(::std::ostream& os, const ::std::map<KEY, VAL>& container) 
		{
		namespace ucc = utils::console::colour;
		os << ucc::type << "std::map";
		return typeless::operator<<(os, container);
		}
	}