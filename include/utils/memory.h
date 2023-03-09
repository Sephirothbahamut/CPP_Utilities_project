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

	/// <summary> Non rebindable alternative to std::reference_wrapper. Meant mostly for strictly mimicing a T& inside containers. For any other use prefer std::reference_wrapper. </summary>
	template <typename T>
	class reference
		{
		public:
			using value_type = T;

			reference(value_type& ref) noexcept : ptr{&ref} {}

			constexpr operator value_type () const noexcept { return *ptr; }
			constexpr operator value_type&()       noexcept { return *ptr; }

			constexpr reference& operator=(const value_type& value) noexcept { (*ptr) = value; return *this; }

		private:
			utils::observer_ptr<value_type> ptr;
		};

	namespace concepts
		{
		template <typename T>
		concept reference = std::same_as<T, utils::reference<typename T::value_type>> || std::same_as<T, std::reference_wrapper<typename T::value_type>>;
		}
	}