#pragma once
#include <vector>

namespace utils::vector
	{
	template<typename T>
	typename std::vector<T>::iterator
		insert_sorted(std::vector<T>& vec, T const& item)
		{
		return vec.insert(std::upper_bound(vec.begin(), vec.end(), item), item);
		}

	template<typename T, typename Pred>
	typename std::vector<T>::iterator
		insert_sorted(std::vector<T>& vec, T const& item, Pred pred)
		{
		return vec.insert(std::upper_bound(vec.begin(), vec.end(), item, pred), item);
		}
	}