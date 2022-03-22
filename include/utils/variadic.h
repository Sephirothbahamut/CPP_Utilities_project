#pragma once

#include <memory>
#include <type_traits>
#include <tuple>

namespace utils
	{
	namespace tuple
		{
		template<typename... Ts, typename F>
		void for_each_in_tuple(std::tuple<Ts...>& t, F f)
			{
			std::apply([=](auto& ...x) { (..., f(x)); }, t);
			}
		}

	namespace variadic
		{
		template <typename Container_T, Container_T search_for, Container_T element, Container_T... rest>
		struct container_contains_value
			{
			constexpr static bool value = search_for == element &&
				container_contains_value<Container_T, search_for, rest...>::value;
			};

		template <typename Container_T, Container_T search_for, Container_T element>
		struct container_contains_value<Container_T, search_for, element>
			{
			constexpr static bool value = search_for == element;
			};

		template <typename msg_t, typename Current, typename... R>
		struct contains_type
			{
			constexpr static bool value = std::is_same<msg_t, Current>::value || contains_type<msg_t, R...>::value;
			};

		template <typename msg_t, typename Current>
		struct contains_type<msg_t, Current>
			{
			constexpr static bool value = std::is_same<msg_t, Current>::value;
			};

		template <typename Child, typename Parent, typename... R>
		struct contains_child_of
			{
			constexpr static bool value = std::is_base_of<Parent, Child>::value &&
				(std::is_convertible<Child, R>::value && ...); // Is that the intent?
			};

		template <typename Child, typename Parent>
		struct contains_child_of<Child, Parent>
			{
			constexpr static bool value = std::is_base_of<Parent, Child>::value;
			};
		}


	template <typename T, typename Object>
	inline constexpr auto contains_child_of_type(Object& object) { return std::is_base_of_v<T, Object::value_type>; }


	template<typename Container_T>
	struct is_unique_ptr : std::false_type {};

	template<typename Container_T, typename D>
	struct is_unique_ptr<std::unique_ptr<Container_T, D>> : std::true_type {};


	template<typename Container_T>
	struct true_contained_type { using type = Container_T; };

	template<typename Container_T, typename D>
	struct true_contained_type<std::unique_ptr<Container_T, D>> { using type = Container_T; };
	}