#pragma once
#include <memory>
#include <concepts>

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

		constexpr unique_deepcopy_ptr           (std::unique_ptr<T>&& unique_ptr) : std::unique_ptr<T>           {std::move(unique_ptr)} {}
		constexpr unique_deepcopy_ptr& operator=(std::unique_ptr<T>&& unique_ptr) { std::unique_ptr<T>::operator=(std::move(unique_ptr)); return *this; }

		constexpr unique_deepcopy_ptr           (const unique_deepcopy_ptr& copy) : std::unique_ptr<T>           {std::make_unique<T>(*copy)} {}
		constexpr unique_deepcopy_ptr& operator=(const unique_deepcopy_ptr& copy) { std::unique_ptr<T>::operator=(std::make_unique<T>(*copy)); return *this; }
		};

	namespace concepts
		{
		template <typename T>
		concept reference = std::same_as<T, std::reference_wrapper<typename T::type>>;
		}

	template <typename T>
	struct remove_cvref
		{
		using type = std::remove_cvref_t<T>;
		//type operator()(const T& input) { return std::static_cast<type>(input); } //TODO when MSVC implements static operator()
		type cast(const T& input) { return static_cast<type>(input); }
		};

	template <concepts::reference T>
	struct remove_cvref<T>
		{
		using type = std::remove_cvref_t<typename T::type>;
		//type operator()(const T& input) { return std::static_cast<type>(input); } //TODO when MSVC implements static operator()
		type cast(const T& input) { return static_cast<type>(input); }
		};

	template <typename T>
	using remove_cvref_t = typename remove_cvref<T>::type;
	}