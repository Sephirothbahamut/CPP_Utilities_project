#pragma once
#include <fstream>
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <concepts>

#include "message.h"
#include "containers/multithreading/self_consuming_queue.h"

namespace utils
	{
	//TODO check codereview stackexchange and apply changes
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

			//Push messages begin
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
			//Push messages end

			void inf(std::string&& string) noexcept requires std::same_as<T, utils::message> { push(utils::message::inf(std::move(string))); }
			void log(std::string&& string) noexcept requires std::same_as<T, utils::message> { push(utils::message::log(std::move(string))); }
			void dgn(std::string&& string) noexcept requires std::same_as<T, utils::message> { push(utils::message::dgn(std::move(string))); }
			void err(std::string&& string) noexcept requires std::same_as<T, utils::message> { push(utils::message::err(std::move(string))); }
			void wrn(std::string&& string) noexcept requires std::same_as<T, utils::message> { push(utils::message::wrn(std::move(string))); }

		protected:
			std::ofstream file;

			utils::containers::multithreading::self_consuming_queue<T, utils::containers::multithreading::operation_flag_bits::current | utils::containers::multithreading::operation_flag_bits::pre> message_queue
				{
				[this](T& element) -> void
					{
					std::cout << element << std::endl;
					file << element << std::endl;
					},
				[this](std::vector<T>& elements)
					{
					if constexpr (std::same_as<T, utils::message>)
						{
						//TODO reimplement get_timestamp in some way for the sorting
						//std::sort(elements.begin(), elements.end(), [](const utils::message& a, const utils::message& b) { return a.get_timestamp().time_since_epoch().count() < b.get_timestamp().time_since_epoch().count(); });
						}
					}
				};
		};
	}

#ifndef UTILS_NO_GLOBALS
#ifndef UTILS_LOGGER_NO_GLOBALS
namespace utils::globals
	{
	inline utils::logger<utils::message> logger;
	}
#endif
#endif