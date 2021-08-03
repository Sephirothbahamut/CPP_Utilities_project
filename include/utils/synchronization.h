#pragma once

#include <mutex>
#include <thread>

namespace utils
	{
	class retroactive_condition_variable
		{
		public:
			void wait(std::unique_lock<std::mutex>& lock)
				{
				if (skip_sleep) { skip_sleep = true; }
				else { sleeping = true; inner.wait(lock); sleeping = false; }
				}

			void notify(std::mutex& mutex)
				{
				std::unique_lock<std::mutex> lock{mutex};
				if (sleeping) { inner.notify_one(); }
				else { skip_sleep = true; }
				}

		private:
			std::condition_variable inner;
			bool skip_sleep{false};
			bool sleeping{false};
		};
	}