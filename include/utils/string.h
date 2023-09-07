#pragma once

#include <string>

namespace utils::string
	{
	template <typename char_T>
	struct types
		{
		types() = delete;

		using char_t = char_T;

		using string = std::basic_string<char_t>;
		using view   = std::basic_string_view<char_t>;
		using c      = char_t*;

		struct hash;
		};

	namespace concepts
		{
		template <typename T, typename char_t = char>
		concept stringlike = std::convertible_to<T, std::basic_string_view<char_t>>;
		}

	template <typename char_T>
	struct types<char_T>::hash
		{
		using string_t = std::basic_string<char_t>;
		using string_view_t = std::basic_string_view<char_t>;

		using hash_type = std::hash<view>;
		using is_transparent = void;

		template <concepts::stringlike<char_t> T>
		constexpr std::size_t operator()(const T& str) const noexcept { return hash_type{}(str); }
		};
	}