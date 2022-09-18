#pragma once

#include <utility>

#include "try_opt.h"

namespace utils::construct
	{
	//TODO expose construction related try stuff from try_opt.h

	// https://stackoverflow.com/questions/38440844/constructor-arguments-from-a-tuple
	template <class T, class Tuple, size_t... Is>
	T from_tuple(Tuple&& tuple, std::index_sequence<Is...>) 
		{
		return T{std::get<Is>(std::forward<Tuple>(tuple))...};
		}

	template <class T, class Tuple>
	T from_tuple(Tuple&& tuple) 
		{
		return from_tuple<T>(
			std::forward<Tuple>(tuple),
			std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
		}
	}