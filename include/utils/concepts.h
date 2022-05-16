#pragma once

#include <concepts>

namespace utils::concepts
	{
	template <typename A, typename B>
	concept undecorated_same_as = std::same_as<std::remove_cv_t<A>, std::remove_cv_t<B>>;

	template<typename T, typename ... U>
	concept any_of = (std::same_as<T, U> || ...);
	template<typename T, typename ... U>
	concept undecorated_any_of = (undecorated_same_as<T, U> || ...);
	}