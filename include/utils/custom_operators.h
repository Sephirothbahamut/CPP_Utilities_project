#pragma once

#include <concepts>
#include "function_traits.h"

namespace utils
	{
	template <class T/*, std::enable_if<std::tuple_size_v<ltl::function_arg_types<T>> == 2>*/>
	struct custom_operator
		{
		/*using ret_t = ltl::function_return_type_t<T>;
		using argst = ltl::function_arg_types<T>;
		using lhs_t = std::tuple_element<0, argst>;
		using rhs_t = std::tuple_element<1, argst>;*/

		T func;

		constexpr custom_operator(T func) : func(func) {}

		template <class U>
		struct _proxy {
			T func;
			U lhs;
			constexpr auto operator>(auto const& rhs) const -> decltype(auto) {
				return func(lhs, rhs);
				}
			};

		template <class U>
		constexpr friend _proxy<U> operator<(U const& lhs, custom_operator const& op)
			{
			return _proxy<U> {.func = op.func, .lhs = lhs };
			}
		};

	
	}