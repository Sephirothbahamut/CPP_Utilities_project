#pragma once
#include <memory>

namespace utils
	{
	template <typename T, typename Deleter>
	std::unique_ptr<T, Deleter> make_unique_with_deleter(T* ptr) { return std::unique_ptr<T, Deleter>(ptr); }

	template <typename T>
	using observer_ptr = T*;

	template <typename T>
	struct unique_deepcopy_ptr : std::unique_ptr<T>
		{
		using std::unique_ptr<T>::unique_ptr;

		unique_deepcopy_ptr           (std::unique_ptr<T>&& unique_ptr) : std::unique_ptr<T>           {std::move(unique_ptr)} {}
		unique_deepcopy_ptr& operator=(std::unique_ptr<T>&& unique_ptr) { std::unique_ptr<T>::operator=(std::move(unique_ptr)); return *this; }

		unique_deepcopy_ptr           (const unique_deepcopy_ptr& copy) : std::unique_ptr<T>           {std::make_unique<T>(*copy)} {}
		unique_deepcopy_ptr& operator=(const unique_deepcopy_ptr& copy) { std::unique_ptr<T>::operator=(std::make_unique<T>(*copy)); }
		};
	}