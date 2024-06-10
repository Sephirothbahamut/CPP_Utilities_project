#pragma once
#include <memory>
#include <concepts>

#include "compilation/gpu.h"

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
		concept reference_wrapper = std::same_as<std::remove_cvref_t<T>, std::reference_wrapper<typename T::type>>;
		template <typename T>
		concept const_reference_wrapper = reference_wrapper<T> && std::is_const_v<typename T::type>;

		template <typename T>
		concept reference_raw = std::is_reference_v<T>;
		template <typename T>
		concept const_reference_raw = std::is_reference_v<T> && std::is_const_v<T>;

		template <typename T>
		concept reference = reference_wrapper<T> || reference_raw<T>;
		template <typename T>
		concept const_reference = const_reference_wrapper<T> || const_reference_raw<T>;

		template <typename T>
		concept const_value = std::is_const_v<T> || const_reference_wrapper<T>;
		}
	static_assert(concepts::reference<std::reference_wrapper<float>>);

	template <typename T>
	struct remove_reference
		{
		using type = std::remove_reference_t<T>;
		//type operator()(const T& input) { return std::static_cast<type>(input); } //TODO when MSVC implements static operator()
		utils_gpu_available static constexpr type cast(const T& input) { return static_cast<type>(input); }
		};

	template <concepts::reference_wrapper T>
	struct remove_reference<T>
		{
		using type = typename T::type;
		//type operator()(const T& input) { return std::static_cast<type>(input); } //TODO when MSVC implements static operator()
		utils_gpu_available static constexpr type cast(const T& input) { return static_cast<type>(input); }
		};

	template <typename T>
	using remove_reference_t = typename remove_reference<T>::type;

	template <typename T>
	struct remove_const_reference
		{
		using type = std::remove_cvref_t<T>;
		//type operator()(const T& input) { return std::static_cast<type>(input); } //TODO when MSVC implements static operator()
		utils_gpu_available static constexpr type cast(const T& input) { return static_cast<type>(input); }
		};

	template <concepts::reference_wrapper T>
	struct remove_const_reference<T>
		{
		using type = std::remove_cvref_t<typename T::type>;
		//type operator()(const T& input) { return std::static_cast<type>(input); } //TODO when MSVC implements static operator()
		utils_gpu_available static constexpr type cast(const T& input) { return static_cast<type>(input); }
		};

	template <typename T>
	using remove_const_reference_t = typename remove_const_reference<T>::type;

	template <typename T>
	utils_gpu_available constexpr auto remove_reference_v(T& in) noexcept -> remove_reference_t<T>&
		{
		return static_cast<remove_reference_t<T>&>(in);
		}
	}