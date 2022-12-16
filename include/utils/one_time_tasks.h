#pragma once

#include <functional>

#include "thread_pool.h"
#include "containers/multithreading/concurrent_queue.h"

namespace iige
	{
	class one_time_tasks
		{
		using inner_container_t = utils::containers::multithreading::concurrent_queue<std::function<void()>>;

		public:
			void push(const std::function<void()>& callable) { inner_container.enqueue(callable); }
			void execute()
				{
				std::function<void()> callable;
				while (inner_container.try_dequeue(callable)) { callable(); }
				}

			void execute(utils::thread_pool& thread_pool)
				{
				std::vector<std::future<void>> futures;

				std::function<void()> callable;
				while (inner_container.try_dequeue(callable))
					{
					futures.emplace_back(thread_pool.submit(callable));
					}

				for (auto& future : futures)
					{
					if (future.valid()) { future.wait(); }
					}
				}

		private:
			inner_container_t inner_container;
		};

	class one_time_tasks_counted
		{
		using inner_container_t = utils::containers::multithreading::concurrent_queue<std::function<void()>>;

		public:
			void push(const std::function<void()>& callable) { inner_container.enqueue(callable); count++; }
			void execute()
				{
				std::function<void()> callable;
				while (inner_container.try_dequeue(callable)) { count--; callable(); }
				}

			void execute(utils::thread_pool& thread_pool)
				{
				std::vector<std::future<void>> futures;

				std::function<void()> callable;
				while (inner_container.try_dequeue(callable))
					{
					count--;
					futures.emplace_back(thread_pool.submit(callable));
					}

				for (auto& future : futures)
					{
					if (future.valid()) { future.wait(); }
					}
				}

			size_t size() { return  count; }
			bool  empty() { return !count; }

		private:
			std::atomic_size_t count;

			inner_container_t inner_container;
		};
	}