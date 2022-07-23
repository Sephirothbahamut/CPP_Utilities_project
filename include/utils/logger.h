#pragma once
#include <fstream>
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>

#include "message.h"

namespace utils
	{
	//TODO check codereview stackexchange and apply changes
	template <typename T>
	class logger
		{
		public:
			using value_type = T;

			logger(const std::string& file_name = "log.txt")
				: thread(&logger::writer, this)
				{
				file.open(file_name);
				}

			logger(const logger& copy) = delete;			// don't copy threads please, thank you
			logger& operator=(const logger& copy) = delete;	// don't copy threads please, thank you
			logger(logger&& move) noexcept = default;	//could make those (running is already a flag for the instance being alive or not) but I'm lazy
			logger& operator=(logger&& move) = default;	//could make those (running is already a flag for the instance being alive or not) but I'm lazy

			~logger() { if (running) { close(); } }

			//Push messages begin
			void operator<<(const value_type& message) noexcept { push(message); }
			void operator()(const value_type& message) noexcept { push(message); }

			template <typename ...Args>
			void emplace(Args&&... args) noexcept
				{
				value_type message{args...};
				push(message);
				}

			void push(const value_type& message) noexcept
				{
				if (true)
					{
					std::lock_guard lock(queue_free);
					queue_log.push(message);
					}
				work_available.notify_one();
				}
			//Push messages end

			void close() noexcept
				{
				running = false;
				work_available.notify_one();
				thread.join();

				queue_write.swap(queue_log);
				write_all();

				file.close();
				}

		protected:
			std::ofstream file;
			std::queue<value_type> queue_log;
			std::queue<value_type> queue_write;
			std::thread thread;

			std::atomic_bool running = true;
			std::mutex queue_free;
			std::condition_variable work_available;


			void writer() noexcept
				{
				while (running)
					{
					if (true)
						{
						std::unique_lock lock{queue_free};
						if (queue_log.empty()) { work_available.wait(lock); }

						queue_write.swap(queue_log);
						}

					write_all();
					}
				}

			void write_all() noexcept
				{
				while (!queue_write.empty())
					{
					value_type& message = queue_write.front();

					std::cout << message << std::endl;
					file << message << std::endl;

					queue_write.pop();
					}
				}

		};

	template <>
	class logger<utils::message>
		{
		public:
			using value_type = utils::message;

			logger(const std::string& file_name = "log.txt")
				: thread(&logger::writer, this)
				{
				file.open(file_name);
				}

			logger(const logger& copy) = delete;			// don't copy threads please, thank you
			logger& operator=(const logger& copy) = delete;	// don't copy threads please, thank you
			logger(logger&& move) noexcept = default;	//could make those (running is already a flag for the instance being alive or not) but I'm lazy
			logger& operator=(logger&& move) = default;	//could make those (running is already a flag for the instance being alive or not) but I'm lazy

			~logger() { if (running) { close(); } }

			//Push messages begin
			void operator<<(const value_type& message) noexcept { push(message); }
			void operator()(const value_type& message) noexcept { push(message); }

			template <typename ...Args>
			void emplace(Args&&... args) noexcept
				{
				value_type message{args...};
				push(message);
				}

			void push(const value_type& message) noexcept
				{
				if (true)
					{
					std::lock_guard lock(queue_free);
					queue_log.push(message);
					}
				work_available.notify_one();
				}

			void inf(std::string&& string) noexcept { push(utils::message::inf(std::move(string))); }
			void log(std::string&& string) noexcept { push(utils::message::log(std::move(string))); }
			void dgn(std::string&& string) noexcept { push(utils::message::dgn(std::move(string))); }
			void err(std::string&& string) noexcept { push(utils::message::err(std::move(string))); }
			void wrn(std::string&& string) noexcept { push(utils::message::wrn(std::move(string))); }

			//Push messages end

			void close() noexcept
				{
				running = false;
				work_available.notify_one();
				thread.join();

				queue_write.swap(queue_log);
				write_all();

				file.close();
				}

		protected:
			std::ofstream file;
			std::queue<value_type> queue_log;
			std::queue<value_type> queue_write;
			std::thread thread;

			std::atomic_bool running = true;
			std::mutex queue_free;
			std::condition_variable work_available;


			void writer() noexcept
				{
				while (running)
					{
					if (true)
						{
						std::unique_lock lock{queue_free};
						if (queue_log.empty()) { work_available.wait(lock); }

						queue_write.swap(queue_log);
						}

					write_all();
					}
				}

			void write_all() noexcept
				{
				while (!queue_write.empty())
					{
					value_type& message = queue_write.front();

					std::cout << message << std::endl;
					file << message << std::endl;

					queue_write.pop();
					}
				}

		};
	}

namespace utils::globals
	{
	inline utils::logger<utils::message> logger;
	}