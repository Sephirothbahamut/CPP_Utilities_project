#pragma once
#include <vector>
#include <deque>
#include <list>
#include <set>
#include "../variadic.h"
#include "multitype_container.h"

namespace utils::containers
	{
	template <template<class> class Container_type, typename ...Types>
	class polymorphic_access_container : public multitype_container <Container_type, Types...>
		{
		public:
			using base_t = multitype_container <Container_type, Types...>;
			using base_t::multitype_container;

			template <typename msg_t, typename Function>
			void for_each_element_of_type(Function function)
				{
				for_each_containing_type<msg_t>([&](auto& container) { for (auto& element : container) { function(element); } });
				}

			template <typename msg_t, typename Function>
			void for_each_containing_type(Function function)
				{
				utils::tuple::for_each_in_tuple(base_t::containers, [&](auto& container)
					{
					if constexpr (std::is_base_of_v<msg_t, typename std::remove_reference_t<decltype(container)>::value_type>) { function(container); }
					});
				}

		protected:

			template<typename T, std::size_t index = 0>
			static constexpr std::size_t get_index_containing_type_or_derived()
				{
				if constexpr (std::is_base_of<T, typename std::tuple_element<index, base_t::tuple_t>::type::value_type>) { return index; }
				else { return get_index_containing_type_or_derived<T, index + 1>(); }
				}
		};

	}