#pragma once

#include <type_traits>
#include <tuple>

namespace utils
	{
	namespace tuple
		{

		template<typename... Ts, typename F>
		void for_each_in_tuple(std::tuple<Ts...> const& t, F f)
			{
			std::apply([](auto& ...x) { (..., F(x)); }, t);
			}
		}

	namespace variadic
		{
		template <typename T, T search_for, T element, T... rest>
		struct contains_value
			{
			constexpr static bool value = search_for == element &&
				contains_value<T, search_for, rest...>::value;
			};
		namespace //private
			{
			template <typename T, T search_for, T element>
			struct contains_value<T, search_for, element>
				{
				constexpr static bool value = search_for == element;
				};

			template <typename Child, typename Parent, typename... R>
			struct contains_child_of
				{
				constexpr static bool value = std::is_base_of<Parent, Child>::value &&
					contains_child_of<Child, R...>::value;
				};
			namespace //private
				{
				template <typename Child, typename Parent>
				struct contains_child_of<Child, Parent>
					{
					constexpr static bool value = std::is_base_of<Parent, Child>::value;
					};
				}
			}
		}
	}