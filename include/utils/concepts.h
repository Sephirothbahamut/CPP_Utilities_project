#pragma once

#include <type_traits>
#include <concepts>

namespace utils::concepts
	{
	template <typename whatever>
	void unimplemented_specialization() 
		{
		//Thanks Alice from the #include discord
		static_assert(sizeof(whatever) && false, "Required template/concept specialization missing"); 
		}

	template <typename A, typename B>
	concept same_as_undecorated = std::same_as<std::remove_cvref_t<A>, std::remove_cvref_t<B>>;

	template <typename A, typename B>
	concept derived_from_undecorated = std::derived_from<std::remove_cv_t<A>, std::remove_cv_t<B>>;

	template <typename T>
	concept non_const_iterable = requires(T t)
		{
				{ *t.begin() } -> std::same_as<typename T::value_type>;
		};

	template<typename T, typename ... U>
	concept any_of = (std::same_as<T, U> || ...);
	template<typename T, typename ... U>
	concept any_of_undecorated = (same_as_undecorated<T, U> || ...);

	template <typename T>
	concept trivially_copyable      = requires(T t) { std::is_trivially_copyable_v     <T>; };

	template <typename T>
	concept trivially_constructible = requires(T t) { std::is_trivially_constructible_v<T>; };

	template <typename T>
	concept default_constructible   = requires(T t) { std::is_default_constructible_v  <T>; };

	template <typename T>
	concept iterable_non_const = requires(T t)
		{
				{ *t.begin() } -> std::same_as<typename T::value_type&>;
				{ *t.end  () } -> std::same_as<typename T::value_type&>;
				{  t.size () } -> std::same_as<size_t>;
		};

	template <typename T>
	concept iterable = requires(T t)
		{
				{ *t.begin() } -> same_as_undecorated<typename T::value_type>;
				{ *t.end  () } -> same_as_undecorated<typename T::value_type>;
				{  t.size () } -> std::same_as<size_t>;
		};
	}