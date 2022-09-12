#pragma once
#include <vector>
#include <array>

namespace utils::containers::memberwise_operators::concepts
	{
	template <typename T>
	concept stdarr = std::derived_from<T, std::array <typename T::value_type, std::tuple_size<T>::value>>;
	template <typename T>
	concept stdvec = std::derived_from<T, std::vector<typename T::value_type, typename T::allocator_type>>;

	template <typename T>
	concept stdvecarr = stdarr<T> || stdvec<T>;
	}