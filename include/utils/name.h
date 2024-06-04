#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "string.h"
#include "id_pool.h"

namespace utils
	{
	template <typename char_t = char>
	class basic_name
		{
		public:
			using id_t  = size_t;
			using types = utils::string::types<char_t>;

		private:
			class names_database
				{
				public:
					template <utils::string::concepts::stringlike<char_t> T>
					id_t emplace(const T& string) noexcept
						{
						auto new_id{id_pool.get()};
						auto ret{string_to_id.emplace(std::piecewise_construct, std::forward_as_tuple(string), std::forward_as_tuple(new_id))};
						if (!ret.second)//if was not emplaced, something already existed with that key
							{
							id_pool.release(new_id);
							return ret.first->second;
							}
						else
							{
							id_to_string.emplace_back(ret.first->first);
							return new_id;
							}
						}

					types::view operator[](id_t in_id) const noexcept { return id_to_string[in_id]; }

				private:
					std::vector<typename types::view> id_to_string;
					std::unordered_map<typename types::string, id_t, typename types::hash> string_to_id;
					utils::id_pool_manual id_pool;
				};

		public:
			template <utils::string::concepts::stringlike<char_t> T>
			basic_name(const T& string) : id{names_database.emplace(string)} {};

			std::strong_ordering operator<=>(const basic_name<char_t>& other) const noexcept { return id <=> other.id; }
			bool operator== (const basic_name<char_t>& other) const noexcept { return id ==  other.id; }

			types::view operator*() const noexcept { return names_database[id]; }
			types::view as_string() const noexcept { return operator*(); }
			operator types::view() const noexcept { return operator*(); }

			size_t hash() const noexcept { return id; }

		private:
			id_t id;
			inline static names_database names_database;
		};
	
	using name    = basic_name<char>;
	using wname   = basic_name<wchar_t>;
	using u8name  = basic_name<char8_t>;
	using u16name = basic_name<char16_t>;
	using u32name = basic_name<char32_t>;
	}

namespace std
	{
	template <typename char_t>
	struct hash<utils::basic_name<char_t>>
		{
		size_t operator()(const utils::basic_name<char_t>& value) const noexcept { return value.hash(); }
		};
	}