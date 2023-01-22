#pragma once

#include <tuple>
#include <algorithm>
#include "concepts.h"

namespace utils
	{
	namespace template_wrapper
		{
		template <size_t N>
		struct string 
			{
			constexpr string(const char(&str)[N]) 
				{
				std::copy_n(str, N, value);
				}
			static constexpr std::size_t size = N - 1;
			char value[N];
			};

		template <string S1, string S2>
		consteval auto concat() 
			{
			return []<std::size_t... Is, std::size_t... Js>(std::index_sequence<Is...>, std::index_sequence<Js...>) 
					{
					constexpr char const buff[S1.size + S2.size + 1]{S1.value[Is]..., S2.value[Js]..., '\0'};
					return template_string<S1.size + S2.size + 1>(buff);
					}
				(std::make_index_sequence<S1.size>{}, std::make_index_sequence<S2.size>{});
			}
		}
	}