#pragma once

#include <thread>
#include <atomic>
#include <functional>
#include <condition_variable>
#include "producer_consumer_queue.h"

namespace utils::containers
	{
	template <typename T>
	class self_consuming_queue : public producer_consumer_queue<T>
		{
		public:
			self_consuming_queue(std::function<void(std::vector<T>&)> consume) : consume{ consume }, thread{ &self_consuming_queue::consumer, this } {}
			using base_t = producer_consumer_queue<T>;

			~self_consuming_queue()
				{
				inner_flush();
				}

			template <typename ...Args>
			void emplace(Args&&... args) noexcept
				{
				base_t::emplace(std::forward<Args>(args)...);
				work_available.notify_one();
				}

			void push(const base_t::value_type& message) noexcept
				{
				base_t::push(message);
				work_available.notify_one();
				}

			void flush()
				{
				inner_flush();
				base_t::producer_data.clear();
				running = true;
				thread = std::thread{ &self_consuming_queue::consumer, this };
				}

		private:
			std::function<void(std::vector<T>&)> consume;

			void inner_flush()
				{
				running = false;
				work_available.notify_one();
				thread.join();

				consume(base_t::producer_data);
				}

			void consumer() noexcept
				{
				while (running)
					{
					if (true)
						{
						std::unique_lock lock{ base_t::queues_access_mutex };
						if (base_t::producer_data.empty()) { work_available.wait(lock); }

						base_t::consumer_data.clear();
						base_t::consumer_data.swap(base_t::producer_data);
						}

					consume(base_t::consumer_data);
					}
				}

			std::atomic_bool running{ true };
			std::condition_variable work_available;

			std::thread thread;
		};
	}