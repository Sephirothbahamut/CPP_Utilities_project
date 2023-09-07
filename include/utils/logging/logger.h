#pragma once
#include <fstream>
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <concepts>

#include "message.h"
#include "../containers/multithreading/self_consuming_queue.h"

namespace utils::logging
	{
	template <typename T>
	class logger
		{
		public:
			using value_type = T;

			logger(const std::string& file_name = "log.txt") : file {file_name} {}

			logger           (const logger&  copy)          = delete ; // don't copy threads please, thank you
			logger& operator=(const logger&  copy)          = delete ; // don't copy threads please, thank you
			logger           (      logger&& move) noexcept = default; //could make those (running is already a flag for the instance being alive or not) but I'm lazy
			logger& operator=(      logger&& move) noexcept = default; //could make those (running is already a flag for the instance being alive or not) but I'm lazy

#pragma region Push messages begin
			void operator<<(const value_type& message) noexcept { push(message); }
			void operator()(const value_type& message) noexcept { push(message); }

			template <typename ...Args>
			void emplace(Args&&... args) noexcept
				{
				message_queue.emplace(std::forward<Args>(args)...);
				}

			void push(const value_type& message) noexcept
				{
				message_queue.push(message);
				}

			void flush() noexcept
				{
				message_queue.flush();
				}

			constexpr void inf(const string::concepts::stringlike auto& string) noexcept requires(concepts::message<value_type>) { push(value_type::inf(string)); }
			constexpr void log(const string::concepts::stringlike auto& string) noexcept requires(concepts::message<value_type>) { push(value_type::log(string)); }
			constexpr void dgn(const string::concepts::stringlike auto& string) noexcept requires(concepts::message<value_type>) { push(value_type::dgn(string)); }
			constexpr void err(const string::concepts::stringlike auto& string) noexcept requires(concepts::message<value_type>) { push(value_type::err(string)); }
			constexpr void wrn(const string::concepts::stringlike auto& string) noexcept requires(concepts::message<value_type>) { push(value_type::wrn(string)); }
#pragma endregion Push messages end

		protected:
			std::ofstream file;

			utils::containers::multithreading::self_consuming_queue<T, utils::containers::multithreading::operation_flag_bits::pre> message_queue
				{
				[this](T& element) -> void
					{
					std::cout << element << std::endl;
					file << element << std::endl;
					},
				[this](std::vector<T>& elements)
					{
					if constexpr (concepts::message<T>)
						{
						std::sort(elements.begin(), elements.end());
						}
					}
				};
		};
	}