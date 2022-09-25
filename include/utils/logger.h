#pragma once
#include <fstream>
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <concepts>

#include "message.h"
#include "containers/self_consuming_queue.h"

namespace utils
	{
	//TODO check codereview stackexchange and apply changes
	template <typename T>
	class logger
		{
		public:
			using value_type = T;

			logger(const std::string& file_name = "log.txt") : file {file_name} {}

			logger(const logger& copy) = delete;			// don't copy threads please, thank you
			logger& operator=(const logger& copy) = delete;	// don't copy threads please, thank you
			logger(logger&& move) noexcept = default;	//could make those (running is already a flag for the instance being alive or not) but I'm lazy
			logger& operator=(logger&& move) = default;	//could make those (running is already a flag for the instance being alive or not) but I'm lazy

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

			utils::containers::self_consuming_queue<T> message_queue
				{
				[this](std::vector<T>& to_load) -> void
					{
					for (auto& message : to_load)
						{
						std::cout << message << std::endl;
						file << message << std::endl;
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