#pragma once
#include <fstream>
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>

namespace utils
	{
	//TODO
	// With C++20 replace the time_points with actual readable time https://stackoverflow.com/questions/62954221/c-print-chronotime-pointchronohigh-resolution-clock-to-be-readable
	template <typename Message_type>
	class logger
		{
		private:
			std::ofstream file;
			std::queue<Message_type> queue_log;
			std::queue<Message_type> queue_write;
			std::thread thread;

			std::atomic_bool running = true;
			std::mutex queue_free;
			std::condition_variable work_available;


			void writer()
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

			void write_all()
				{
				while (!queue_write.empty())
					{
					Message_type& message = queue_write.front();

					std::cout << message << std::endl;
					file << message << std::endl;

					queue_write.pop();
					}
				}

		public:
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
			void operator<<(const Message_type& message) { push(message); }
			void operator()(const Message_type& message) { push(message); }

			template <typename ...Args>
			void emplace(Args&&... args)
				{
				Message_type message{args...};
				push(message);
				}

			void push(const Message_type& message)
				{
				if (true)
					{
					std::lock_guard lock(queue_free);
					queue_log.push(message);
					}
				work_available.notify_one();
				}
			//Push messages end

			void close()
				{
				running = false;
				work_available.notify_one();
				thread.join();

				queue_write.swap(queue_log);
				write_all();

				file.close();
				}
		};
	}