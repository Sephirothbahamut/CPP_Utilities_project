#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "id_pool.h"

namespace utils
	{
	namespace concepts
		{
		template <typename T, typename char_t = char>
		concept stringlike = std::same_as<std::remove_cvref_t<T>, std::basic_string<char_t>>
			|| std::same_as<std::remove_cvref_t<T>, std::basic_string_view<char_t>>
			|| std::same_as<std::remove_cvref_t<T>, char_t*>;
		}

	template <typename char_t>
	struct string_hash
		{
		using string_t = std::basic_string<char_t>;
		using string_view_t = std::basic_string_view<char_t>;

		using hash_type = std::hash<std::string_view>;
		using is_transparent = void;

		constexpr std::size_t operator()(const char_t*        str) const noexcept { return hash_type{}(str); }
		constexpr std::size_t operator()(string_t             str) const noexcept { return hash_type{}(str); }
		constexpr std::size_t operator()(string_view_t const& str) const noexcept { return hash_type{}(str); }
		};



	template <typename char_t>
	class name
		{
		public:
			using string_t      = std::basic_string<char_t>;
			using string_view_t = std::basic_string_view<char_t>;
			using c_str_t       = const char_t*;
			using id_t          = size_t;

		private:
			class names_database
				{
				public:
					id_t emplace(std::string_view string) noexcept
						{
						auto id{id_pool.get()};
						auto ret{string_to_id.emplace(std::piecewise_construct, std::forward_as_tuple(string), std::forward_as_tuple(id))};
						if (!ret.second)//if was not emplaced, something already existed with that key
							{
							id_pool.release(id);
							return ret.first->second;
							}
						else
							{
							id_to_string.emplace_back(ret.first->first);
							return id;
							}
						}

					std::string_view operator[](id_t id) const noexcept { return id_to_string[id]; }

				private:
					std::vector<string_view_t> id_to_string;
					std::unordered_map<string_t, id_t, string_hash<char_t>> string_to_id;
					utils::id_pool_manual id_pool;
				};

		public:
			name(string_view_t string) : id{names_database.emplace(string)} {};
			name(c_str_t       string) : id{names_database.emplace(string)} {};

			std::strong_ordering operator<=>(const name& other) const noexcept { return id <=> other.id; }
			bool operator== (const name& other) const noexcept { return id ==  other.id; }

			std::string_view operator*() const noexcept { return names_database[id]; }
			std::string_view as_string() const noexcept { return operator*(); }
			operator std::string_view () const noexcept { return operator*(); }

		private:
			id_t id;
			inline static names_database names_database;
		};
	}