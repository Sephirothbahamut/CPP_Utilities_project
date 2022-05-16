#include <vector>
#include <deque>
#include <stdexcept>
#include <cstddef>
#include <array>
#include <concepts>
#include <string>

#include "../compilation/debug.h"

namespace utils::containers
	{
	class buffer
		{
		public:

			template <typename T>
			void push(T value) noexcept
				{
				static_assert(std::is_trivially_copyable<T>::value);
				std::byte* to_push = reinterpret_cast<std::byte*>(&value);
				for (size_t i = 0; i < sizeof(T); i++) { data.push_back(to_push[i]); }
				}

			template <>
			void push<std::string>(std::string value) noexcept
				/*template <typename T>
				void push<std::basic_string<T>>(T value) noexcept*/
				{
				size_t size = value.size();
				std::byte* to_size = reinterpret_cast<std::byte*>(size);
				for (size_t i = 0; i < sizeof(decltype(size)); i++) 
					{ data.push_back(to_size[i]); }

				/*std::byte* to_push = reinterpret_cast<std::byte*>(value.data());
				for (size_t i = 0; i < sizeof(to_push); i++) { data.push_back(to_push[i]); }*/
				}

			template <typename T>
			T get() utils_if_release(noexcept)
				{
				static_assert(std::is_trivially_copyable<T>());
				utils_if_debug(check_bytes_sufficient<T>());

				std::array<std::byte, sizeof(T)> pulled;
				for (size_t i = 0; i < sizeof(T); i++) { pulled[i] = data.front(); data.pop_front(); }

				//return *reinterpret_cast<T*>(pulled.data()); //bad because the array is not guaranteed to be aligned by T, it is aligned by byte. memcpy copies bytes, solving alignment issues
				T tmp;
				std::memcpy(&tmp, pulled.data(), sizeof(T));
				return tmp;
				}

			template <typename T>
			T& get_except()
				{
				static_assert(std::is_trivially_copyable<T>());
				check_bytes_sufficient<T>();
				return get<T>();
				}

			template <typename T>
			bool has() const noexcept { return (data.size() >= sizeof(T)); }

			bool empty() const noexcept { return data.empty(); }
			size_t size() const noexcept { return data.size(); }

		private:
			std::deque<std::byte> data;

			template <typename T>
			void check_bytes_sufficient() { if (has<T>()) { throw std::runtime_error{"The stream does not contain enough bytes to retrive type T"}; } }
		};
	}
